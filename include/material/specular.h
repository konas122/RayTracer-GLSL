#ifndef __SPECULAR_H__
#define __SPECULAR_H__

#include "material.h"

class SpecularMaterial : public Material {
public:
    SpecularMaterial(const glm::vec3 &albedo) : Material(albedo) {}
    std::optional<BSDFSample> sampleBSDF(const glm::vec3 &hit_point, const glm::vec3 &view_direction, const RNG &rng) const override;
};

#endif
