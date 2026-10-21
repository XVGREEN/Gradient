#include <iostream>
#include "core.h"


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


    void setupFullscreenQuad() {
        float quadVertices[] = {
            
                -1.0f, -1.0f, 0.f, 0.f,   
                 1.0f, -1.0f, 1.f, 0.f,   
                 1.0f,  1.0f, 1.f, 1.f,   

                -1.0f, -1.0f, 0.f, 0.f,  
                 1.0f,  1.0f, 1.f, 1.f,   
                -1.0f,  1.0f, 0.f, 1.f    
        };

        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);

        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);


        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeof(float)*2));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);


    }






    unsigned int load_texture(const char* path, float& w, float& h) {
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, channels;
        unsigned char* buffer;

        buffer = stbi_load(path, &width, &height, &channels, 0);
        if (!buffer) {
            printf("image not loaded");
            return 0;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
        glGenerateMipmap(GL_TEXTURE_2D);
        w = width;
        h = height;
        stbi_image_free(buffer);

        return texture;
    }


    void drawImageQuad(int x, int y, int width, int height, float time, glm::vec2 mouse, bool preview, unsigned int program) {
        glViewport(x, y, width, height);

        glUseProgram(gradient::program);
        glUniform3f(glGetUniformLocation(program, "iResolution"), (float)width, (float)height, 1.0f);
        glUniform1f(glGetUniformLocation(program, "iTime"), 0.0);
        glUniform2f(glGetUniformLocation(program, "mouse"), (float)mouse.x, (float)mouse.y);
        glUniform2f(glGetUniformLocation(program, "viewport"), x, y);
        glUniform1f(glGetUniformLocation(program, "preview"), (preview) ? 1. : 0.);
        glUniform1f(glGetUniformLocation(program, "strength"), strength);
        glUniform1f(glGetUniformLocation(program, "rotation"), rotation);
        glUniform1f(glGetUniformLocation(program, "red"),red);
        glUniform1f(glGetUniformLocation(program, "green"), green);
        glUniform1f(glGetUniformLocation(program, "blue"),blue);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    
}