//
// Created by Michael Wittmann on 13/06/2020.
//

#define GL_SILENCE_DEPRECATION
#include "ImGUIHelper.hpp"

#include <GL/glew.h>  // Initialize with glewInit()

#include <future>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
namespace app {
float ImGUIHelper::triangleColor_;
}

namespace app {

bool ImGUIHelper::Setup(SDL_Window* window, SDL_GLContext* glContext,
                        ImVec4& clear_color) {
  ImGUIHelper::triangleColor_ = .5f;
  const char* glsl_version = "#version 410 core";
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
  // Setup Platform/Renderer bindings
  ImGui_ImplSDL2_InitForOpenGL(window, glContext);
  ImGui_ImplOpenGL3_Init(glsl_version);
  io.Fonts->AddFontDefault();
  io.Fonts->AddFontFromFileTTF("../../src/fonts/Roboto-Medium.ttf", 16.0f);
  io.Fonts->AddFontFromFileTTF("../../src/fonts/Cousine-Regular.ttf", 15.0f);
  io.Fonts->AddFontFromFileTTF("../../src/fonts/DroidSans.ttf", 16.0f);
  io.Fonts->AddFontFromFileTTF("../../src/fonts/ProggyTiny.ttf", 10.0f);
  io.Fonts->AddFontFromFileTTF("../../src/fonts/ARIALUNI.ttf", 18.0f, NULL,
                               io.Fonts->GetGlyphRangesJapanese());
  clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  return false;
}

void ImGUIHelper::RenderUI(SDL_Window* window) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(window);
  ImGui::NewFrame();
  auto f2 = std::async(std::launch::async, []() {
    ImGui::Begin("Triangle Slider");

    ImGui::Text("Slide to change the color of the triangle.");  // Display some
    if (ImGui::SliderFloat("Color", &triangleColor_, 0.0f, 1.0f, "")) {
    }

    ImGui::Text("%.0f FPS", ImGui::GetIO().Framerate);
    ImGui::End();
  });
}

void ImGUIHelper::RenderPrepare(ImVec2& displaySize, ImVec4& clear_color) {
  // Rendering
  ImGui::Render();
  glViewport(0, 0, (int)displaySize.x, (int)displaySize.y);
  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
}

void ImGUIHelper::ProcessEvent(const SDL_Event& event) {
  ImGui_ImplSDL2_ProcessEvent(&event);
}

void ImGUIHelper::Cleanup() {
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

void ImGUIHelper::RenderDrawData() {
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}  // namespace app