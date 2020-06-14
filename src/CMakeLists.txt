# Generic test that uses conan libs

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

add_executable(
        SDL2Test
        main.cpp
        App.cpp
        App.hpp
        detail/Core.hpp
        detail/Log.cpp
        detail/Log.hpp
        detail/SDL2Core.hpp
        imgui_impl_opengl3.cpp
        imgui_impl_opengl3.h
        imgui_impl_sdl.cpp
        imgui_impl_sdl.h
        OpenGLManager.cpp
        OpenGLManager.hpp
        ImGUIHelper.cpp ImGUIHelper.hpp Shader.cpp Shader.hpp)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_gfx REQUIRED)
find_package(SDL2_net REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(
        SDL2Test
        PRIVATE project_options
        project_warnings
        CONAN_PKG::catch2
        CONAN_PKG::spdlog
        CONAN_PKG::glm
        CONAN_PKG::imgui
        CONAN_PKG::glew
        OpenGL::GL
        OpenGL::GLU
        SDL2::Main
        SDL2::Image
        SDL2::Mixer
        SDL2::GFX
        SDL2::Net
        SDL2::TTF
)
