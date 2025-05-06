#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <glm/glm.hpp>

struct Material {
    glm::vec3 albedo = {1, 1, 1};
    bool is_specular = false;
    glm::vec3 emissive = {0, 0, 0};
};

#endif
