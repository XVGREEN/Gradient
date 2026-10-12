#include <iostream>
#include "utils.h"

namespace gradient {
	
    GLuint compileShader(GLenum type, const char* src) {
        GLuint id = glCreateShader(type);
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        GLint success;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            char log[512];
            glGetShaderInfoLog(id, 512, nullptr, log);
            std::cerr << "Shader compile error: " << log << std::endl;
        }
        return id;
    }


    GLuint createShaderProgram(const char* vertexSrc, const char* fragmentSrc) {
        GLuint vs = compileShader(GL_VERTEX_SHADER, vertexSrc);
        GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);

        GLuint program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);

        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            char log[512];
            glGetProgramInfoLog(program, 512, nullptr, log);
            std::cerr << "Shader link error: " << log << std::endl;
        }

        glDeleteShader(vs);
        glDeleteShader(fs);
        return program;
    }







    //void drawfilterBox(int x, int y, unsigned int program) {
    //    float width = FILTERBOX_RATIOX * resolution.x;
    //    float height =  FILTERBOX_RATIOY * resolution.y;
    //    glViewport(x, y, width, height);
    //    glUseProgram(program);
    //    glUniform3f(glGetUniformLocation(program, "iResolution"), width, height, 1.0f);
    //    glUniform1f(glGetUniformLocation(program, "iTime"), 1.);
    //    glUniform2f(glGetUniformLocation(program, "viewport"), x, y);
    //    glBindVertexArray(quadVAO);
    //    glDrawArrays(GL_TRIANGLES, 0, 6);
    //    glBindVertexArray(0);
    //}



}