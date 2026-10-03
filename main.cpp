
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <charconv>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


double app_time = 0.;
static std::string current_word;
glm::vec2 resolution;
glm::ivec2 iresolution;
#define CODE_BAR_RATIO .4
#define TOOL_BAR_RATIO .3
#define TIME_SPEED 1.0/60.0


unsigned int load_texture(const char* path) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
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

    stbi_image_free(buffer);

    return texture;
}






// --- Placeholder fullscreen-quad shader (replace with the real one tomorrow) ---
static const char* placeholder_vertex_src = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

static const char* placeholder_fragment_src = R"(

#version 330 core
out vec4 FragColor;

uniform vec3 iResolution;
uniform float iTime;
uniform vec2 mouse;
uniform sampler2D tex;

#define T iTime


void mainImage(out vec4 o, in vec2 u) {
    vec2 uv = (u*2.-iResolution.xy)/iResolution.x;
    u = u/iResolution.x;
    u.y=1.-u.y;
     vec3 col = texture(tex,u).xyz;
      col*=(1.-length(uv));
    o = vec4(col,1.);
}

void main() {
    mainImage(FragColor, gl_FragCoord.xy);
}
)";

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


GLuint quadVAO = 0, quadVBO = 0;
GLuint quadShaderProgram = 0;

void setupFullscreenQuad() {
    float quadVertices[] = {

        -1.0f, -1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f,

        -1.0f, -1.0f,
         1.0f,  1.0f,
        -1.0f,  1.0f,
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    quadShaderProgram = createShaderProgram(placeholder_vertex_src, placeholder_fragment_src);
}


void drawFullscreenQuad(int x, int y, int width, int height, float time, glm::vec2 mouse) {
    glViewport(x, y, width, height);

    glUseProgram(quadShaderProgram);
    glUniform3f(glGetUniformLocation(quadShaderProgram, "iResolution"), (float)width, (float)height, 1.0f);
    glUniform1f(glGetUniformLocation(quadShaderProgram, "iTime"), time);
    glUniform2f(glGetUniformLocation(quadShaderProgram, "mouse"), (float)mouse.x, (float)mouse.y);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

int main()
{
    glfwInit();
    bool paused = false;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1560, 720, "Gradient", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewInit();

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    float bgColor[3] = { 0.2f, 0.2f, 0.2f };

    ImGui::GetIO().FontGlobalScale = 2.;
    float last_newline = 0;
    float last_backspace = 0;
   
    int texture = load_texture("images/femto.jpg");

    setupFullscreenQuad();
    
    while (!glfwWindowShouldClose(window))
    {
        if (!paused)app_time += TIME_SPEED;
        glfwPollEvents();
        float time = glfwGetTime();
        glfwGetFramebufferSize(window, &iresolution.x, &iresolution.y);
        resolution = { float(iresolution.x),float(iresolution.y) };
        double  mousex = 0, mousey = 0;

        glfwGetCursorPos(window, &mousex, &mousey);


        float codewidth = CODE_BAR_RATIO * resolution.x;
        float codepos = resolution.x - CODE_BAR_RATIO * resolution.x;


        float toolbarwidth = codepos;
        float toolbarpos = resolution.y - TOOL_BAR_RATIO * resolution.y;
        float toolbarheight = resolution.y - toolbarpos;


        int quadX = 0;
        int quadY = (int)toolbarheight;
        int quadWidth = (int)codepos;
        int quadHeight = (int)toolbarpos;

        glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        if (quadWidth > 0 && quadHeight > 0) {
            drawFullscreenQuad(quadX, quadY, quadWidth, quadHeight, app_time, glm::vec2(mousex, mousey));
        }


        glViewport(0, 0, iresolution.x, iresolution.y);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(codepos, 0));
        ImGui::SetNextWindowSize(ImVec2(codewidth, resolution.y));
        ImGui::Begin("Code");
        // drawtext(shader, time);
        ImGui::SameLine();
        ImGui::End();


        ImGui::SetNextWindowPos(ImVec2(0, toolbarpos));
        ImGui::SetNextWindowSize(ImVec2(toolbarwidth, toolbarheight));


        ImGui::Begin("Tools");

        if (ImGui::Button("Pause"))paused = !paused;
        if (ImGui::Button("Restart")) app_time = 0.0;
        ImGui::End();





        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

    }

    return 0;
}