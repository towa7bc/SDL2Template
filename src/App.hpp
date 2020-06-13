//
// Created by Michael Wittmann on 06/06/2020.
//

#ifndef SDL2TEST_APP_HPP
#define SDL2TEST_APP_HPP

#include <GL/glew.h>  // Initialize with glewInit()
#include <SDL.h>

#include <array>
#include <string>
#include <vector>

#include "OpenGLManager.hpp"
#include "detail/Core.hpp"  // for create_ref
#include "imgui.h"

namespace app {
class App {
 public:
  App();
  int Run();

 private:
  bool Setup();
  void Update();
  void Render();
  void RenderImGUI();
  void HandleSDL2Events(const SDL_Event &);
  void UpdateFPS();
  void Cleanup();
  bool running_;
  SDL_Window *window_;
  static constexpr int windowDefaultWidth_ = 1024;
  static constexpr int windowDefaultHeight_ = 728;
  ImVec4 clear_color_;
  ImVec2 displaySize_;
  SDL_GLContext glContext_;
  OpenGLManager openGlManager_;
};

}  // namespace app

#endif  // SDL2TEST_APP_HPP
