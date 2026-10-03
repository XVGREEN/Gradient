
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "shaders/shaders.h"
#include "utils/utils.h"
std::vector<int>programs;
unsigned int program;

constexpr  float FILTERBOX_RATIOX = .1;
constexpr  float FILTERBOX_RATIOY = .2;
int current_filter = 0;

double app_time = 0.;
static std::string current_word;
glm::vec2 resolution;
glm::ivec2 iresolution;
#define CODE_BAR_RATIO .2
#define TOOL_BAR_RATIO .3
#define TIME_SPEED 1.0/60.0
void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS) return;

    double cx, cy;
    glfwGetCursorPos(window, &cx, &cy);
    int winW, winH;
    glfwGetWindowSize(window, &winW, &winH);



    float mx = cx * (resolution.x / winW);
    float my = resolution.y - cy * (resolution.y / winH);
 
    float codewidth = CODE_BAR_RATIO * resolution.x;
    float codepos = resolution.x - codewidth;


    int boxX = (int)(codepos + codewidth / 3);
    int boxW = (int)(FILTERBOX_RATIOX * resolution.x);
    int boxH = (int)(FILTERBOX_RATIOY * resolution.y);
    const float yRatios[4] = { .05f, .3f, .55f, .78f };

    for (int i = 0; i < 4; i++) {
        int boxY = (int)(resolution.y * yRatios[i]);
        if (mx >= boxX && mx < boxX + boxW && my >= boxY && my < boxY + boxH) {
            program = programs[i];
            return;
        }
    }
}

unsigned int load_texture(const char* path,float &w,float& h) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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


}


void drawFullscreenQuad(int x, int y, int width, int height, float time, glm::vec2 mouse,unsigned int program) {
    glViewport(x, y, width, height);

    glUseProgram(program);
    glUniform3f(glGetUniformLocation(program, "iResolution"), (float)width, (float)height, 1.0f);
    glUniform1f(glGetUniformLocation(program, "iTime"), time);
    glUniform2f(glGetUniformLocation(program, "mouse"), (float)mouse.x, (float)mouse.y);
    glUniform2f(glGetUniformLocation(program, "viewport"), x, y);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
void drawfilterBox(int x, int y,unsigned int program ) {
    float width= FILTERBOX_RATIOX * resolution.x;
    float height = FILTERBOX_RATIOY * resolution.y;
    glViewport(x, y, width,height);
    glUseProgram(program);
    glUniform3f(glGetUniformLocation(program, "iResolution"), width, height, 1.0f);
    glUniform1f(glGetUniformLocation(program, "iTime"), 1.);
    glUniform2f(glGetUniformLocation(program, "viewport"), x, y);
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

    GLFWwindow* window = glfwCreateWindow(1000, 720, "Gradient:femto.jpg", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewInit();

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    float bgColor[3] = { 0.2f, 0.2f, 0.2f };

    ImGui::GetIO().FontGlobalScale = 2.;
    float last_newline = 0;
    float last_backspace = 0;
    float img_heigth;
    float img_width;
    int texture = load_texture("images/femto.jpg",img_width,img_heigth);
    float img_ratio = img_width / img_heigth;

    setupFullscreenQuad();
    unsigned int default_program=createShaderProgram(default_vertex_src, default_fragment_src);
    unsigned int vignete_program = createShaderProgram(default_vertex_src, vignete);
    unsigned int gray_program = createShaderProgram(default_vertex_src, grayscale);
    unsigned int chromatic_program = createShaderProgram(default_vertex_src, chromatic);
    program = default_program;
    programs.push_back(default_program);
    programs.push_back(vignete_program);
    programs.push_back(gray_program);
    programs.push_back(chromatic_program);
    glfwSetMouseButtonCallback(window, mouse_callback);
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

        //quadWidth /= img_width*.1;
        //quadHeight /= img_heigth*.1;
        glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


       // if (quadWidth > 0 && quadHeight > 0) {
            drawFullscreenQuad(quadX, quadY, quadWidth, quadHeight, app_time, glm::vec2(mousex, mousey),program);
            
       
        //} 
        drawfilterBox(codepos+codewidth/3, resolution.y*.05, default_program);
        drawfilterBox(codepos + codewidth / 3, resolution.y * .3, vignete_program);
        drawfilterBox(codepos + codewidth / 3, resolution.y * .55, gray_program);
        drawfilterBox(codepos + codewidth / 3, resolution.y * .78, chromatic_program);



       

        //glViewport(0, 0, iresolution.x, iresolution.y);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(codepos, 0));
        ImGui::SetNextWindowSize(ImVec2(codewidth, resolution.y));
        ImGui::Begin("Code");

        ImGui::SameLine();
        ImGui::End();


        ImGui::SetNextWindowPos(ImVec2(0, toolbarpos));
        ImGui::SetNextWindowSize(ImVec2(toolbarwidth, toolbarheight));


        ImGui::Begin("Tools");
        ImGui::End();





        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);

    }

    return 0;
}