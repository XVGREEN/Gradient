#pragma once
#include <GLFW/glfw3.h>
#include "../state/globals.h"
namespace gradient {
    namespace events {
        extern void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        extern void handle_press_events(GLFWwindow * window);



        
    }
}