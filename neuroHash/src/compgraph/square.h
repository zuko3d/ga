#ifndef SQUARE_H
#define SQUARE_H


class Square
{
public:
    Square();

    void forward(const std::vector<numeric_t> &input, std::vector<numeric_t> &output);

    void calcGrad(const std::vector<numeric_t> &input, const std::vector<numeric_t> &output);

    const std::vector<numeric_t> &grad() const;

protected:
    std::vector<numeric_t> grad_;
};

#endif // SQUARE_H
