
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
#define WINDOWS_APHA .7
std::vector<int>programs;
unsigned int program;
glm::vec2 center(0);
float strength = 0;
float red = 0;
float green = 0;
float blue = 0;

constexpr  float FILTERBOX_RATIOX = .1;
constexpr  float FILTERBOX_RATIOY = .2;
constexpr float CANVAS_START = 25;
constexpr float MOVE_IMAGE_SPEED = 1;
int current_filter = 0;
float ZOOM=1.;
double app_time = 0.;
static std::string current_word;
glm::vec2 resolution;
glm::ivec2 iresolution;
#define LAYER_BAR_RATIO .17
#define TOOL_BAR_RATIO .1
#define TIME_SPEED 1.0/60.0
#define ZOOM_SPEED .01;
//void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
//    if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS) return;
//
//    double cx, cy;
//    glfwGetCursorPos(window, &cx, &cy);
//    int winW, winH;
//    glfwGetWindowSize(window, &winW, &winH);
//
//
//
//    float mx = cx * (resolution.x / winW);
//    float my = resolution.y - cy * (resolution.y / winH);
// 
//    float codewidth = CODE_BAR_RATIO * resolution.x;
//    float codepos = resolution.x - codewidth;
//
//
//    int boxX = (int)(codepos + codewidth / 3);
//    int boxW = (int)(FILTERBOX_RATIOX * resolution.x);
//    int boxH = (int)(FILTERBOX_RATIOY * resolution.y);
//    const float yRatios[4] = { .05f, .3f, .55f, .78f };
//
//    for (int i = 0; i < 4; i++) {
//        int boxY = (int)(resolution.y * yRatios[i]);
//        if (mx >= boxX && mx < boxX + boxW && my >= boxY && my < boxY + boxH) {
//            program = programs[i];
//            return;
//        }
//    }
//}


void key_callback(GLFWwindow* window, int key, int scancode,int action ,int mods) {

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        center = glm::vec2(0);
        ZOOM = 0;
        return ;
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


void drawImageQuad(int x, int y, int width, int height, float time, glm::vec2 mouse,bool preview,unsigned int program) {
    glViewport(x, y, width, height);

    glUseProgram(program);
    glUniform3f(glGetUniformLocation(program, "iResolution"), (float)width, (float)height, 1.0f);
    glUniform1f(glGetUniformLocation(program, "iTime"), time);
    glUniform2f(glGetUniformLocation(program, "mouse"), (float)mouse.x, (float)mouse.y);
    glUniform2f(glGetUniformLocation(program, "viewport"), x, y);
    glUniform1f(glGetUniformLocation(program, "preview"), (preview)?1.:0.);
    glUniform1f(glGetUniformLocation(program, "strength"), strength);

    glUniform1f(glGetUniformLocation(program, "red"), red);
    glUniform1f(glGetUniformLocation(program, "green"), green);
    glUniform1f(glGetUniformLocation(program, "blue"), blue);
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
    bool filter_preview=false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1000, 720, "Gradient:femto.jpg", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewInit();

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    float bgColor[3] = { 0.1f, 0.1f, 0.1f };

    ImGui::GetIO().FontGlobalScale = 1.5;
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
    unsigned int contrast_program = createShaderProgram(default_vertex_src, contrast); 
    unsigned int radial_program = createShaderProgram(default_vertex_src, radial_chromatic);
    unsigned int gausian_program = createShaderProgram(default_vertex_src, gaussian_blur);
    unsigned int matrix_program = createShaderProgram(default_vertex_src, matrix);
    unsigned int color_program = createShaderProgram(default_vertex_src, color_adjust);
    program = default_program;
    programs.push_back(default_program);
    programs.push_back(vignete_program);
    programs.push_back(gray_program);
    programs.push_back(chromatic_program);
    
    glfwSetKeyCallback(window, key_callback);
    while (!glfwWindowShouldClose(window))
    {
        using vec2 = glm::vec2;
        using vec4= glm::vec4;
        using vec3 = glm::vec3;
        
        if (!paused)app_time += TIME_SPEED;
        glfwPollEvents();
        float time = glfwGetTime();
        glfwGetFramebufferSize(window, &iresolution.x, &iresolution.y);
        resolution = { float(iresolution.x),float(iresolution.y) };
        double  mousex = 0, mousey = 0;

        glfwGetCursorPos(window, &mousex, &mousey);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            center.x -= MOVE_IMAGE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            center.x += MOVE_IMAGE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            center.y+= MOVE_IMAGE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            center.y -= MOVE_IMAGE_SPEED;
        }

        //control keys
        bool ctrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;

        if (ctrl && glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
            ZOOM += ZOOM_SPEED;
        }
        if (ctrl && glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
            ZOOM -= ZOOM_SPEED;
        }
        


       
        vec4 toolbar;
         toolbar.x = 0;
         toolbar.y = CANVAS_START;
         toolbar.z = resolution.x * TOOL_BAR_RATIO;
         toolbar.w = resolution.y - CANVAS_START * 3; //TODO:change this to no use constants;


        vec4 options_bar;
        options_bar.x = resolution.x - resolution.x * LAYER_BAR_RATIO;
        options_bar.y = CANVAS_START;
        options_bar.z = resolution.x * LAYER_BAR_RATIO;
        options_bar.w = toolbar.w;
        

        vec4 canvas;
        canvas.x =  toolbar.z;
        canvas.y = toolbar.y;
        canvas.z = options_bar.x - (toolbar.x + toolbar.z);
        canvas.w = options_bar.w;

       


        vec4 main_viewport=vec4();
        main_viewport.x = (resolution.x)/2. - ((img_width*ZOOM)/2.);
        main_viewport.y = (resolution.y)/2. - ((img_width*ZOOM)/2.);
        main_viewport.z = img_width*ZOOM;
        main_viewport.w = img_heigth * ZOOM;
        main_viewport.x += center.x;
        main_viewport.y += center.y;


        vec4 bottom = vec4(0);
        bottom.x = 0;
        bottom.y = options_bar.w;
        bottom.z = resolution.x;
        bottom.w = resolution.y - options_bar.w;
        
        

        glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        int winW, winH;
        glfwGetWindowSize(window, &winW, &winH);
        float scalex = resolution.x / winW;
        float scaley = resolution.y / winH;
        //drawImageQuad(..., glm::vec2(mousex * scale, mousey * scale), filter_preview, program);

       
        drawImageQuad(main_viewport.x, main_viewport.y, main_viewport.z, main_viewport.w, app_time, glm::vec2(mousex*scalex, mousey*scaley),filter_preview,program);
            
       
     
       //drawfilterBox(toolbar.x+100, resolution.y*.05, default_program);
       //drawfilterBox(codepos + codewidth / 3, resolution.y * .3, vignete_program);
       //drawfilterBox(codepos + codewidth / 3, resolution.y * .55, gray_program);
       // drawfilterBox(codepos + codewidth / 3, resolution.y * .78, chromatic_program);



       

        //glViewport(0, 0, iresolution.x, iresolution.y);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New","Ctrl+N")) {
                }
                if (ImGui::MenuItem("Open", "Ctrl+O")) {
                    printf("baka");
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                }
                if (ImGui::MenuItem("Save as..")) {
                }
                if (ImGui::MenuItem("Exit")) {
                }
                
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Undo","Ctrl+Z")) {
                }
                if (ImGui::MenuItem("Redo", "Ctrl + Y")) {
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View")) {
                if (ImGui::MenuItem("Filter Previews")) {
                }
                if (ImGui::MenuItem("Grid")) {
                }
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Filters")) {
                if (ImGui::MenuItem("Vignete")) {
                    program = vignete_program;
                }
                if (ImGui::MenuItem("adjust color")) {
                    program = color_program;
                    strength = 0;
                }
                
                if (ImGui::MenuItem("contrast")) {
                    program = contrast_program;
                    strength = 0;
                }
                if (ImGui::MenuItem("Chormatic")) {
                    program = chromatic_program;
                    strength = 0;
                }
                if (ImGui::MenuItem("Radial chormatic")) {
                    program = radial_program;
                    strength = 0;
                }
                if (ImGui::MenuItem("grayscale")) {
                    program = gray_program;
                    strength = 0;
                }
                if (ImGui::MenuItem("gausin")) {
                    program = gausian_program;
                    strength = 0;
                }
                if (ImGui::MenuItem("matrix")) {
                    program = matrix_program;
                    strength = 0;
                }
                ImGui::EndMenu();
                
            }
            
            if (ImGui::BeginMenu("Settings")) {
                if (ImGui::MenuItem("Filter Preview Mode")) {
                    filter_preview = !filter_preview;
                }
                ImGui::EndMenu();

            }

            if (ImGui::BeginMenu("Help")) {
                if (ImGui::MenuItem("Help")) {
                }
                if (ImGui::MenuItem("About")) {
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        //tools window
        ImGui::SetNextWindowBgAlpha(WINDOWS_APHA);
        ImGui::SetNextWindowPos(ImVec2(toolbar.x, toolbar.y));
        ImGui::SetNextWindowSize(ImVec2(toolbar.z, toolbar.w));
        ImGui::Begin("Tools");
        
        ImGui::SameLine();
        ImGui::End();

        //options window
        ImGui::SetNextWindowBgAlpha(WINDOWS_APHA);
        ImGui::SetNextWindowPos(ImVec2(options_bar.x,options_bar.y));
        ImGui::SetNextWindowSize(ImVec2(options_bar.z, options_bar.w));
        ImGui::Begin("Options");
            if (program == vignete_program || program == contrast_program || program==chromatic_program  || program == gausian_program) {
                ImGui::SliderFloat("Strength", &strength, 0., 5.);
            }
            if (program == color_program) {
                ImGui::SliderFloat("Red", &red, 0., 255.);
                ImGui::SliderFloat("Green", &green, 0., 255.);
                ImGui::SliderFloat("Blue", &blue, 0., 255.);
            }

        ImGui::End();

        //info bar 
        ImGui::SetNextWindowBgAlpha(WINDOWS_APHA);
        ImGui::SetNextWindowPos(ImVec2(bottom.x, bottom.y));
        ImGui::SetNextWindowSize(ImVec2(bottom.z,bottom.w));
        ImGui::Begin("Info");
        ImGui::Text("Tool:Pen  |  X:%d Y:%d", (int)mousex- (int)main_viewport.x, (int)mousey-(int)main_viewport.y);
        ImGui::SameLine();
        ImGui::Text((std::string("     Preview Mode:") + std::string((filter_preview) ? "yes" : "no")).c_str());
        ImGui::SameLine();
        ImGui::Indent(resolution.x * .6);
        ImGui::SetNextItemWidth(resolution.x * .3);
        ImGui::SliderFloat("Zoom:", &ZOOM, .0, 5.0);
        ImGui::End();
        


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      
        glfwSwapBuffers(window);

    }

    return 0;
}