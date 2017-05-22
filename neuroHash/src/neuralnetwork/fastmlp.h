#ifndef FASTMLP_H
#define FASTMLP_H

#include "src/global/global.h"

#include <vector>
#include <cstdint>
#include <string>
#include <cstring>
#include <functional>

class FastMLP
{
public:
    FastMLP(std::vector<size_t> layers);
    FastMLP(
        std::vector< std::vector<std::vector<uint32_t> > > _weights,
        std::vector< std::vector<uint32_t> > _barriers
    );
    ~FastMLP();

    std::string calcOut(const std::vector<uint32_t>& input);
    std::string calcOutSingle(uint32_t* input, size_t size) {
        std::vector<uint32_t> v;
        v.resize(size);
        std::memcpy(&v[0], input, sizeof(uint32_t) * size);
        return calcOut(v);
    }

    hashFunc_t getHashFunc();

    std::string serialize() const;

// Those should be private but we want to change the ANN from outside in an easy manner
    uint32_t barrierFunction(uint64_t sum, uint32_t barrier) const;

    std::vector< uint32_t > wlayers_;
    std::vector< uint32_t > blayers_;
    std::vector< uint32_t > weights_;
    std::vector< uint32_t > barriers_;
};

#endif // FASTMLP_H
