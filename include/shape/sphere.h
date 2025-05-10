#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "shape.h"

struct Sphere : public Shape{
    glm::vec3 center;
    float radius;

    Sphere (const glm::vec3 &center, float radius) : center(center), radius(radius) {}

    std::optional<HitInfo> intersect(const Ray &ray, float t_min, float t_max) const override;

    Bounds getBounds() const override {
        return {center - radius, center + radius};
    }
};

#endif
