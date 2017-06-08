#ifndef SUMMATOR_H
#define SUMMATOR_H

#include "computationnode.h"

class Summator
{
public:
    Summator();

    void forward(const std::vector<numeric_t> &input, std::vector<numeric_t> &output) const;

    void calcGrad(const std::vector<numeric_t> &input, const std::vector<numeric_t> &output);

    const std::vector<numeric_t> &grad() const;

protected:
    std::vector<numeric_t> grad_;
};

#endif // SUMMATOR_H
