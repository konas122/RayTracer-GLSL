#ifndef __RGB_H__
#define __RGB_H__

#include <glm/glm.hpp>


struct RGB {
    RGB(int r, int g, int b) : r(r), g(g), b(b) {}

    RGB(const glm::vec3 &color) {
        r = glm::clamp<int>(static_cast<int>(glm::pow(color.x, 1.0 / 2.2) * 255), 0, 255);
        g = glm::clamp<int>(static_cast<int>(glm::pow(color.y, 1.0 / 2.2) * 255), 0, 255);
        b = glm::clamp<int>(static_cast<int>(glm::pow(color.z, 1.0 / 2.2) * 255), 0, 255);
    }

    operator glm::vec3() const  {
        return glm::vec3 {
            glm::pow(r / 255.f, 2.2),
            glm::pow(g / 255.f, 2.2),
            glm::pow(b / 255.f, 2.2),
        };
    }

    int r, g, b;
};

#endif
