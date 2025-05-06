#ifndef __MODEL_H__
#define __MODEL_H__

#include "triangle.h"
#include "accelerate/bound.h"

#include <filesystem>


class Model : public Shape {
public:
    Model(const std::vector<Triangle> &triangles) : triangles(triangles) {}

    Model(const std::filesystem::path &filename);

    std::optional<HitInfo> intersect(const Ray &ray, float t_min, float t_max) const override;

private:
    Bounds bounds{};
    std::vector<Triangle> triangles;

    void build();
};

#endif
