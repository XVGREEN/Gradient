#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "gradient.h"

int main()
{
    glfwInit();
    bool paused = false;
 

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1368, 768, "Gradient:femto.jpg", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewInit();

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    float bgColor[3] = { 0.1f, 0.1f, 0.1f };

    ImGui::GetIO().FontGlobalScale = 2.5;
    float last_newline = 0;
    float last_backspace = 0;
    float img_heigth;
    float img_width;
    int texture = gradient::load_texture("images/lily.jpg",img_width,img_heigth);
    float img_ratio = img_width / img_heigth;

    gradient::setupFullscreenQuad();
    gradient::initPrograms();
   
   // ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 16.0f);
    ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\corbel.ttf", 10.0f);

    glfwSetKeyCallback(window, gradient::key_callback);
    while (!glfwWindowShouldClose(window))
    {
        using vec2 = glm::vec2;
        using vec4= glm::vec4;
        using vec3 = glm::vec3;
        
        if (!paused)gradient::app_time += TIME_SPEED;
        glfwPollEvents();
        float time = glfwGetTime();
        glfwGetFramebufferSize(window, &gradient::iresolution.x, &gradient::iresolution.y);
        gradient::resolution = { float(gradient::iresolution.x),float(gradient::iresolution.y) };
        double  mousex = 0, mousey = 0;

        glfwGetCursorPos(window, &mousex, &mousey);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            gradient::center.x -= gradient::MOVE_IMAGE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            gradient::center.x += gradient::MOVE_IMAGE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            gradient::center.y+= gradient::MOVE_IMAGE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            gradient::center.y -= gradient::MOVE_IMAGE_SPEED;
        }

        //control keys
        bool ctrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;

        if (ctrl && glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
            gradient::ZOOM += .2*ZOOM_SPEED;
        }
        if (ctrl && glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
            gradient::ZOOM -= .2*ZOOM_SPEED;
        }
        gradient::ZOOM=glm::clamp(gradient::ZOOM, 0.0f, 5.f);
        


       
        vec4 toolbar;
         toolbar.x = 0;
         toolbar.y = gradient::CANVAS_START;
         toolbar.z = gradient::resolution.x * TOOL_BAR_RATIO;
         toolbar.w = gradient::resolution.y - gradient::CANVAS_START * 3; //TODO:change this to no use constants;


        vec4 options_bar;
        options_bar.x = gradient::resolution.x - gradient::resolution.x * LAYER_BAR_RATIO;
        options_bar.y = gradient::CANVAS_START;
        options_bar.z = gradient::resolution.x * LAYER_BAR_RATIO;
        options_bar.w = toolbar.w;
        

        vec4 canvas;
        canvas.x =  toolbar.z;
        canvas.y = toolbar.y;
        canvas.z = options_bar.x - (toolbar.x + toolbar.z);
        canvas.w = options_bar.w;

       


        vec4 main_viewport=vec4();
        main_viewport.x = (gradient::resolution.x)/2. - ((img_width* gradient::ZOOM)/2.);
        main_viewport.y = (gradient::resolution.y)/2. - ((img_heigth* gradient::ZOOM)/2.);
        main_viewport.z = img_width* gradient::ZOOM;
        main_viewport.w = img_heigth * gradient::ZOOM;
        main_viewport.x += gradient::center.x;
        main_viewport.y += gradient::center.y;


        vec4 bottom = vec4(0);
        bottom.x = 0;
        bottom.y = options_bar.w;
        bottom.z = gradient::resolution.x;
        bottom.w = gradient::resolution.y - options_bar.w;
        
        

        glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        int winW, winH;
        glfwGetWindowSize(window, &winW, &winH);
        float scalex = gradient::resolution.x / winW;
        float scaley = gradient::resolution.y / winH;
        gradient::drawImageQuad(main_viewport.x, main_viewport.y, main_viewport.z, main_viewport.w, gradient::app_time, glm::vec2(mousex*scalex, mousey*scaley),gradient::filter_preview,gradient::program);
        gradient::drawui(toolbar,options_bar,bottom,main_viewport,glm::vec2(mousex,mousey));
      
        glfwSwapBuffers(window);

    }
    glfwDestroyWindow(window);

    return 0;
}