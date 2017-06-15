#ifndef AFFINE_H
#define AFFINE_H

#include "../global/global.h"

#include "computationnode.h"

#include <vector>

class Affine : public ComputationNode
{
public:
    Affine(size_t inputSize, size_t outputSize);
    Affine(size_t inputSize, size_t outputSize, const std::vector<numeric_t> &values);

    ~Affine() { }

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
            const std::vector<numeric_t> &x,
            const std::vector<numeric_t> &df,
            std::vector<numeric_t> &output);

    void applyLearnedData(double learningRate = 1.0);

    void setInputSize(size_t inputSize) { }

    size_t outputSize() const { return rows_; }
protected:
    size_t rows_ = 0;
    size_t cols_ = 0;
    std::vector<numeric_t> matrix_;

    std::vector<numeric_t> cumulative_delta_;
    int cumuvative_n_ = 0;
};

#endif // AFFINE_H
