#include "trainerphenotype.h"

#include "src/neuralnetwork/Simple2x2x1Perceptron.h"

const int kTotalPop = 100;

TrainerPhenotype::TrainerPhenotype()
{
    nVillages_ = 1 + hrand() % kTotalPop;
    if(nVillages_ > kTotalPop) nVillages_ = kTotalPop;
    villageSize_ = kTotalPop / nVillages_;
    mutaFactor_ = hRnd();
    crossFactor_ = hRnd();
    topX_ = hRnd() / (1.0 + mutaFactor_ + crossFactor_);

    calcFitness();
}

TrainerPhenotype TrainerPhenotype::mutate()
{
    TrainerPhenotype ret(*this);

    ret.nVillages_ += hrand() % 11 - 5;
    if(ret.nVillages_ > kTotalPop) ret.nVillages_ = kTotalPop;
    if(ret.nVillages_ < 1) ret.nVillages_ = 1;

    ret.villageSize_ = kTotalPop / ret.nVillages_;
    if(ret.villageSize_ < 2) ret.villageSize_ = 2;
    if(ret.villageSize_ > kTotalPop) ret.villageSize_ = kTotalPop;

    ret.calcFitness();
    return ret;
}

TrainerPhenotype TrainerPhenotype::cross(const TrainerPhenotype &otherParent) const
{
    TrainerPhenotype ret(*this);

    if(hrand() % 2) ret.nVillages_ = otherParent.nVillages_;
    if(ret.nVillages_ > kTotalPop) ret.nVillages_ = kTotalPop;
    ret.villageSize_ = kTotalPop / ret.nVillages_;
	if (ret.villageSize_ < 2) ret.villageSize_ = 2;

    if(hrand() % 2) ret.mutaFactor_ = otherParent.mutaFactor_;
    if(hrand() % 2) ret.crossFactor_ = otherParent.crossFactor_;
    if(hrand() % 2) ret.topX_ = otherParent.topX_;

	double maxTopX_ = 1.0 / (1.1 + ret.mutaFactor_ + ret.crossFactor_);
	if (ret.topX_ > maxTopX_) ret.topX_ = maxTopX_;

    ret.calcFitness();
    return ret;
}

void TrainerPhenotype::calcFitness()
{
    fitness_ = 1.0;
    for(int i = 0; i < 40; i++) {
        auto result = GeneticTrainer<Simple2x2x1Perceptron>::survivalOfTheFittest(
                    10000, 400, nVillages_, villageSize_, mutaFactor_, crossFactor_, topX_);

        if(fitness_ > result.fitness()) fitness_ = result.fitness();
    }
}
