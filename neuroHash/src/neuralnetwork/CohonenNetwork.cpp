#include "CohonenNetwork.h"

#include "src/global/global.h"
#include <cstring>

CohonenNetwork::CohonenNetwork(std::vector<size_t> layers)
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


CohonenNetwork::~CohonenNetwork()
{
}

std::string CohonenNetwork::calcOut(const std::vector<uint32_t>& input)
{
	std::vector<uint32_t> current;
	std::vector<uint64_t> next;

	current = input;
	
	for (int i = 0; i < weights_.size(); i++) {
		next.resize(barriers_[i + 1].size(), 0);
		size_t winner = 0;
		for (int j = 1; j < current.size(); j++) {
			if (current[j] > current[winner]) {
				winner = j;
			}
		}
		auto sz = weights_[i][winner].size();
		auto curWinner = current[winner];
		for (int k = 0; k < sz; k++) {
			next[k] += curWinner * weights_[i][winner][k];
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
		current[i] = static_cast<uint32_t>(next[i] & 0xFFFFFFFF);
	}
	memcpy(&ret[0], &current[0], ret.size());
	//return static_cast<uint32_t>(ret & UINT32_MAX);
	return ret;
}

std::function<std::string(uint32_t)> CohonenNetwork::getHashFunc()
{
	return std::bind(&CohonenNetwork::calcOutSingle, this, std::placeholders::_1);
}

std::string CohonenNetwork::serialize() const
{
	std::string ret = "barriers:\n";
	for (const auto& layer : barriers_) {
		for (const auto& b : layer) {
			ret += std::to_string(b) + " ";
		}
		ret += "\n";
	}
	ret += "weights:\n";
	for (const auto& layer : weights_) {
		for (const auto& neuron : layer) {
			for (const auto& w : neuron) {
				ret += std::to_string(w) + " ";
			}
			ret += " | ";
		}
		ret += "\n";
	}
	ret += "\n";

	return ret;
}

uint32_t CohonenNetwork::barrierFunction(uint64_t sum, uint32_t barrier) const
{
	if (!barrier) return 0;
	sum += 2; // here we kill fixed points 0 and 1
	uint64_t ret = sum % barrier;
	ret *= ret;
	ret %= barrier;
	ret *= ret;
	ret %= barrier;
	ret *= sum;
	return ret % barrier;
}
