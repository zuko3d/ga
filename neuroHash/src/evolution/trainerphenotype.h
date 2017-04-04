#ifndef TRAINERPHENOTYPE_H
#define TRAINERPHENOTYPE_H

#include "phenotype.h"

#include "genetictrainer.h"

class TrainerPhenotype : public Phenotype
{
public:
    TrainerPhenotype();

    double fitness() const { assert(fitness_ >= 0); return fitness_; }

    TrainerPhenotype mutate();

    TrainerPhenotype cross(const TrainerPhenotype &otherParent) const;

    std::string serialize() const { return  std::to_string(nVillages_) + ", " +
                                            std::to_string(villageSize_) + ", " +
                                            std::to_string(mutaFactor_) + ", " +
                                            std::to_string(crossFactor_) + ", " +
                                            std::to_string(topX_) ; }

    TrainerPhenotype operator+(const TrainerPhenotype& otherParent) const {
        return this->cross(otherParent);
    }
private:
    void calcFitness();

    size_t nVillages_;
	size_t villageSize_;
    double mutaFactor_;
    double crossFactor_;
    double topX_;
};

#endif // TRAINERPHENOTYPE_H
