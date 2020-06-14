//
// Created by Michael Wittmann on 14/06/2020.
//

#include "ImageLoader.hpp"

#include <SDL_image.h>

namespace app {

GLuint ImageLoader::CreateOpenGLTextureFromSDL_Surface(SDL_Surface* surface,
                                                       bool mipmap) {
  GLuint texture = 0;
  if ((bool)surface) {
    GLenum texture_format = 0;
    GLenum internal_format = 0;
    GLenum tex_type = 0;
    if (surface->format->BytesPerPixel == 4) {
      if (surface->format->Rmask == 0x000000ff) {
        texture_format = GL_RGBA;
        tex_type = GL_UNSIGNED_INT_8_8_8_8_REV;
      } else {
        texture_format = GL_BGRA;
        tex_type = GL_UNSIGNED_INT_8_8_8_8;
      }
      internal_format = GL_RGBA8;
    } else {
      if (surface->format->Rmask == 0x000000ff) {
        texture_format = GL_RGB;
        tex_type = GL_UNSIGNED_BYTE;
      } else {
        texture_format = GL_BGR;
        tex_type = GL_UNSIGNED_BYTE;
      }
      internal_format = GL_RGB8;
    }

    int alignment = 8;
    while ((surface->pitch % alignment) != 0) {
      alignment >>= 1;  // x%1==0 for any x
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

    int expected_pitch =
        (surface->w * surface->format->BytesPerPixel + alignment - 1) /
        alignment * alignment;
    if (surface->pitch - expected_pitch >=
        alignment) {  // Alignment alone wont't solve it now
      glPixelStorei(GL_UNPACK_ROW_LENGTH,
                    surface->pitch / surface->format->BytesPerPixel);
    } else {
      glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, surface->w, surface->h, 0,
                 texture_format, tex_type, surface->pixels);
    if (mipmap) {
      glGenerateMipmap(GL_TEXTURE_2D);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
    } else {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  }
  SDL_FreeSurface(surface);
  return texture;
}

GLuint ImageLoader::CreateOpenGLTextureFromImageFile(
    const std::string& filename, bool mipmap) {
  SDL_Surface* surface = IMG_Load(filename.data());
  return CreateOpenGLTextureFromSDL_Surface(surface, mipmap);
}

}  // namespace app