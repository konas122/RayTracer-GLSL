#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "camera/ray.h"
#include "accelerate/bound.h"
#include <optional>


struct Shape {
    virtual std::optional<HitInfo> intersect(const Ray &ray, float t_min, float t_max) const = 0;
    virtual Bounds getBounds() const {
        return {};
    }
    virtual ~Shape() = default;
};

#endif
