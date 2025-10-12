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
        if (light->getType() != LightType::EInfiniteLight) {
            lights_mis_compensation.emplace_back(light);
        }
    }

    void build(float scene_radius);

    std::optional<LightSourceSample> sample(float u, bool allow_mis_compensation) const;

    float getProb(const Light *light, bool allow_mis_compensation) const {
        if (allow_mis_compensation) {
            if (light->getType() == LightType::EInfiniteLight) {
                return 0;
            }
            return light2prob_mis_compensation.at(light);
        }
        return light2prob.at(light);
    }

private:
    std::vector<const Light *> lights;
    std::unordered_map<const Light *, float> light2prob;
    AliasTable alias_table;

    std::vector<const Light *> lights_mis_compensation;
    AliasTable alias_table_mis_compensation;
    std::unordered_map<const Light *, float> light2prob_mis_compensation;
};

#endif
