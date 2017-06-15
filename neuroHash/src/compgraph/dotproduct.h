#ifndef DOTPRODUCT_H
#define DOTPRODUCT_H

#include "../global/global.h"

#include "computationnode.h"

#include <vector>

class DotProduct : public ComputationNode
{
public:
    DotProduct() { }
    DotProduct(size_t inputSize);
    DotProduct(const std::vector<numeric_t> &vec);

    ~DotProduct() { }

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

    size_t outputSize() const { return 1; }

protected:
    std::vector<numeric_t> vec_;

    std::vector<numeric_t> cumulative_delta_;
    int cumuvative_n_ = 0;
};

#endif // DOTPRODUCT_H
