#pragma once

#include "../../neuroHash/src/global.h"
#include <unordered_set>

template<class Ann> class AnnHasher
{
public:
	AnnHasher<Ann>();
	~AnnHasher<Ann>();

	double fitness() const {
		if (fitness_ < 0) {
			std::cout << __FUNCTION__ << " fitness_ = " << fitness_ << "\n";
			assert(fitness_ >= 0);
		}
		return fitness_;
	}

	AnnHasher<Ann> mutate() const;

	AnnHasher<Ann> cross(const AnnHasher<Ann> & p1, const AnnHasher<Ann> & p2) const;
	
	std::string serialize() const {
		return ann_.serialize();
	}

	bool operator>(const AnnHasher<Ann>& op) const {
		return fitness_ < op.fitness_; // We compare to sort in "strange" order
	}

	bool operator<(const AnnHasher<Ann>& op) const {
		return fitness_ > op.fitness_;
	}

	int source = -1;

protected:
	void calcFitness();
	double fitness_;

	Ann ann_;
};

#include "MultilayerPerceptron.h"

template<class Ann>
AnnHasher<Ann>::AnnHasher() : ann_({1,4,5,4,4,3,1 })
{
	calcFitness();
	source = 0;
}

template<class Ann>
AnnHasher<Ann>::~AnnHasher()
{
}

template<class Ann>
AnnHasher<Ann> AnnHasher<Ann>::mutate() const
{
	AnnHasher<Ann> ret(*this);

	size_t layer = hrand() % ret.ann_.barriers_.size();
	size_t neuron = hrand() % ret.ann_.barriers_[layer].size();

	ret.ann_.barriers_[layer][neuron] += hrand() % 64000 - 32000;

	layer = hrand() % ret.ann_.weights_.size();
	neuron = hrand() % ret.ann_.weights_[layer].size();
	size_t w = hrand() % ret.ann_.weights_[layer][neuron].size();

	ret.ann_.weights_[layer][neuron][w] += hrand() % 64000 - 32000;

	ret.source = 1;
	ret.calcFitness();
	return ret;
}

template<class Ann>
AnnHasher<Ann> AnnHasher<Ann>::cross(const AnnHasher<Ann> & p1, const AnnHasher<Ann>& p2) const
{
	// TODO: don't copy! just resize it before usage!
	AnnHasher<Ann> ret(*this);
	
	for (size_t i = 0; i < ann_.barriers_.size(); i++) {
		for (size_t j = 0; j < ann_.barriers_[i].size(); j++) {
			auto& r1 = ann_.barriers_[i][j];
			auto& r2 = p1.ann_.barriers_[i][j];
			auto& r3 = p2.ann_.barriers_[i][j];
			ret.ann_.barriers_[i][j] = (r1 | r2) & (r1 | r3) & (r2 | r3);
		}
	}

	for (size_t i = 0; i < ann_.weights_.size(); i++) {
		for (size_t j = 0; j < ann_.weights_[i].size(); j++) {
			for (size_t k = 0; k < ann_.weights_[i][j].size(); k++) {
				auto& r1 = ann_.weights_[i][j][k];
				auto& r2 = p1.ann_.weights_[i][j][k];
				auto& r3 = p2.ann_.weights_[i][j][k];
				ret.ann_.weights_[i][j][k] = (r1 | r2) & (r1 | r3) & (r2 | r3);
			}
		}
	}

	ret.source = 2;
	ret.calcFitness();
	return ret;
}

template<class Ann>
void AnnHasher<Ann>::calcFitness()
{
	int tests = 100;
	int localtests = 2;

	const auto bitsize = sizeof(ann_.barriers_[0][0]) * 8;
	uint32_t totalErr = 0;
	/*
	for (int test = 0; test < tests; test++) {
		uint32_t in = hrand();
		std::vector<uint32_t> out;
		out.resize(localtests);
		for (auto& n : out) {
			auto tmpIn = in ^ (1 << (rand() % bitsize));
			n = ann_.calcOut({ tmpIn });
		}

		out.push_back(ann_.calcOut({ in }));
		for(int i = 0; i < out.size(); i++){
			for(int j = i + 1; j < out.size(); j++) {
				auto fit = __popcnt(out[i] ^ out[j]);

				if (fit > bitsize / 2) {
					fit -= bitsize / 2;
				}
				else {
					fit = bitsize / 2 - fit;
				}
				totalErr += fit;
			}
		}
	}
	*/
	const uint32_t collTests = 1000;
	std::unordered_set<uint32_t> hashes;
	for (uint32_t i = 0; i < collTests; i++) {
		hashes.insert(ann_.calcOut({ i }));
	}

	fitness_ = 1.0 / (1.0 + totalErr) / (1.0 + collTests - hashes.size());
}

