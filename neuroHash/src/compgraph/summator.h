#ifndef SUMMATOR_H
#define SUMMATOR_H

#include "src/global/global.h"

#include "computationnode.h"

class Summator : public ComputationNode
{
public:
    Summator() { }
    ~Summator() { }

    /**
     * @brief Initialize all internal node data. Will be called once at nodes's creation
     * @param iparams
     * @param dparams
     * @param values
     */
    void init(
            const std::vector<int> &iparams,
            const std::vector<double> &dparams,
            const std::vector<numeric_t> &values
            );

    void forward(
            const std::vector<numeric_t> &input,
            std::vector<numeric_t> &output) const;

    void backward(
            const std::vector<numeric_t> &lhs,
            const std::vector<numeric_t> &rhs,
            std::vector<numeric_t> &output
            );

    void applyLearnedData(double learningRate = 1.0);

    size_t outputSize() const { return 1; }
};

#endif // SUMMATOR_H
