#ifndef TESTPHENOTYPEDIOFANT_H
#define TESTPHENOTYPEDIOFANT_H

#include "phenotype.h"

class TestPhenotypeDiofant : public Phenotype
{
public:
    TestPhenotypeDiofant();

    double fitness() const { 
		if (fitness_ < 0) {
			std::cout << __FUNCTION__<< " fitness_ = " << fitness_ << "\n";
			assert(fitness_ >= 0);
		}		
		return fitness_; 
	}

    TestPhenotypeDiofant mutate();

    TestPhenotypeDiofant cross(const TestPhenotypeDiofant &otherParent) const;

    std::string serialize() const { return	std::to_string(x_) + ", " + 
											std::to_string(y_) + ", " +
											std::to_string(a_) + ", " +
											std::to_string(b_) + ", " +
											std::to_string(c_) + ", " +
											std::to_string(d_) + ", ";}

    TestPhenotypeDiofant operator+(const TestPhenotypeDiofant& otherParent) const {
        return this->cross(otherParent);
    }

protected:
    void calcFitness();

    double x_, y_, a_, b_, c_, d_;
};

#endif // TESTPHENOTYPEDIOFANT_H
