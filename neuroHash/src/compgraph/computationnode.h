#ifndef COMPUTATIONNODE_H
#define COMPUTATIONNODE_H

#include "../global/global.h"

#include <vector>

typedef double numeric_t;

class ComputationNode
{
public:
    ComputationNode();

    virtual void init(
            std::vector<int> iparams,
            std::vector<double> dparams,
            std::vector<numeric_t> values
            ) = 0;

    virtual void forward(
            const std::vector<numeric_t> &input,
            std::vector<numeric_t> &output
            ) const = 0;
};

#endif // COMPUTATIONNODE_H
