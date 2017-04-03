#include "trainerphenotype.h"

#include "testphenotypediofant.h"

const int kTotalPop = 200;

TrainerPhenotype::TrainerPhenotype()
{
    nVillages_ = 1 + hrand() % kTotalPop;
    if(nVillages_ > kTotalPop) nVillages_ = kTotalPop;
    villageSize_ = kTotalPop / nVillages_;
    mutaFactor_ = hRnd();
    crossFactor_ = hRnd();
    topX_ = hRnd();

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
    if(hrand() % 2) ret.mutaFactor_ = otherParent.mutaFactor_;
    if(hrand() % 2) ret.crossFactor_ = otherParent.crossFactor_;
    if(hrand() % 2) ret.topX_ = otherParent.topX_;

    ret.calcFitness();
    return ret;
}

void TrainerPhenotype::calcFitness()
{
    fitness_ = 1.0;
    for(int i = 0; i < 5; i++) {
        auto result = GeneticTrainer<TestPhenotypeDiofant>::survivalOfTheFittest(
                    10000, 200, nVillages_, villageSize_, mutaFactor_, crossFactor_, topX_).fitness();

        if(fitness_ > result) fitness_ = result;
    }
}
