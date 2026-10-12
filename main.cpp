#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "gradient.h"


void key_callback(GLFWwindow* window, int key, int scancode,int action ,int mods) {

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        gradient::center = glm::vec2(0);
        gradient::ZOOM = 1.;
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

    glUseProgram(gradient::program);
    glUniform3f(glGetUniformLocation(program, "iResolution"), (float)width, (float)height, 1.0f);
    glUniform1f(glGetUniformLocation(program, "iTime"), time);
    glUniform2f(glGetUniformLocation(program, "mouse"), (float)mouse.x, (float)mouse.y);
    glUniform2f(glGetUniformLocation(program, "viewport"), x, y);
    glUniform1f(glGetUniformLocation(program, "preview"), (preview)?1.:0.);
    glUniform1f(glGetUniformLocation(program, "strength"), gradient::strength);
    glUniform1f(glGetUniformLocation(program, "rotation"), gradient::rotation);
    glUniform1f(glGetUniformLocation(program, "red"), gradient::red);
    glUniform1f(glGetUniformLocation(program, "green"), gradient::green);
    glUniform1f(glGetUniformLocation(program, "blue"),gradient::blue);
    
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
    int texture = load_texture("images/lily.jpg",img_width,img_heigth);
    float img_ratio = img_width / img_heigth;

    setupFullscreenQuad();
    gradient::initPrograms();
   
   // ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 16.0f);
    ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\corbel.ttf", 10.0f);

    glfwSetKeyCallback(window, key_callback);
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
        //drawImageQuad(..., glm::vec2(mousex * scale, mousey * scale), filter_preview, program);

       
        drawImageQuad(main_viewport.x, main_viewport.y, main_viewport.z, main_viewport.w, gradient::app_time, glm::vec2(mousex*scalex, mousey*scaley),filter_preview,gradient::program);
            
       
     
       //drawfilterBox(toolbar.x+100, resolution.y*.05, default_program);
       //drawfilterBox(codepos + codewidth / 3, resolution.y * .3, vignete_program);
       //drawfilterBox(codepos + codewidth / 3, resolution.y * .55, gray_program);
       // drawfilterBox(codepos + codewidth / 3, resolution.y * .78, chromatic_program);



       

        //glViewport(0, 0, iresolution.x, iresolution.y);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
     
        gradient::drawmenu();
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
            if (gradient::program == gradient::vignete_program || gradient::program == gradient::contrast_program || gradient::program==gradient::chromatic_program  || gradient::program == gradient::gausian_program) {
                ImGui::SliderFloat("Strength", &gradient::strength, 0., 5.);
            }
            if (gradient::program == gradient::color_program) {
                ImGui::SliderFloat("Red", &gradient::red, 0., 255.);
                ImGui::SliderFloat("Green", &gradient::green, 0., 255.);
                ImGui::SliderFloat("Blue", &gradient::blue, 0., 255.);
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
        ImGui::Indent(gradient::resolution.x * .35);
        ImGui::SetNextItemWidth(gradient::resolution.x * .1);
        ImGui::SliderFloat("Zoom:", &gradient::ZOOM, .0, 5.0);
        ImGui::SameLine();
        ImGui::Indent(300.);
        ImGui::SetNextItemWidth(gradient::resolution.x * .1);
        ImGui::SliderFloat("Rotation:", &gradient::rotation, .0, gradient::PI);
        ImGui::End();
        


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      
        glfwSwapBuffers(window);

    }
    glfwDestroyWindow(window);

    return 0;
}