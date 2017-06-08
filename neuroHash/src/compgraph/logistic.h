#ifndef LOGISTIC_H
#define LOGISTIC_H

#include "../global/global.h"

#include "computationnode.h"

#include <vector>

class Logistic
{
public:
    Logistic();

    void forward(const std::vector<numeric_t> &input, std::vector<numeric_t> &output);
};

#endif // LOGISTIC_H
