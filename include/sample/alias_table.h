#ifndef ALIAS_TABLE_HPP
#define ALIAS_TABLE_HPP

#include <vector>


class AliasTable {
private:
    struct Item {
        float p;
        union {
            float q;
            int alias;
        };
    };

    struct SamepleResult {
        int index;
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
