#include "testphenotype.h"

#include <cmath>

TestPhenotype::TestPhenotype()
{
    x_ = static_cast<double>(hrand()) * hRnd();

    calcFitness();
}

TestPhenotype TestPhenotype::mutate()
{
    TestPhenotype ret(*this);
    ret.x_ /= hRnd();
    ret.x_ *= hRnd();

    ret.calcFitness();
    return ret;
}

TestPhenotype TestPhenotype::cross(const TestPhenotype &otherParent) const
{
    TestPhenotype ret(*this);
    ret.x_ += otherParent.x_;
    ret.x_ /= 2;

    ret.calcFitness();
    return ret;
}

void TestPhenotype::calcFitness()
{
    fitness_ = 1.0 / fabs(10.0 - x_);
}
