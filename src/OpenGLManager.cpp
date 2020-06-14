//
// Created by Michael Wittmann on 12/06/2020.
//

#include "OpenGLManager.hpp"

#include <GL/glew.h>  // Initialize with glewInit()

#include "ImGUIHelper.hpp"

namespace app {

void OpenGLManager::InitializeVertexBuffer() {
  // unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(VAO_);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO
  // as the vertex attribute's bound vertex buffer object so afterwards we can
  // safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally
  // modify this VAO, but this rarely happens. Modifying other VAOs requires a
  // call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
  // VBOs) when it's not directly necessary.
  glBindVertexArray(0);
}

// Called after the window and OpenGL are initialized. Called exactly once,
// before the main loop.
void OpenGLManager::init() {
  // InitializeProgram();
  shader_.Load("../../src/vert.glsl", "../../src/frac.glsl");
  InitializeVertexBuffer();
}

// Called to update the display.
// You should call glutSwapBuffers after all of your rendering to display what
// you rendered. If you need continuous updates of the screen, call
// glutPostRedisplay() at the end of the function.
void OpenGLManager::render() {
  shader_.use();
  shader_.manipulateUniformFloat("ourColor", ImGUIHelper::triangleColor_);
  glBindVertexArray(VAO_);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  shader_.unUse();
}

// Called whenever the window is resized. The new window size is given, in
// pixels. This is an opportunity to call glViewport or glScissor to keep up
// with the change in size.
void OpenGLManager::reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void OpenGLManager::clearResources() {
  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO_);
  glDeleteBuffers(1, &VBO_);
}

}  // namespace app