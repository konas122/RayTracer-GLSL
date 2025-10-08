#include "shape/plane.h"
#include "sample/spherical.h"

std::optional<HitInfo> Plane::intersect(const Ray &ray, float t_min, float t_max) const {
    float hit_t = glm::dot(point - ray.origin, normal) / glm::dot(ray.direction, normal);
    const glm::vec3 hit_point = ray.hit(hit_t);
    glm::vec3 hit_point_to_center = hit_point - point;
    if ((hit_t > t_min && hit_t < t_max) && (glm::dot(hit_point_to_center, hit_point_to_center) < radius * radius)) {
        return HitInfo{hit_t, hit_point, normal};
    }
    return {};
}

float Plane::getArea() const {
    return PI * radius * radius;
}

std::optional<ShapeSample> Plane::sampleShape(const RNG &rng) const {
    glm::vec2 sample_local = UniformSampleUnitDisk({ rng.uniform(), rng.uniform() }) * radius;
    glm::vec3 sample_point = point + sample_local.x * x_axis + sample_local.y * z_axis;
    return ShapeSample { sample_point, normal, 1.f / getArea() };
}
