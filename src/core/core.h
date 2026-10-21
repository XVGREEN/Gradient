#pragma once
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include "../state/globals.h"
#include "open_file_dialog.h"
#include "save_as_dialog.h"
namespace gradient {

    extern GLuint compileShader(GLenum type, const char* src);
    extern GLuint createShaderProgram(const char* vertexSrc, const char* fragmentSrc);
    extern  "C" unsigned char* stbi_load(char const* filename, int* x, int* y, int* channels_in_file, int desired_channels);
    extern "C" void stbi_image_free(void* retval_from_stbi_load);
    extern unsigned int load_texture(const char* path, float& w, float& h);
    extern void setupFullscreenQuad();
    extern void drawImageQuad(int x, int y, int width, int height, float time, glm::vec2 mouse, bool preview, unsigned int program);
}