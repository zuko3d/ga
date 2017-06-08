#ifndef COMPUTATIONALGRAPH_H
#define COMPUTATIONALGRAPH_H

#include "computationnode.h"

class ComputationalGraph
{
public:
    ComputationalGraph();

    void compute(const std::vector<numeric_t>& input, std::vector<numeric_t> &output);
};

#endif // COMPUTATIONALGRAPH_H
