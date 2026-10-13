#include "events.h"
namespace gradient {

    // handle key presses 
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
            center = glm::vec2(0);
            ZOOM = 1.;
            return;
        }

    }
}