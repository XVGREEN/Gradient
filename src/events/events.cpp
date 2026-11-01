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
        static void mouseButtonCallback(GLFWwindow* w, int button, int action, int mods) {
            
            static bool press = false;
            if (button == GLFW_MOUSE_BUTTON_LEFT == !press) {
                if (action == GLFW_PRESS) {
                    double cx, cy;
                    glfwGetCursorPos(w, &cx,&cy);
                    start_pos = glm::vec2(cx, cy);

                }
            }
            if (button == GLFW_MOUSE_BUTTON_LEFT && press) {
                if (action == GLFW_RELEASE) {
                    double cx, cy;
                    glfwGetCursorPos(w, &cx, &cy);
                    end_pos = glm::vec2(cx, cy);

                }
            }
            
        }
    }
}