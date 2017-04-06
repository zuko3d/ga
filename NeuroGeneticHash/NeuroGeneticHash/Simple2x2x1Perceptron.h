#pragma once

#include <vector>
#include "../../neuroHash/src/global.h"

class Simple2x2x1Perceptron
{
public:
	Simple2x2x1Perceptron();
	~Simple2x2x1Perceptron();
	
	double fitness() const {
		if (fitness_ < 0) {
			std::cout << __FUNCTION__ << " fitness_ = " << fitness_ << "\n";
			assert(fitness_ >= 0);
		}
		return fitness_;
	}

	Simple2x2x1Perceptron mutate() const;

	Simple2x2x1Perceptron cross(const Simple2x2x1Perceptron & p1, const Simple2x2x1Perceptron & p2) const;

	std::string serialize() const {
		std::string ret = "";
		for (const auto& w : weights) {
			ret += std::to_string(w) + " ";
		}
		return	ret;
	}
	
	bool operator>(const Simple2x2x1Perceptron& op) const {
		return fitness_ < op.fitness_; // We compare to sort in "strange" order
	}

	bool operator<(const Simple2x2x1Perceptron& op) const {
		return fitness_ > op.fitness_;
	}

	int source = -1;

    void simulate();
protected:

	static std::vector<std::vector<double> > trainData;

	void calcFitness();
	double fitness_;

	double costFunction(double x);

	std::vector<double> weights;
	
};

