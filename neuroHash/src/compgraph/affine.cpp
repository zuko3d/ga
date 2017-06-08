#include "affine.h"

Affine::Affine()
{
    std::cout << "constructed!\n";
}

void Affine::init(size_t rows, size_t cols)
{
    cols_ = cols;
    rows_ = rows;
    matrix_.resize(rows * cols);

    for(auto& num: matrix_) {
        num = (double) (hrand() % 20);
    }
}

void Affine::init(size_t rows, size_t cols, const std::vector<numeric_t> &matrix)
{
    assert((rows * cols) == matrix.size());

    rows_ = rows;
    cols_ = cols;

    matrix_ = matrix;
}

void Affine::forward(const std::vector<numeric_t> &input, std::vector<numeric_t> &output)
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
