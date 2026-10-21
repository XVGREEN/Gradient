#include "image.h"
#include <cassert>
#include <utility>

namespace gradient {
   image::image(glm::ivec2 Size, std::vector<unsigned char> Buffer)
        : size(Size), buffer(std::move(Buffer))
    {
        assert(buffer.size() == size_t(size.x) * size.y * 4);
    }
}