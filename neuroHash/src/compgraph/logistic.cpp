#include "logistic.h"

Logistic::Logistic()
{

}

void Logistic::forward(const std::vector<numeric_t> &input, std::vector<numeric_t> &output)
{
    assert(input.size() == output.size());

    auto it_in = input.begin();
    auto it_end = input.end();
    auto it_out = output.begin();

    while(it_in != it_end) {
        if((*it_in) > 30) {
            *it_out = 1.0;
        } else if ((*it_in) < -30) {
            *it_out = 0.0;
        } else {
            *it_out = 1.0 / ( 1.0 + exp(- (*it_in)));
        }

        it_in++;
        it_out++;
    }
}
