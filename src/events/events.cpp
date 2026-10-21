#include "events.h"
namespace gradient {

    namespace events {
        void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

            if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
                center = glm::vec2(0);
                ZOOM = 1.;
                return;
            }

        }
        void handle_press_events(GLFWwindow * window) {

            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                gradient::center.x -= gradient::MOVE_IMAGE_SPEED;
            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                gradient::center.x += gradient::MOVE_IMAGE_SPEED;
            }
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                gradient::center.y += gradient::MOVE_IMAGE_SPEED;
            }
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                gradient::center.y -= gradient::MOVE_IMAGE_SPEED;
            }
        }
    }
}