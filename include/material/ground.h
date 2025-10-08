#ifndef __GROUND_H__
#define __GROUND_H__

#include "material.h"

class GroundMaterial : public Material {
public:
    GroundMaterial(const glm::vec3 &albedo_) : albedo(albedo_) {}
    std::optional<BSDFSample> sampleBSDF(const glm::vec3 &hit_point, const glm::vec3 &view_direction, const RNG &rng) const override;

    glm::vec3 BSDF(const glm::vec3 &hit_point, const glm::vec3 &light_direction, const glm::vec3 &view_direction) const override;
    bool isDeltaDistribution() const override { return false; }

private:
    glm::vec3 albedo {};
};

#endif
