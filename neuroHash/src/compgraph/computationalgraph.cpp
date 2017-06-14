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
