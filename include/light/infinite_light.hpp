#ifndef __INFINITE_LIGHT_H__
#define __INFINITE_LIGHT_H__

#include "light.hpp"

class InfiniteLight : public Light {
public:
    InfiniteLight(const glm::vec3 &Le) : Light(Le) {}

    float Phi(float scene_radius) const override;
    LightType getType() const override { return LightType::EInfiniteLight; }
    std::optional<LightSample> sampleLight(const glm::vec3 &surface_point, float scene_radius, const RNG &rng, bool allow_mis_compensation) const override;
    float getPDF(const glm::vec3 &surface_point, const glm::vec3 &light_point, const glm::vec3 &normal, bool allow_mis_compensation) const override;
};

#endif
