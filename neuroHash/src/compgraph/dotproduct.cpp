#include "dotproduct.h"

#include <algorithm>

DotProduct::DotProduct(size_t inputSize)
{
    inputSize_ = inputSize;

    vec_.resize(inputSize_);

    cumulative_delta_.resize(vec_.size());
    for(auto& x: cumulative_delta_) {
        x = 0;
    }
}

DotProduct::DotProduct(const std::vector<numeric_t> &vec)
{
    init({}, {}, vec);
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

    numeric_t gradNormX = 0;
    numeric_t gradNormInner = 0;

    for(const auto& x: lhs) {
        gradNormX += x * x;
    }

    for(const auto& x: vec_) {
        gradNormInner += x * x;
    }

    auto df = rhs[0];

    auto xi = lhs.begin();
    auto ai = vec_.begin();
    auto cum_delta = cumulative_delta_.begin();

    for(auto& out: output) {
        //TODO: improve performance!
        if(gradNormX > 1e-4) {
            (*cum_delta) += df * (*xi) / gradNormX;
        } else {
            (*cum_delta) += df;
        }

        if(gradNormInner > 1e-4) {
            out = df * (*ai) / gradNormInner;
        } else {
            out = df;
        }

        xi++;
        ai++;
        cum_delta++;
    }

    cumuvative_n_++;
}

void DotProduct::applyLearnedData(double learningRate)
{
    double norm = 0;
    for(const auto& x: cumulative_delta_) {
        norm += x * x;
    }

    norm = sqrt(norm);

    double deltaFactor;
    if(norm > 1e-4) {
        deltaFactor = learningRate /cumuvative_n_ / norm;
    } else {
        deltaFactor = learningRate;
    }

    std::transform(cumulative_delta_.begin(), cumulative_delta_.end(), vec_.begin(), vec_.begin(),
                   [deltaFactor] (numeric_t delta, numeric_t x) -> numeric_t {
                        return x + delta * deltaFactor;
                   });

    cumuvative_n_ = 0;
    for(auto& x: cumulative_delta_) {
        x = 0;
    }
}

std::string DotProduct::info() const
{
    std::string ret;

    for(const auto& v: vec_) {
        ret += std::to_string(v) + " ";
    }

    return ret;
}
