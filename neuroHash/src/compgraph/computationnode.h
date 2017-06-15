#ifndef COMPUTATIONNODE_H
#define COMPUTATIONNODE_H

#include "../global/global.h"

#include <vector>

typedef double numeric_t;

class ComputationNode
{
public:
    ComputationNode();
    virtual ~ComputationNode();

    /**
     * @brief Initialize all internal node data. Will be called once at nodes's creation
     * @param iparams
     * @param dparams
     * @param values
     */
    virtual void init(
            const std::vector<int> &iparams,
            const std::vector<double> &dparams,
            const std::vector<numeric_t> &values
            ) = 0;

    /**
     * @brief Calculate output (in-place) for given input
     * @param input
     * @param output
     */
    virtual void forward(
            const std::vector<numeric_t> &input,
            std::vector<numeric_t> &output
            ) const = 0;

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
    virtual void backward(
            const std::vector<numeric_t> &lhs,
            const std::vector<numeric_t> &rhs,
            std::vector<numeric_t> &output
            ) = 0;

    /**
     * @brief Apply changes, stored in internal learning buffer.
     */
    virtual void applyLearnedData(double learningRate = 1.0) = 0;

    virtual size_t outputSize() const = 0;

    virtual size_t inputSize() const { return inputSize_; }

    virtual void setInputSize(size_t inputSize) { inputSize_ = inputSize; }

    virtual std::string info() const { return ""; }

    virtual std::string name() const { return "Abstract ComputationNode"; }

protected:
    size_t inputSize_ = 0;
};

#endif // COMPUTATIONNODE_H
