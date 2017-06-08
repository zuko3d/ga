#include "summator.h"

Summator::Summator()
{

}

void Summator::forward(const std::vector<numeric_t> &input, std::vector<numeric_t> &output) const
{
    assert(output.size() == 1);

    numeric_t cur = 0;

    for(const auto& num: input) {
        cur += num;
    }

    output[0] = cur;
}

void Summator::calcGrad(const std::vector<numeric_t> &input, const std::vector<numeric_t> &output)
{
    assert(output.size() == 1);

    grad_.resize(input.size(), 0);

    for(auto& num: grad_) {
        num = output[0];
    }
}

const std::vector<numeric_t> &Summator::grad() const
{
    return grad_;
}
