#ifndef __BVH_H__
#define __BVH_H__

#include "bound.h"
#include "shape/triangle.h"

#include <memory>
#include <variant>


struct BVHTreeNode {
    Bounds bounds{};
    std::vector<Triangle> triangles;
    std::shared_ptr<BVHTreeNode> left, right;

    size_t depth;
    size_t split_axis;

    void updateBounds() {
        bounds = {};
        for (const auto &triangle : triangles) {
            bounds.expand(triangle.p0);
            bounds.expand(triangle.p1);
            bounds.expand(triangle.p2);
        }
    }
};


struct alignas(32) BVHNode {
    Bounds bounds{};
    union {
        int child1_index;
        int triangle_index;
    };

    uint16_t triangle_count;
    uint8_t depth;
    uint8_t split_axis;
};

struct BVHState {
    size_t total_node_count {};
    size_t leaf_node_count {};
    size_t max_leaf_node_triangle_count {};

    void addLeafNode(std::shared_ptr<BVHTreeNode> node) {
        leaf_node_count ++;
        max_leaf_node_triangle_count = glm::max(max_leaf_node_triangle_count, node->triangles.size());
    }
};


class BVH : public Shape {
public:
    void build(std::vector<Triangle> &&triangles);
    std::optional<HitInfo> intersect(const Ray &ray, float t_min, float t_max) const override;

private:
    void recursiveSplit(std::shared_ptr<BVHTreeNode> node, BVHState &state);
    size_t recursiveFlatten(std::shared_ptr<BVHTreeNode> node);

private:
    std::shared_ptr<BVHTreeNode> root;
    std::vector<BVHNode> nodes;
    std::vector<Triangle> ordered_triangles;
};

#endif
