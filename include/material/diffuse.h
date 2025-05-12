#ifndef __DIFFUSE_H__
#define __DIFFUSE_H__

#include "material.h"

class DiffuseMaterial : public Material {
public:
    DiffuseMaterial(const glm::vec3 &albedo_) : Material(albedo_) {}
    std::optional<BSDFSample> sampleBSDF(const glm::vec3 &hit_point, const glm::vec3 &view_direction, const RNG &rng) const override;
};

#endif
