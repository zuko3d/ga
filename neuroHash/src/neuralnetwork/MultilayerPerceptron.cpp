#include "MultilayerPerceptron.h"

#include "src/global/global.h"
#include "src/global/globalstatistics.h"

#include <cstring>
#include <sstream>


MultilayerPerceptron::MultilayerPerceptron(std::vector<size_t> layers)
{
	barriers_.resize(layers.size());

    for (size_t i = 0; i < layers.size(); i++) {
		barriers_[i].resize(layers[i]);
		for (auto& b : barriers_[i]) {
            b = GlobalStatistics::primes_[ hrand() % order_ + GlobalStatistics::startPrime ];
		}
	}
    for(auto& b : barriers_.back()){
        b = 0xFFFFFFFF - 4; // the nearest prime number to MAX_UINT32
    }

	weights_.resize(barriers_.size() - 1);
    for (size_t i = 0; i < weights_.size(); i++) {
		weights_[i].resize(barriers_[i].size());
		for (auto& w : weights_[i]) {
			w.resize(barriers_[i + 1].size());
			for (auto& weight : w) {
                //weight = hrand() & order_;
                weight = GlobalStatistics::primes_[ hrand() % order_ + GlobalStatistics::startPrime ];
			}
		}
	}
}

MultilayerPerceptron::MultilayerPerceptron(std::vector<std::vector<std::vector<uint32_t>>> _weights, std::vector<std::vector<uint32_t>> _barriers) :
	weights_(_weights), barriers_(_barriers)
{

}


MultilayerPerceptron::~MultilayerPerceptron()
{
}

std::string MultilayerPerceptron::calcOut(const std::vector<uint32_t>& _input)
{
	std::vector<uint32_t> current;
	std::vector<uint64_t> next;
	auto input = _input;


	while (input.size() < barriers_[0].size()) {
		input.push_back(0);
	}

	/*
	current.resize(input.size());
	auto& barrier = barriers_[0];
	for (int j = 0; j < barrier.size(); j++) {
	current[j] = barrierFunction(input[j], barrier[j]);
	}
	*/

	current = input;

    for (size_t i = 0; i < weights_.size(); i++) {
		next.clear();
		next.resize(barriers_[i + 1].size(), 0);
        for (size_t j = 0; j < weights_[i].size(); j++) {
            for (size_t k = 0; k < weights_[i][j].size(); k++) {
                next[k] += static_cast<uint64_t>(current[j]) * static_cast<uint64_t>(weights_[i][j][k]);
			}
		}

		current.resize(next.size());
		auto& barrier = barriers_[i + 1];
        for (size_t j = 0; j < barrier.size(); j++) {
			current[j] = barrierFunction(next[j], barrier[j]);
		}
	}

	std::string ret;
	ret.resize(current.size() * sizeof(current[0]));
    /*
    for (size_t i = 0; i < current.size(); i++) {
        //current[i] = _byteswap_ulong(current[i]);
        current[i] = __builtin_bswap32(current[i]);
	}
    */
	memcpy(&ret[0], &current[0], ret.size());
	//return static_cast<uint32_t>(ret & UINT32_MAX);
    return ret;
}

uint32_t MultilayerPerceptron::calcOutUint32(const std::vector<uint32_t> &_input)
{
    std::vector<uint32_t> current;
    std::vector<uint64_t> next;
    auto input = _input;


    while (input.size() < barriers_[0].size()) {
        input.push_back(0);
    }

    current = input;

    for (size_t i = 0; i < weights_.size(); i++) {
        next.clear();
        next.resize(barriers_[i + 1].size(), 0);
        for (size_t j = 0; j < weights_[i].size(); j++) {
            for (size_t k = 0; k < weights_[i][j].size(); k++) {
                next[k] += static_cast<uint64_t>(current[j]) * static_cast<uint64_t>(weights_[i][j][k]);
            }
        }

        current.resize(next.size());
        auto& barrier = barriers_[i + 1];
        for (size_t j = 0; j < barrier.size(); j++) {
            current[j] = barrierFunction(next[j], barrier[j]);
        }
    }

    return current[0];
}

void MultilayerPerceptron::randomlyChangeWeight()
{
    size_t layer = hrand() % (weights_.size());
    size_t neuron = hrand() % weights_[layer].size();
    size_t w = hrand() % weights_[layer][neuron].size();

    //ret.ann_.weights_[layer][neuron][w] = hrand(); // & ret.ann_.order_;
    weights_[layer][neuron][w] = GlobalStatistics::primes_[ hrand() % order_ + GlobalStatistics::startPrime ];
}

void MultilayerPerceptron::randomlyChangeBias()
{
    size_t layer = hrand() % (barriers_.size() - 1);
    size_t neuron = hrand() % barriers_[layer].size();

    barriers_[layer][neuron] = GlobalStatistics::primes_[ hrand() % order_ + GlobalStatistics::startPrime ];
}

hashFunc_t MultilayerPerceptron::getHashFunc()
{
	return std::bind(&MultilayerPerceptron::calcOutSingle, this, std::placeholders::_1, std::placeholders::_2);
}

std::string MultilayerPerceptron::serialize() const
{
	std::ostringstream ss;

	//std::string ret = "barriers:\n";
	ss << "barriers:\n{\n";
    for (size_t j = 0; j < barriers_.size(); j++) {
		auto& layer = barriers_[j];
		ss << "{ ";
        for (size_t i = 0; i < layer.size(); i++) {
			//ret += std::to_string(b) + " ";
			if (i != 0) ss << ", ";
			ss << "0x" << std::hex << std::uppercase << layer[i];
		}
		//ret += "\n";
		ss << " }";
		if (j < (barriers_.size() - 1)) ss << ",";
		ss << "\n";
	}
	//ret += "weights:\n";
	ss << "}\nweights:\n{\n";
    for (size_t k = 0; k < weights_.size(); k++) {
		auto& layer = weights_[k];
		ss << "{ \n";
        for (size_t j = 0; j < layer.size(); j++) {
			auto& neuron = layer[j];
			ss << "{ ";
            for (size_t i = 0; i < neuron.size(); i++) {
				//ret += std::to_string(w) + " ";
				if (i != 0) ss << ", ";
				ss << "0x" << std::hex << std::uppercase << neuron[i];
			}
			//ret += " | ";
			ss << " }";
			if (j < (layer.size() - 1)) ss << ",";
			ss << "\n";
		}
		//ret += "\n";
		ss << "}";
		if (k < (weights_.size() - 1)) ss << ",";
		ss << "\n";
	}
	//ret += "\n";
	ss << "}\n";
	return ss.str();
}

uint32_t MultilayerPerceptron::barrierFunction(uint64_t sum, uint32_t barrier) const
{
	if (!barrier) return 0;
    //sum += 3; // here we kill fixed points 0 and 1
    uint64_t ret = sum % barrier;

    for(int i = 0; i < 6; i++){
        ret *= ret;
        ret %= barrier;
    }

    // uint64_t ret = (sum + 2) % barrier;
    //ret *= ret + 1;
    //ret %= barrier;
    return ret;
}
