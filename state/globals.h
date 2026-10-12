#pragma once
#include <glm/glm.hpp>
#include <string>

namespace gradient {
    #define WINDOWS_APHA 0.7f 
    const float PI = 3.1415f;
    constexpr float FILTERBOX_RATIOX = 0.1f;
    constexpr float FILTERBOX_RATIOY = 0.2f;
    constexpr float CANVAS_START = 25.0f;
    constexpr float MOVE_IMAGE_SPEED = 1.0f;

    #define LAYER_BAR_RATIO 0.1f
    #define TOOL_BAR_RATIO 0.1f
    #define TIME_SPEED (1.0f / 60.0f)
    #define ZOOM_SPEED 0.01f 

    inline unsigned int program;
    inline float strength;
    inline float filter_preview;
    inline glm::vec2 center(0.0f);
    inline float red = 0.0f;
    inline float green = 0.0f;
    inline float blue = 0.0f;
    inline glm::vec2 resolution;
    inline glm::ivec2 iresolution;
    inline int current_filter = 0;
    inline float ZOOM = 1.0f;
    inline float rotation = 0.0f;
    inline double app_time = 0.0;
    inline std::string current_word;
}