#ifndef GLOBALSTATISTICS_H
#define GLOBALSTATISTICS_H

#include "src/global.h"

#include <vector>

class GlobalStatistics
{
public:
    GlobalStatistics();

    static uint64_t totalMutations;
    static uint64_t positiveMutations;

    static uint64_t totalCross;
    static uint64_t positiveCross;

    static uint32_t mutRlayer;
    static uint32_t mutRweight;

    static std::vector<uint32_t> primes_;
};

#endif // GLOBALSTATISTICS_H
