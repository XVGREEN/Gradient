#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace gradient {
    struct image {
        glm::ivec2 size;
        std::vector<unsigned char> buffer;

        image(glm::ivec2 Size, std::vector<unsigned char> Buffer);
    };
}