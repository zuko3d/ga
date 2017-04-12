#pragma once

#include <vector>
#include "../../neuroHash/src/global.h"

class PerceptronHasher
{
public:
	PerceptronHasher();
	~PerceptronHasher();

	double fitness() const {
		if (fitness_ < 0) {
			std::cout << __FUNCTION__ << " fitness_ = " << fitness_ << "\n";
			assert(fitness_ >= 0);
		}
		return fitness_;
	}

	PerceptronHasher mutate() const;

	PerceptronHasher cross(const PerceptronHasher & p1, const PerceptronHasher & p2) const;

	double calcOut(double x1, double x2) const;

	std::string serialize() const {
		std::string ret = "";
		for (const auto& w : weights) {
			ret += std::to_string(w) + " ";
		}
		return	ret;
	}

	bool operator>(const PerceptronHasher& op) const {
		return fitness_ < op.fitness_; // We compare to sort in "strange" order
	}

	bool operator<(const PerceptronHasher& op) const {
		return fitness_ > op.fitness_;
	}

	int source = -1;

	void simulate();
protected:

	static std::vector<std::vector<double> > trainData;

	void calcFitness();
	double fitness_;

	double costFunction(double x) const;

	std::vector<double> weights;
};

