#pragma once

#include <vector>
#include "../../neuroHash/src/global.h"

class MaxEquation
{
public:
	MaxEquation();
	~MaxEquation();

	double fitness() const {
		if (fitness_ < 0) {
			std::cout << __FUNCTION__ << " fitness_ = " << fitness_ << "\n";
			assert(fitness_ >= 0);
		}
		return fitness_;
	}

	MaxEquation mutate() const;

	MaxEquation cross(const MaxEquation & p1, const MaxEquation & p2) const;

	std::string serialize() const {
		std::string ret = "";
		for (const auto& w : weights) {
			ret += std::to_string(w) + " ";
		}
		return	ret;
	}

	bool operator>(const MaxEquation& op) const {
		return fitness_ < op.fitness_; // We compare to sort in "strange" order
	}

	bool operator<(const MaxEquation& op) const {
		return fitness_ > op.fitness_;
	}

	int source = -1;

	void simulate();
protected:

	static std::vector<std::vector<double> > trainData;

	void calcFitness();
	double fitness_;

	std::vector<double> weights;
};

