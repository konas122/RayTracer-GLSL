#ifndef __RAND_H__
#define __RAND_H__

#include <random>

class RNG {
public:
    RNG() {}
    float uniform() const {
        return uniform_distribution(gen);
    }

private:
    mutable std::mt19937 gen = std::mt19937{};
    mutable std::uniform_real_distribution<float> uniform_distribution{0, 1};
};

#endif
