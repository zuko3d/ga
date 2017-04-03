#include "trainerphenotype.h"

#include "testphenotypediofant.h"

TrainerPhenotype::TrainerPhenotype()
{
    nVillages_ = 1 + hrand() % 200;
    villageSize_ = nVillages_ + 1 + hrand() % 500;
    mutaFactor_ = hRnd();
    crossFactor_ = hRnd();
    topX_ = hRnd();

    calcFitness();
}

TrainerPhenotype TrainerPhenotype::mutate()
{
    TrainerPhenotype ret(*this);

    ret.nVillages_ += hrand() % 10 - 5;
    if(ret.nVillages_ < 1) ret.nVillages_ = 1;

    ret.villageSize_ += hrand() % 200 - 100;
    if(ret.villageSize_ < 10) ret.villageSize_ = 10;
    if(ret.villageSize_ > 1000) ret.villageSize_ = 1000;

    ret.calcFitness();
    return ret;
}

TrainerPhenotype TrainerPhenotype::cross(const TrainerPhenotype &otherParent) const
{
    TrainerPhenotype ret(*this);

    if(hrand() % 2) ret.nVillages_ = otherParent.nVillages_;
    if(hrand() % 2) ret.villageSize_ = otherParent.villageSize_;
    if(hrand() % 2) ret.mutaFactor_ = otherParent.mutaFactor_;
    if(hrand() % 2) ret.crossFactor_ = otherParent.crossFactor_;
    if(hrand() % 2) ret.topX_ = otherParent.topX_;

    ret.calcFitness();
    return ret;
}

void TrainerPhenotype::calcFitness()
{
    auto best = GeneticTrainer<TestPhenotypeDiofant>::survivalOfTheFittest(
                    10000, 500, nVillages_, villageSize_, mutaFactor_, crossFactor_, topX_);
    fitness_ = best.fitness();
}
