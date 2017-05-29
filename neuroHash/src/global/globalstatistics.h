#ifndef GLOBALSTATISTICS_H
#define GLOBALSTATISTICS_H

#include "src/global/global.h"

#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>

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

    static size_t startingPrimeForWeights;
    static size_t startingPrimeForBarriers;
    static size_t mlpOrder;

    static std::vector<uint32_t> primes_;
    static std::vector<uint32_t> weightValues_;
    static std::vector<uint32_t> barrierValues_;

    static std::vector<vtkSmartPointer<vtkRenderWindow> > windows_;
};

#endif // GLOBALSTATISTICS_H
