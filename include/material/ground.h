#ifndef __GROUND_H__
#define __GROUND_H__

#include "material.h"

class GroundMaterial : public Material {
public:
    GroundMaterial(const glm::vec3 &albedo_) : Material(albedo_) {}
    std::optional<BSDFSample> sampleBSDF(const glm::vec3 &hit_point, const glm::vec3 &view_direction, const RNG &rng) const override;
};

#endif
