//
// Created by Michael Wittmann on 14/06/2020.
//

#ifndef SDL2TEST_IMAGELOADER_HPP
#define SDL2TEST_IMAGELOADER_HPP

#include <GL/glew.h>
#include <SDL.h>

#include <string>

namespace app {

class ImageLoader {
 public:
  static GLuint CreateOpenGLTextureFromSDL_Surface(SDL_Surface* surface,
                                                   bool mipmap = false);
  static GLuint CreateOpenGLTextureFromImageFile(const std::string& filename,
                                                 bool mipmap = false);
};

}  // namespace app

#endif  // SDL2TEST_IMAGELOADER_HPP
