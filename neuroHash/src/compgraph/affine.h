#ifndef AFFINE_H
#define AFFINE_H

#include "../global/global.h"

#include "computationnode.h"

#include <vector>

class Affine
{
public:
    Affine();

    Affine(size_t rows, size_t cols);

    void init(
            std::vector<int> iparams,
            std::vector<double> dparams,
            std::vector<numeric_t> values
            );

    void init(size_t rows, size_t cols);
    void init(size_t rows, size_t cols, const std::vector<numeric_t> &matrix);

    void forward(const std::vector<numeric_t> &input, std::vector<numeric_t> &output);

protected:
    size_t rows_ = 0;
    size_t cols_ = 0;
    std::vector<numeric_t> matrix_;
};

#endif // AFFINE_H
