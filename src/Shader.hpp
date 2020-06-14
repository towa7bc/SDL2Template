//
// Created by Michael Wittmann on 14/06/2020.
//

#ifndef SDL2TEST_SHADER_HPP
#define SDL2TEST_SHADER_HPP

#include <GL/glew.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace app {

class Shader {
 public:
  unsigned int shaderProgramId_;
  // constructor generates the shader on the fly
  // ------------------------------------------------------------------------
  void Load(const char* vertexPath, const char* fragmentPath);
  // activate the shader
  // ------------------------------------------------------------------------
  void use();
  void unUse();
  // utility uniform functions
  // ------------------------------------------------------------------------
  void setBool(const std::string& name, bool value) const;
  // ------------------------------------------------------------------------
  void setInt(const std::string& name, int value) const;
  // ------------------------------------------------------------------------
  void setFloat(const std::string& name, float value) const;
  void manipulateUniformFloat(const std::string& name, float& value) const;

 private:
  // utility function for checking shader compilation/linking errors.
  // ------------------------------------------------------------------------
  void checkCompileErrors(unsigned int shader, std::string type);
};

}  // namespace app

#endif  // SDL2TEST_SHADER_HPP
