//
// Created by Michael Wittmann on 12/06/2020.
//

#ifndef SDL2TEST_OPENGLMANAGER_HPP
#define SDL2TEST_OPENGLMANAGER_HPP

#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>  // Initialize with glewInit()

#include <algorithm>
#include <array>
#include <string>
#include <vector>

#include "Shader.hpp"

namespace app {

class OpenGLManager {
 private:
  Shader shader_{};
  unsigned int VBO_{0};
  unsigned int VAO_{0};
  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices_[9] = {
      -0.5f, -0.5f, 0.0f,  // left
      0.5f,  -0.5f, 0.0f,  // right
      0.0f,  0.5f,  0.0f   // top
  };
  void InitializeVertexBuffer();

 public:
  // Called after the window and OpenGL are initialized. Called exactly once,
  // before the main loop.
  void init();
  // Called to update the display.
  void render();
  // Called whenever the window is resized. The new window size is given, in
  // pixels. This is an opportunity to call glViewport or glScissor to keep up
  // with the change in size.
  void reshape(int w, int h);
  void clearResources();
};

}  // namespace app

#endif  // SDL2TEST_OPENGLMANAGER_HPP
