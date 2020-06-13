//
// Created by Michael Wittmann on 13/06/2020.
//

#ifndef SDL2TEST_IMGUIHELPER_HPP
#define SDL2TEST_IMGUIHELPER_HPP

#include <SDL.h>

#include "imgui.h"

namespace app {

class ImGUIHelper {
 public:
  static bool Setup(SDL_Window* window, SDL_GLContext* glContext,
                    ImVec4& clear_color);
  static void RenderUI(SDL_Window* windstatic);
  static void RenderPrepare(ImVec2& displaySize, ImVec4& clear_color);
  static void ProcessEvent(const SDL_Event& event);
  static void Cleanup();
  static void RenderDrawData();
  static float triangleColor_;
};

}  // namespace app

#endif  // SDL2TEST_IMGUIHELPER_HPP
