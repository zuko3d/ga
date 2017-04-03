#ifndef TESTPHENOTYPE_H
#define TESTPHENOTYPE_H

#include "phenotype.h"

class TestPhenotype : public Phenotype
{
public:
    TestPhenotype();

    double fitness() const { return fitness_; }

    TestPhenotype mutate();

    TestPhenotype cross(const TestPhenotype &otherParent) const;

    std::string serialize() const { return std::to_string(x_); }

    TestPhenotype operator+(const TestPhenotype& otherParent) const {
        return this->cross(otherParent);
    }

protected:
    void calcFitness();

    double x_;
};

#endif // TESTPHENOTYPE_H
