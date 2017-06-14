#ifndef SQUARE_H
#define SQUARE_H

#include "../global/global.h"

#include "computationnode.h"

#include <vector>

class Square : public ComputationNode
{
public:
    Square() { }
    ~Square() { }

    /**
     * @brief Initialize all internal node data. Will be called once at nodes's creation
     * @param iparams
     * @param dparams
     * @param values
     */
    void init(
            const std::vector<int> &iparams,
            const std::vector<double> &dparams,
            const std::vector<numeric_t> &values
        );

    /**
     * @brief Calculate output (in-place) for given input
     * @param input
     * @param output
     */
    void forward(
            const std::vector<numeric_t> &input,
            std::vector<numeric_t> &output
        ) const;

    /**
     * Propagate df backward. After this function internal "learning" state is changed,
     * but no real changes are done to internal parameters. To apply changes -
     * call applyLearnedData().
     *
     * Output will consists of deltas for input values in such a way that ouput will change on rhs.
     *
     * @brief Backward step
     * @param lhs "Original" input (not differentials!) to that nodes
     * @param rhs Expected differentials of ouput
     * @param output Output parameters, storing results
     */
    void backward(
            const std::vector<numeric_t> &lhs,
            const std::vector<numeric_t> &rhs,
            std::vector<numeric_t> &output
        );


    /**
     * @brief Apply changes, stored in internal learning buffer.
     */
    void applyLearnedData(double learningRate = 1.0);

    size_t outputSize() const { return inputSize_; }
};

#endif // SQUARE_H
