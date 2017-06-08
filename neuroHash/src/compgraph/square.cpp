#include "square.h"

Square::Square()
{

}

void Square::forward(const std::vector<numeric_t> &input, std::vector<numeric_t> &output)
{
    assert(input.size() == output.size());

    auto it_in = input.begin();
    auto it_end = input.end();
    auto it_out = output.begin();

    while(it_in != it_end) {
        *it_out = (*it_in) * (*it_in);
        it_in++;
        it_out++;
    }
}

void Square::calcGrad(const std::vector<numeric_t> &input, const std::vector<numeric_t> &output)
{

}
