#include "MultilayerPerceptron.h"

#include "../../neuroHash/src/global.h"
#include <cstring>
#include <sstream>

MultilayerPerceptron::MultilayerPerceptron(std::vector<size_t> layers)
{
	barriers_.resize(layers.size());

	for (int i = 0; i < layers.size(); i++) {
		barriers_[i].resize(layers[i]);
		for (auto& b : barriers_[i]) {
			b = 1 + hrand();
		}
	}

	weights_.resize(barriers_.size() - 1);
	for (int i = 0; i < weights_.size(); i++) {
		weights_[i].resize(barriers_[i].size());
		for (auto& w : weights_[i]) {
			w.resize(barriers_[i + 1].size());
			for (auto& weight : w) {
				weight = hrand();
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

	for (int i = 0; i < weights_.size(); i++) {
		next.clear();
		next.resize(barriers_[i + 1].size(), 0);
		for (int j = 0; j < weights_[i].size(); j++) {
			for (int k = 0; k < weights_[i][j].size(); k++) {
				next[k] += static_cast<uint64_t>(current[j]) * weights_[i][j][k];
			}
		}

		current.resize(next.size());
		auto& barrier = barriers_[i + 1];
		for (int j = 0; j < barrier.size(); j++) {
			current[j] = barrierFunction(next[j], barrier[j]);
		}
	}

	std::string ret;
	ret.resize(current.size() * sizeof(current[0]));
	for (int i = 0; i < current.size(); i++) {
		current[i] = _byteswap_ulong(current[i]);
	}
	memcpy(&ret[0], &current[0], ret.size());
	//return static_cast<uint32_t>(ret & UINT32_MAX);
	return ret;
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
	for (int j = 0; j < barriers_.size(); j++) {
		auto& layer = barriers_[j];
		ss << "{ ";
		for (int i = 0; i < layer.size(); i++) {
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
	for (int k = 0; k < weights_.size(); k++) {
		auto& layer = weights_[k];
		ss << "{ \n";
		for (int j = 0; j < layer.size(); j++) {
			auto& neuron = layer[j];
			ss << "{ ";
			for (int i = 0; i < neuron.size(); i++) {
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
	//sum += 2; // here we kill fixed points 0 and 1
	//uint64_t ret = sum % barrier;
	//ret *= ret;
	//ret %= barrier;
	//ret *= ret;
	//ret %= barrier;
	//ret *= sum;

	uint64_t ret = (sum + 2) % barrier;
	ret *= (sum + 3) % barrier;
	return ret % barrier;
}
