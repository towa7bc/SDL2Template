//
// Created by Michael Wittmann on 06/06/2020.
//

#define GL_SILENCE_DEPRECATION

#include "App.hpp"

#include <GL/glew.h>  // Initialize with glewInit()
#include <SDL.h>

#include <future>
#include <memory>
#include <string>

#include "detail/Log.hpp"
#include "detail/SDL2Core.hpp"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

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
  ImGui_ImplSDL2_ProcessEvent(&event);
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
          openGlManager_.reshape(displaySize_.x, displaySize_.y);
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

void App::RenderImGUI() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(window_);
  ImGui::NewFrame();

  auto f2 = std::async(std::launch::async, [&]() {
    static float triangleColor = 0.0f;
    static int counter = 0;

    ImGui::Begin("Triangle Slider");  // Create a window called "Hello, world!"
    // and append into it.

    ImGui::Text("Slide to change the color of the triangle.");  // Display some
                                                                // text (you can
    // use a format strings too)
    ImGui::SliderFloat("float", &triangleColor, 0.0f,
                       1.0f);  // Edit 1 float using a slider from 0.0f to 1.0f

    if (ImGui::Button(
            "Button")) {  // Buttons return true when clicked (most widgets
      // return true when edited/activated)
      counter++;
    }
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
  });
}

void App::Render() {
  // Rendering
  ImGui::Render();
  glViewport(0, 0, (int)displaySize_.x, (int)displaySize_.y);
  glClearColor(clear_color_.x, clear_color_.y, clear_color_.z, clear_color_.w);
  glClear(GL_COLOR_BUFFER_BIT);

  openGlManager_.render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
  const char* glsl_version = "#version 410 core";
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
  SDL_WindowFlags window_flags = (SDL_WindowFlags)(
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
  // Initialize OpenGL loader
  bool err = glewInit() != GLEW_OK;
  if (err) {
    return 1;
  }
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable
  // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
  // Enable Gamepad Controls
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  displaySize_ = io.DisplaySize;
  // Setup Platform/Renderer bindings
  ImGui_ImplSDL2_InitForOpenGL(window_, glContext_);
  ImGui_ImplOpenGL3_Init(glsl_version);
  io.Fonts->AddFontDefault();
  io.Fonts->AddFontFromFileTTF("../../src/fonts/Roboto-Medium.ttf", 16.0f);
  io.Fonts->AddFontFromFileTTF("../../src/fonts/Cousine-Regular.ttf", 15.0f);
  io.Fonts->AddFontFromFileTTF("../../src/fonts/DroidSans.ttf", 16.0f);
  io.Fonts->AddFontFromFileTTF("../../src/fonts/ProggyTiny.ttf", 10.0f);
  io.Fonts->AddFontFromFileTTF("../../src/fonts/ARIALUNI.ttf", 18.0f, NULL,
                               io.Fonts->GetGlyphRangesJapanese());
  clear_color_ = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  openGlManager_.init();
  return true;
  //});
  // return f2.get();
}

App::App() : running_(true), window_(nullptr), glContext_() {}

void App::UpdateFPS() {}

void App::Cleanup() {
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_GL_DeleteContext(glContext_);
  cleanup(window_);
  openGlManager_.clearResources();
  SDL_Quit();
}

void App::Update() {}

}  // namespace app