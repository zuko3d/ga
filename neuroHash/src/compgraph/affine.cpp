#include "affine.h"

#include <algorithm>

Affine::Affine(size_t inputSize, size_t outputSize)
{
    init({inputSize, outputSize}, {}, {});
}

Affine::Affine(size_t inputSize, size_t outputSize, const std::vector<numeric_t> &values)
{
    init({inputSize, outputSize}, {}, values);
}

void Affine::init(const std::vector<int> &iparams, const std::vector<double> &dparams, const std::vector<numeric_t> &values)
{
    assert(iparams.size() == 2);
    cols_ = iparams[0];
    rows_ = iparams[1];

    inputSize_ = cols_;

    if(values.size() == cols_ * rows_) {
        matrix_ = values;
    } else if (values.size() != 0) {
        std::cerr << __FUNCTION__ <<  " Wrong values size!" << std::endl;
    } else {
        matrix_.resize(rows_ * cols_);
        for(auto& num: matrix_) {
            num = hRnd();
        }
    }

    cumulative_delta_.resize(matrix_.size(), 0);
}

void Affine::forward(const std::vector<numeric_t> &input, std::vector<numeric_t> &output) const
{
    assert(input.size() == cols_);
    assert(output.size() == rows_);

    for(auto& num: output) {
        num = 0;
    }

    for(size_t i = 0; i < rows_; i++) {
        numeric_t cur = 0;
        size_t idx = cols_ * i;
        for(size_t j = 0; j < cols_; j++) {
            cur += input[j] * matrix_[idx];
            idx++;
        }
        output[i] = cur;
    }
}

void Affine::backward(const std::vector<numeric_t> &x, const std::vector<numeric_t> &df, std::vector<numeric_t> &output)
{
    assert(df.size() == rows_);
    assert(x.size() == cols_);

    output.resize(cols_);

    numeric_t normX = 0;
    numeric_t gradNormInner = 0;

    for(const auto& xi: x) {
        normX += xi * xi;
    }

    auto ai = matrix_.begin();
    auto cum_delta = cumulative_delta_.begin();

    for(int i = 0; i < rows_; i++) {
        auto xi = x.begin();

        gradNormInner = 0;
        auto ai2 = ai;
        for(int i = 0; i < cols_; i++) {
            gradNormInner += (*ai2) * (*ai2);
        }

        for(auto& out: output) {
            if(normX > 1e-4) {
                (*cum_delta) += df[i] * (*xi) / normX;
            } else {
                (*cum_delta) += df[i];
            }

            if(gradNormInner > 1e-4) {
                out = df[i] * (*ai) / gradNormInner;
            } else {
                out = df[i];
            }

            xi++;
            ai++;
            cum_delta++;
        }
    }

    cumuvative_n_++;
}

void Affine::applyLearnedData(double learningRate)
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

    std::transform(cumulative_delta_.begin(), cumulative_delta_.end(), matrix_.begin(), matrix_.begin(),
                   [deltaFactor] (numeric_t delta, numeric_t x) -> numeric_t {
                        return x + delta * deltaFactor;
                   });

    cumuvative_n_ = 0;
    for(auto& x: cumulative_delta_) {
        x = 0;
    }
}
