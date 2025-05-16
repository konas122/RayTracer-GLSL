#ifndef __CONDUCTOR_H__
#define __CONDUCTOR_H__

#include "material.h"
#include "microfacet.h"

class ConductorMaterial : public Material {
public:
    ConductorMaterial(const glm::vec3 &ior, const glm::vec3 &k, float alpha_x = 0, float alpha_z = 0)
        : ior(ior), k(k), microfacet(alpha_x, alpha_z) {}
    std::optional<BSDFSample> sampleBSDF(const glm::vec3 &hit_point, const glm::vec3 &view_direction, const RNG &rng) const override;

private:
    glm::vec3 ior, k;
    Microfacet microfacet;
};

#endif
