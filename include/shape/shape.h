#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "camera/ray.h"
#include "accelerate/bound.h"
#include <optional>


struct ShapeSample {
    glm::vec3 point;
    glm::vec3 normal;
    float pdf;
};

struct Shape {
    virtual std::optional<HitInfo> intersect(const Ray &ray, float t_min, float t_max) const = 0;
    virtual Bounds getBounds() const { return {}; }
    virtual float getArea() const { return  -1; }
    virtual float PDF(const glm::vec3 &point, const glm::vec3 &normal) const { return  1.f / getArea(); }
    virtual std::optional<ShapeSample> sampleShape(const RNG &rng) const { return {}; }
    virtual ~Shape() = default;
};

#endif
