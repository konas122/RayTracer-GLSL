#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "util/rand.h"
#include <glm/glm.hpp>
#include <optional>


struct BSDFSample {
    glm::vec3 bsdf;
    float pdf;
    glm::vec3 light_direction;
};

struct Material {
    // BSDF = BRDF + BTDF
    virtual std::optional<BSDFSample> sampleBSDF(const glm::vec3 &hit_point, const glm::vec3 &view_direction, const RNG &rng) const = 0;
    void setEmissive(const glm::vec3 &emissive) { this->emissive = emissive; }

public:
    glm::vec3 albedo = {1, 1, 1};
    glm::vec3 emissive = {0, 0, 0};

    Material() = default;
    Material(const glm::vec3 &albedo) : albedo(albedo) {}
};

#endif
