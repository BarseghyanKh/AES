#ifndef ATTACS_H
#define ATTACS_H

#include "headers.h"
#include <unordered_map>
#include "byte.h"
#include <fstream>
#include <sstream>

using namespace my_cryptography_lib;
struct VectorHash {
    std::size_t operator()(const std::vector<int>& v) const {
        std::size_t seed = 0;
        for (const int& value : v) {
            seed ^= std::hash<int>()(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};
struct VectorEqual {
    bool operator()(const std::vector<int>& a, const std::vector<int>& b) const {
        return a == b;
    }
};

using Umap = std::unordered_map<std::vector<int>, byte, VectorHash, VectorEqual>;

struct attack {
    std::vector<int> superposition(const std::vector<int>& f, const std::vector<int>& g);
    Umap get_basis(std::vector<std::vector<int>> S);
    std::vector<std::vector<int>> get_permutations();
    void run();
};




#endif // ATTACS_H
