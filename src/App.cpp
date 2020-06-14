//
// Created by Michael Wittmann on 06/06/2020.
//

#include "App.hpp"

#include <GL/glew.h>  // Initialize with glewInit()
#include <SDL.h>

#include <memory>
#include <string>

#include "ImGUIHelper.hpp"
#include "detail/Log.hpp"
#include "detail/SDL2Core.hpp"

namespace app {

int App::Run() {
  if (!Setup()) {
    return -1;
  }
  SDL_Event sdlEvent;
  while (running_) {
    while (SDL_PollEvent(&sdlEvent) != 0) {
      HandleSDL2Events(sdlEvent);
    }
    UpdateFPS();
    Update();
    RenderImGUI();
    Render();
  }
  Cleanup();
  return 0;
}

void App::HandleSDL2Events(const SDL_Event& event) {
  ImGUIHelper::ProcessEvent(event);
  switch (event.type) {
    case SDL_QUIT:
      running_ = false;
      break;
    case SDL_WINDOWEVENT:
      if (event.window.event == SDL_WINDOWEVENT_CLOSE &&
          event.window.windowID == SDL_GetWindowID(window_)) {
        running_ = false;
      }
      break;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
        case SDLK_f: {
          static bool isFullscreen{true};
          SDL_SetRelativeMouseMode(SDL_TRUE);
          if (isFullscreen) {
            SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN);
          } else {
            SDL_SetWindowFullscreen(window_, 0);
          }
          isFullscreen = !isFullscreen;
          int w{0};
          int h{0};
          SDL_GetWindowSize(window_, &w, &h);
          openGlManager_.reshape(w, h);
          SDL_SetRelativeMouseMode(SDL_FALSE);
          break;
        }
        case SDLK_q:
          running_ = false;
          break;
        default:
          break;
      }
      break;
    case SDL_MOUSEBUTTONDOWN:
      break;
    default:
      break;
  }
}

void App::RenderImGUI() { ImGUIHelper::RenderUI(window_); }

void App::Render() {
  int w{0};
  int h{0};
  SDL_GetWindowSize(window_, &w, &h);
  displaySize_.x = w;
  displaySize_.y = h;
  ImGUIHelper::RenderPrepare(displaySize_, clear_color_);
  openGlManager_.render();
  ImGUIHelper::RenderDrawData();
  SDL_GL_SwapWindow(window_);
}

bool App::Setup() {
  if (Log::logger() == nullptr) {
    Log::Initialize();
  }
  Log::logger()->trace("program started.");
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    return false;
  }
  SDL_GL_SetAttribute(
      SDL_GL_CONTEXT_FLAGS,
      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);  // Always required on Mac
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  // Create window with graphics context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  auto window_flags = (SDL_WindowFlags)(
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  if ((window_ = SDL_CreateWindow("Particle Example", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, windowDefaultWidth_,
                                  windowDefaultHeight_, window_flags)) ==
      nullptr) {
    Log::logger()->trace("Window error: " + std::string(SDL_GetError()));
    return false;
  }

  glContext_ = SDL_GL_CreateContext(window_);
  SDL_GL_MakeCurrent(window_, glContext_);
  SDL_GL_SetSwapInterval(1);
  App::RunOpenGLLoader();
  clear_color_ = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  ImGUIHelper::Setup(window_, &glContext_, clear_color_);
  openGlManager_.init();
  return true;
}

App::App() : running_(true), window_(nullptr), glContext_(), openGlManager_() {}

void App::UpdateFPS() {}

void App::Cleanup() {
  // Cleanup
  ImGUIHelper::Cleanup();
  SDL_GL_DeleteContext(glContext_);
  cleanup(window_);
  openGlManager_.clearResources();
  SDL_Quit();
}

void App::Update() {}

bool App::RunOpenGLLoader() {
  bool err = glewInit() != GLEW_OK;
  return !err;
}

}  // namespace app