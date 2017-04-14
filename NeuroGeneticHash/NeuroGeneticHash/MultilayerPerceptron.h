#pragma once

#include <vector>
#include <cstdint>
#include <string>

class MultilayerPerceptron
{
public:
	MultilayerPerceptron(std::vector<size_t> layers);
	~MultilayerPerceptron();

	uint32_t calcOut(const std::vector<uint32_t> input) const;

	std::string serialize() const;

// Those should be private but we want to change the ANN from outside in an easy manner
	uint32_t barrierFunction(uint64_t sum, uint32_t barrier) const;

	std::vector< std::vector<std::vector<uint32_t> > > weights_;
	std::vector< std::vector<uint32_t> > barriers_;
};