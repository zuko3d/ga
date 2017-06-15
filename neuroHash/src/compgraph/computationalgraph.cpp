#include "computationalgraph.h"

#include <algorithm>

ComputationalGraph::ComputationalGraph(size_t inputSize) :
    inputSize_(inputSize)
{

}

ComputationalGraph::~ComputationalGraph()
{
    for(auto& node: nodes_) {
        delete node;
    }
}

void ComputationalGraph::prepareForCalculations()
{
    if(prepared_) return;

    valBuffers_.resize(nodes_.size() + 1); // one more buffer for input
    gradBuffers_.resize(nodes_.size() + 1);

    size_t prevNodeOutSize = inputSize_;

    auto valBuffer = valBuffers_.begin() + 1;
    auto gradBuffer = gradBuffers_.begin() + 1;

    valBuffers_[0].resize(inputSize_);
    gradBuffers_[0].resize(inputSize_);

    for(auto& node: nodes_) {
        node->setInputSize(prevNodeOutSize);
        prevNodeOutSize = node->outputSize();
        valBuffer->resize(prevNodeOutSize);
        valBuffer++;
        gradBuffer->resize(prevNodeOutSize);
        gradBuffer++;
    }

    prepared_ = true;
}

void ComputationalGraph::forward(const std::vector<numeric_t> &input, std::vector<numeric_t> &output)
{
    prepareForCalculations();

    assert(valBuffers_[0].size() == input.size());

    valBuffers_[0] = input;

    auto node = nodes_.begin();

    for(size_t i = 1; i < valBuffers_.size(); i++, node++) {
        (*node)->forward(valBuffers_[i - 1], valBuffers_[i]);
    }

    output = valBuffers_.back();
}

void ComputationalGraph::backward(const std::vector<numeric_t> &df)
{
    assert(prepared_);
    assert(gradBuffers_.back().size() == df.size());

    gradBuffers_.back() = df;

    auto node = nodes_.end() - 1;

    for(size_t i = gradBuffers_.size() - 1; i > 0; i--, node--){
        (*node)->backward(valBuffers_[i - 1], gradBuffers_[i], gradBuffers_[i - 1]);
    }
}

void ComputationalGraph::applyLearnedData(double learningRate)
{
    for(auto& node: nodes_) {
        node->applyLearnedData(learningRate);
    }
}

void ComputationalGraph::addNode(ComputationNode *node)
{
    nodes_.push_back(node);

    prepared_ = false;
}

numeric_t ComputationalGraph::train(const std::vector<std::vector<numeric_t> > &inputs, const std::vector<std::vector<numeric_t> > &expectedOutputs, double learningRate)
{
    assert(inputs.size() == expectedOutputs.size());
    assert(inputs.front().size() == nodes_.front()->inputSize());
    assert(expectedOutputs.front().size() == nodes_.back()->outputSize());

    auto expectedOut = expectedOutputs.begin();
    std::vector<numeric_t> out;
    out.resize(nodes_.back()->outputSize());

    numeric_t totalError = 0;

    for(const auto& input: inputs) {
        forward(input, out);
        std::transform(out.begin(), out.end(), expectedOut->begin(), out.begin(),
                [] (numeric_t guess, numeric_t truth) -> numeric_t {
                    return truth - guess;
                });

        for(const auto& num: out) {
            totalError += num * num;
        }

        backward(out);
        expectedOut++;
    }

    applyLearnedData(learningRate);

    // std::cout << "totalError = " << totalError << std::endl;
    return totalError;
}

void ComputationalGraph::printResults(const std::vector<std::vector<numeric_t> > &inputs, const std::vector<std::vector<numeric_t> > &expectedOutputs)
{
    auto expected = expectedOutputs.begin();
    std::vector<numeric_t> out;
    out.resize(nodes_.back()->outputSize());

    for(const auto& input: inputs) {
        for(const auto& i: input) {
            std::cout << i << "\t";
        }
        std::cout << ":\t";
        for(const auto& i: *expected) {
            std::cout << i << "\t";
        }

        std::cout << "|\t";

        forward(input, out);
        for(const auto& i: out) {
            std::cout << i << "\t";
        }
        std::cout << std::endl;

        expected++;
    }

}

ComputationNode *ComputationalGraph::operator [](size_t index)
{
    return nodes_[index];
}
