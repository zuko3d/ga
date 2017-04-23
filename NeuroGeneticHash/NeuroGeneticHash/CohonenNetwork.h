#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <functional>

class CohonenNetwork
{
public:
	CohonenNetwork(std::vector<size_t> layers);
	~CohonenNetwork();

	std::string calcOut(const std::vector<uint32_t>& input);
	std::string calcOutSingle(uint32_t input) {
		return calcOut({ input });
	}

	std::function<std::string(uint32_t)> getHashFunc();

	std::string serialize() const;

	// Those should be private but we want to change the ANN from outside in an easy manner
	uint32_t barrierFunction(uint64_t sum, uint32_t barrier) const;

	std::vector< std::vector<std::vector<uint32_t> > > weights_;
	std::vector< std::vector<uint32_t> > barriers_;
};

