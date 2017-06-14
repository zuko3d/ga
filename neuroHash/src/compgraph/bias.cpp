#include "bias.h"

#include <algorithm>

Bias::Bias()
{
    bias_ = hRnd();
}

Bias::Bias(numeric_t bias) : bias_(bias)
{

}

Bias::~Bias()
{

}

void Bias::init(const std::vector<int> &iparams, const std::vector<double> &dparams, const std::vector<numeric_t> &values)
{
    UNUSED(iparams);
    UNUSED(dparams);

    if(values.size() == 1) {
        bias_ = values[0];
    }
}

void Bias::forward(const std::vector<numeric_t> &input, std::vector<numeric_t> &output) const
{
    output.resize(input.size());

    //TODO: check performance
    std::transform(input.begin(), input.end(), output.begin(),
                   [this] (numeric_t x) -> numeric_t { return x + this->bias_; });
}

void Bias::backward(const std::vector<numeric_t> &lhs, const std::vector<numeric_t> &rhs, std::vector<numeric_t> &output)
{
    assert(rhs.size() == lhs.size());
    output.resize(lhs.size());

    std::transform(rhs.begin(), rhs.end(), output.begin(),
                   [this] (numeric_t df) -> numeric_t {
        auto half_df = df * 0.5;

        this->cumulative_delta_ += half_df;
        this->cumuvative_n_++;

        return half_df;
    });
}

void Bias::applyLearnedData(double learningRate)
{
    bias_ += learningRate * cumulative_delta_ / cumuvative_n_;
    cumulative_delta_ = 0;
    cumuvative_n_ = 0;
}

std::string Bias::info() const
{
    std::string ret;

    ret += std::to_string(bias_);

    return ret;
}
