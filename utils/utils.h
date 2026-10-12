#pragma once
#include <GL/glew.h> 
namespace gradient {

    GLuint compileShader(GLenum type, const char* src);
    GLuint createShaderProgram(const char* vertexSrc, const char* fragmentSrc);
   
   
}