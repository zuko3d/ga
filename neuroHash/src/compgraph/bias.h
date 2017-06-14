#ifndef BIAS_H
#define BIAS_H

#include "../global/global.h"

#include "computationnode.h"

#include <vector>

class Bias : public ComputationNode
{
public:
    Bias();
    Bias(numeric_t bias);

    ~Bias();

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
            std::vector<numeric_t> &output
            ) const;

    void backward(
            const std::vector<numeric_t> &lhs,
            const std::vector<numeric_t> &rhs,
            std::vector<numeric_t> &output);

    void applyLearnedData(double learningRate = 1.0);

    std::string info() const;

    size_t outputSize() const { return inputSize_; }

protected:
    numeric_t bias_;

    numeric_t cumulative_delta_ = 0.0;
    int cumuvative_n_ = 0;
};

#endif // BIAS_H
