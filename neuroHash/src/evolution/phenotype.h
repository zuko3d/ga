#ifndef PHENOTYPE_H
#define PHENOTYPE_H

#include "src/global/global.h"

#include <string>

class Phenotype
{
public:
    Phenotype() { }

    // The more - the better
    double fitness() const { assert(false); return fitness_; }

    Phenotype mutate() { assert(false); return *this; }

    Phenotype cross(const Phenotype& otherParent) const { assert(false); return otherParent; }

    std::string serialize() const { assert(false); return "NO!"; }

    Phenotype operator+(const Phenotype& otherParent) const {
        return this->cross(otherParent);
    }

    bool operator>(const Phenotype& op) const {
        return fitness_ < op.fitness_; // We compare to sort in "strange" order
    }

    bool operator<(const Phenotype& op) const {
        return fitness_ > op.fitness_;
    }

protected:
    void calcFitness() {
        std::cout << "Phenotype::calcFitness\n";
    }

    double fitness_ = -1234.987554;
};

#endif // PHENOTYPE_H
