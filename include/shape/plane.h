#ifndef __PLANE_H__
#define __PLANE_H__

#include "shape.h"
#include "accelerate/bound.h"


struct Plane : public Shape {
    Plane(const glm::vec3 &point, const glm::vec3 &normal, const float radius)
        : point(point), normal(glm::normalize(normal)), bound(), radius(radius)
    {
        glm::vec3 up = glm::abs(this->normal.y) < 0.99999 ? glm::vec3(0, 1, 0) : glm::vec3(0, 0, 1);
        x_axis = glm::normalize(glm::cross(this->normal, up));
        z_axis = glm::normalize(glm::cross(x_axis, this->normal));

        Bounds bound_local { { -radius, -0.001, -radius }, { radius, 0.001, radius } };
        for (size_t i = 0; i < 8; i ++) {
            glm::vec3 corner = bound_local.getCorner(i);
            bound.expand(corner.x * x_axis + corner.y * normal + corner.z * z_axis);
            bound.expand(point + corner.x * x_axis + corner.y * normal + corner.z * z_axis);
        }
    }

    Bounds getBounds() const override { return bound; }

    std::optional<HitInfo> intersect(const Ray &ray, float t_min, float t_max) const override;

    float getArea() const override;

    virtual std::optional<ShapeSample> sampleShape(const RNG &rng) const override;

    glm::vec3 point;
    glm::vec3 normal, x_axis, z_axis;
    float radius;
    Bounds bound;
};

#endif
