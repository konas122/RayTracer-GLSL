#ifndef __CONDUCTOR_H__
#define __CONDUCTOR_H__

#include "material.h"

class ConductorMaterial : public Material {
public:
    ConductorMaterial(const glm::vec3 &ior, const glm::vec3 &k)
        : ior(ior), k(k) {}
    std::optional<BSDFSample> sampleBSDF(const glm::vec3 &hit_point, const glm::vec3 &view_direction, const RNG &rng) const override;

private:
    glm::vec3 ior, k;
};

#endif
