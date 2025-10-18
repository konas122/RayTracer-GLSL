#ifndef __INFINITE_LIGHT_H__
#define __INFINITE_LIGHT_H__

#include "light.hpp"

class InfiniteLight : public Light {
public:
    InfiniteLight(const glm::vec3 &Le) : Le(Le) {}
    bool impossible() const override { return true; }

    float Phi(float scene_radius) const override;
    std::optional<LightSample> sampleLight(const glm::vec3 &surface_point, float scene_radius, const RNG &rng, bool allow_mis_compensation) const override;
    glm::vec3 getRadiance(const glm::vec3 &surface_point, const glm::vec3 &light_point, const glm::vec3 &normal) const override;
    float getPDF(const glm::vec3 &surface_point, const glm::vec3 &light_point, const glm::vec3 &normal, bool allow_mis_compensation) const override;

private:
    glm::vec3 Le;
};


#endif
