#include "dotproduct.h"

#include <algorithm>

DotProduct::DotProduct(size_t inputSize) :
    inputSize_(inputSize)
{

}

DotProduct::DotProduct(const std::vector<numeric_t> &vec) :
    vec_(vec)
{
    inputSize_ = vec_.size();

    cumulative_delta_.resize(vec_.size());
    for(auto& x: cumulative_delta_) {
        x = 0;
    }
}

void DotProduct::init(const std::vector<int> &iparams, const std::vector<double> &dparams, const std::vector<numeric_t> &values)
{
    UNUSED(iparams);
    UNUSED(dparams);

    vec_ = values;
    inputSize_ = vec_.size();

    cumulative_delta_.resize(vec_.size());
    for(auto& x: cumulative_delta_) {
        x = 0;
    }
}

void DotProduct::forward(const std::vector<numeric_t> &input, std::vector<numeric_t> &output) const
{
    output.resize(1);

    auto v = vec_.begin();
    numeric_t ret = 0;

    for(const auto& x:input) {
        ret += x * (*v);

        v++;
    }

    output[0] = ret;
}

void DotProduct::backward(const std::vector<numeric_t> &lhs, const std::vector<numeric_t> &rhs, std::vector<numeric_t> &output)
{
    assert(rhs.size() == 1);
    output.resize(lhs.size());

    numeric_t gradNormLhs = 0;
    numeric_t gradNormInner = 0;

    for(const auto& x: lhs) {
        gradNormLhs += x * x;
    }

    for(const auto& x: vec_) {
        gradNormInner += x * x;
    }

    auto df = rhs[0];

    //TODO: handle case |gradient| = 0

    auto xi = lhs.begin();
    auto ai = vec_.begin();
    auto cum_delta = cumulative_delta_.begin();

    for(auto& out: output) {
        (*cum_delta) += df * (*xi) / gradNormLhs;
        out = df * (*ai) / gradNormInner;

        cumuvative_n_++;
        xi++;
        ai++;
        cum_delta++;
    }
}

void DotProduct::applyLearnedData(double learningRate)
{
    auto cumuvative_n = cumuvative_n_;
    std::transform(cumulative_delta_.begin(), cumulative_delta_.end(), vec_.begin(), vec_.begin(),
                   [cumuvative_n, learningRate] (numeric_t delta, numeric_t x) -> numeric_t {
                        return x + delta * learningRate / cumuvative_n;
                   });

    cumuvative_n_ = 0;
    for(auto& x: cumulative_delta_) {
        x = 0;
    }
}
