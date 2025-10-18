#ifndef __LIGHT_SAMPLER_H__
#define __LIGHT_SAMPLER_H__


#include "light.hpp"
#include "sample/alias_table.h"
#include <optional>
#include <unordered_map>


struct LightSourceSample {
    const Light *light;
    float prob;
};


class LightSampler {
public:
    LightSampler() = default;

    void addLight(const Light *light) {
        lights.emplace_back(light);
    }

    void build(float scene_radius);

    std::optional<LightSourceSample> sample(float u) const;

    float getProb(const Light *light) const {
        auto result = light2prob.find(light);
        if (result == light2prob.end()) {
            return 0;
        }
        return result->second;
    }

private:
    std::vector<const Light *> lights;
    std::unordered_map<const Light *, float> light2prob;
    AliasTable alias_table;
};

#endif
