#pragma once

#include "../../neuroHash/src/global.h"

#include <vector>
#include <cstdint>
#include <string>
#include <functional>

class MultilayerPerceptron
{
public:
	MultilayerPerceptron(std::vector<size_t> layers);
	MultilayerPerceptron(
		std::vector< std::vector<std::vector<uint32_t> > > _weights,
		std::vector< std::vector<uint32_t> > _barriers
	);
	~MultilayerPerceptron();

	std::string calcOut(const std::vector<uint32_t>& input);
	std::string calcOutSingle(uint32_t* input, size_t size) {
		std::vector<uint32_t> v;
		v.resize(size);
		memcpy(&v[0], input, sizeof(uint32_t) * size);
		return calcOut(v);
	}

	hashFunc_t getHashFunc();

	std::string serialize() const;

// Those should be private but we want to change the ANN from outside in an easy manner
	uint32_t barrierFunction(uint64_t sum, uint32_t barrier) const;

	std::vector< std::vector<std::vector<uint32_t> > > weights_;
	std::vector< std::vector<uint32_t> > barriers_;
};