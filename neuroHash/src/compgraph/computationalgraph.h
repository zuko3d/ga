#ifndef COMPUTATIONALGRAPH_H
#define COMPUTATIONALGRAPH_H

#include "computationnode.h"

#include <vector>

class ComputationalGraph
{
public:
    ComputationalGraph(size_t inputSize = 0);

    ~ComputationalGraph();

    void prepareForCalculations();

    void forward(const std::vector<numeric_t>& input, std::vector<numeric_t> &output);

    void backward(const std::vector<numeric_t> &df);

    void applyLearnedData(double learningRate = 1.0);

    /**
     * @brief addNode
     * @param node Takes ownership of the node!
     */
    void addNode(ComputationNode* node);

    numeric_t train(
            const std::vector<std::vector<numeric_t> >& inputs,
            const std::vector<std::vector<numeric_t> >& expectedOutputs,
            double learningRate = 1.0
            );

    void printResults(
            const std::vector<std::vector<numeric_t> >& inputs,
            const std::vector<std::vector<numeric_t> >& expectedOutputs
            );

    ComputationNode* operator [](size_t index);

protected:
    bool prepared_ = false;

    size_t inputSize_ = 0;

    std::vector<ComputationNode*> nodes_;

    std::vector<std::vector<numeric_t> > valBuffers_;
    std::vector<std::vector<numeric_t> > gradBuffers_;
};

#endif // COMPUTATIONALGRAPH_H
