#pragma once

#include "src/global/global.h"

#include "../global/globalstatistics.h"

#include <vector>
#include <cstdint>
#include <string>
#include <cstring>
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
    uint32_t calcOutUint32(const std::vector<uint32_t>& _input);
	std::string calcOutSingle(uint32_t* input, size_t size) {
		std::vector<uint32_t> v;
		v.resize(size);
        std::memcpy(&v[0], input, sizeof(uint32_t) * size);
		return calcOut(v);
	}

    std::vector<std::vector<uint32_t> > getInternalResults(const std::vector<uint32_t>& _input);

    void randomlyChangeWeight();
    void randomlyChangeBias();

    void reduceWeights();

	hashFunc_t getHashFunc();

	std::string serialize() const;


// Those should be private but we want to change the ANN from outside in an easy manner
	uint32_t barrierFunction(uint64_t sum, uint32_t barrier) const;

	std::vector< std::vector<std::vector<uint32_t> > > weights_;
	std::vector< std::vector<uint32_t> > barriers_;

    uint32_t order_ = GlobalStatistics::mlpOrder;
};
