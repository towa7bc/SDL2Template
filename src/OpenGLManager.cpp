//
// Created by Michael Wittmann on 12/06/2020.
//

#include "OpenGLManager.hpp"

#include <GL/glew.h>  // Initialize with glewInit()
#include <SDL.h>

#include <algorithm>
#include <cmath>
#include <vector>

#include "ImGUIHelper.hpp"

namespace app {

GLuint OpenGLManager::CreateShader(GLenum eShaderType,
                                   const std::string &strShaderFile) {
  GLuint shader = glCreateShader(eShaderType);
  const char *strFileData = strShaderFile.c_str();
  glShaderSource(shader, 1, &strFileData, nullptr);

  glCompileShader(shader);

  GLint status{0};
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    GLint infoLogLength{0};
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

    std::string strInfoLog;
    glGetShaderInfoLog(shader, infoLogLength, nullptr, strInfoLog.data());

    const char *strShaderType = nullptr;
    switch (eShaderType) {
      case GL_VERTEX_SHADER:
        strShaderType = "vertex";
        break;
      case GL_GEOMETRY_SHADER:
        strShaderType = "geometry";
        break;
      case GL_FRAGMENT_SHADER:
        strShaderType = "fragment";
        break;
      default:
        break;
    }
  }

  return shader;
}

GLuint OpenGLManager::CreateProgram(const std::vector<GLuint> &shaderList) {
  GLuint program = glCreateProgram();

  for (unsigned int iLoop : shaderList) {
    glAttachShader(program, iLoop);
  }

  glLinkProgram(program);

  GLint status{0};
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    GLint infoLogLength{0};
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

    std::string strInfoLog;
    glGetProgramInfoLog(program, infoLogLength, nullptr, strInfoLog.data());
  }

  for (unsigned int iLoop : shaderList) {
    glDetachShader(program, iLoop);
  }

  return program;
}

void OpenGLManager::InitializeProgram() {
  std::vector<GLuint> shaderList;

  shaderList.push_back(CreateShader(GL_VERTEX_SHADER, vertexShaderSource_));
  shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSource_));

  shaderProgram_ = CreateProgram(shaderList);

  std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}

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
  InitializeProgram();
  InitializeVertexBuffer();
}

// Called to update the display.
// You should call glutSwapBuffers after all of your rendering to display what
// you rendered. If you need continuous updates of the screen, call
// glutPostRedisplay() at the end of the function.
void OpenGLManager::render() const {
  // draw our first triangle
  glUseProgram(shaderProgram_);
  int vertexColorLocation = glGetUniformLocation(shaderProgram_, "ourColor");
  glUniform4f(vertexColorLocation, 0.0f, ImGUIHelper::triangleColor_, 0.0f,
              1.0f);
  glBindVertexArray(
      VAO_);  // seeing as we only have a single VAO there's no need to bind it
  // every time, but we'll do so to keep things a bit more organized
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glUseProgram(0);
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
  glDeleteProgram(shaderProgram_);
}

}  // namespace app