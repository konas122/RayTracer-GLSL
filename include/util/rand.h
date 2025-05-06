#ifndef __RAND_H__
#define __RAND_H__

#include <random>

class RNG {
public:
    RNG() {}
    float uniform() {
        return uniform_distribution(gen);
    }

private:
    std::mt19937 gen = std::mt19937{};
    std::uniform_real_distribution<float> uniform_distribution{0, 1};
};

#endif
