#include "testphenotypediofant.h"

#include <cmath>

TestPhenotypeDiofant::TestPhenotypeDiofant()
{
    x_ = hrand() - hrand() + hRnd();
    y_ = hrand() - hrand() + hRnd();
    a_ = hrand() - hrand() + hRnd();
    b_ = hrand() - hrand() + hRnd();
    c_ = hrand() - hrand() + hRnd();
    d_ = hrand() - hrand() + hRnd();

    calcFitness();
}

TestPhenotypeDiofant TestPhenotypeDiofant::mutate()
{
    TestPhenotypeDiofant ret(*this);
    if(hrand() % 2){
        ret.x_ *= hRnd() / hRnd();
    } else {
        ret.y_ *= hRnd() / hRnd();
    }
    if(hrand() % 2){
        ret.a_ *= hRnd() / hRnd();
    } else {
        ret.b_ *= hRnd() / hRnd();
    }
    if(hrand() % 2){
        ret.c_ *= hRnd() / hRnd();
    } else {
        ret.d_ *= hRnd() / hRnd();
    }

    ret.calcFitness();
    return ret;
}

TestPhenotypeDiofant TestPhenotypeDiofant::cross(const TestPhenotypeDiofant &otherParent) const
{
    TestPhenotypeDiofant ret(*this);
    ret.y_ = otherParent.y_;
    ret.b_ = otherParent.b_;
    ret.d_ = otherParent.d_;

    ret.calcFitness();
    return ret;
}

void TestPhenotypeDiofant::calcFitness()
{
    fitness_ = 1.0 / (1.0 + fabs(456 * x_ + 7685 * y_ + 9876 * a_ + 56 * b_ + 3645 * c_ + 43655 * d_));
}
