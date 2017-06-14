#include "summator.h"

#include <algorithm>

void Summator::init(const std::vector<int> &iparams, const std::vector<double> &dparams, const std::vector<numeric_t> &values)
{
    UNUSED(iparams);
    UNUSED(dparams);
    UNUSED(values);
}

void Summator::forward(const std::vector<numeric_t> &input, std::vector<numeric_t> &output) const
{
    output.resize(1);

    numeric_t cur = 0;

    for(const auto& num: input) {
        cur += num;
    }

    output[0] = cur;
}

void Summator::backward(
        const std::vector<numeric_t> &lhs,
        const std::vector<numeric_t> &rhs,
        std::vector<numeric_t> &output)
{
    assert(rhs.size() == 1);
    output.resize(lhs.size());

    // Gradient of linear summator is { 1.0, 1.0, ....., 1.0 }
    numeric_t grad_norm = lhs.size();

    auto df_div_grad_norm = rhs[0] / grad_norm;

    for(auto& num: output) {
        num = df_div_grad_norm;
    }
}

void Summator::applyLearnedData(double learningRate)
{
    UNUSED(learningRate);
}
