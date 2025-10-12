#ifndef ALIAS_TABLE_HPP
#define ALIAS_TABLE_HPP

#include <cstddef>
#include <vector>


class AliasTable {
private:
    struct Item {
        double p;
        union {
            double q;
            size_t alias;
        };
    };

    struct SamepleResult {
        size_t index;
        float prob;
    };

public:
    AliasTable() = default;
    void build(const std::vector<float> &values);
    SamepleResult sample(float u) const;

private:
    std::vector<float> probs;
    std::vector<Item> items;
};

#endif
