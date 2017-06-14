#include "square.h"

#include <algorithm>

void Square::init(const std::vector<int> &iparams, const std::vector<double> &dparams, const std::vector<numeric_t> &values)
{
    UNUSED(iparams);
    UNUSED(dparams);
    UNUSED(values);
}

void Square::forward(const std::vector<numeric_t> &input, std::vector<numeric_t> &output) const
{
    output.resize(input.size());

    std::transform(input.begin(), input.end(), output.begin(),
                   [] (numeric_t x) -> numeric_t { return x * x; });
}

void Square::backward(const std::vector<numeric_t> &lhs, const std::vector<numeric_t> &rhs, std::vector<numeric_t> &output)
{
    assert(rhs.size() == lhs.size());
    output.resize(lhs.size());

    numeric_t gradNorm = 0;

    for(const auto& x: lhs) {
        gradNorm += 4.0 * x * x;
    }

    if(gradNorm < 1e-2) { // it's always >= 0
        output = rhs;
    } else {
        std::transform(rhs.begin(), rhs.end(), lhs.begin(), output.begin(),
                       [gradNorm] (numeric_t df, numeric_t x) -> numeric_t {
            return df * 2.0 * x / gradNorm;
        });
    }
}

void Square::applyLearnedData(double learningRate)
{
    UNUSED(learningRate);
}
