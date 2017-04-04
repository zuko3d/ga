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
    if (!(hrand() % 4)) ret.x_ *= (1 + hRnd()) / (1.0 + hRnd());
	if (!(hrand() % 4)) ret.y_ *= (1 + hRnd()) / (1.0 + hRnd());
	if (!(hrand() % 4)) ret.a_ *= (1 + hRnd()) / (1.0 + hRnd());
	if (!(hrand() % 4)) ret.b_ *= (1 + hRnd()) / (1.0 + hRnd());
	if (!(hrand() % 4)) ret.c_ *= (1 + hRnd()) / (1.0 + hRnd());
	if (!(hrand() % 4)) ret.d_ *= (1 + hRnd()) / (1.0 + hRnd());
    

    ret.calcFitness();
    return ret;
}

TestPhenotypeDiofant TestPhenotypeDiofant::cross(const TestPhenotypeDiofant &otherParent) const
{
    TestPhenotypeDiofant ret(*this);
	if (hrand() % 2) ret.x_ = otherParent.x_;
	if (hrand() % 2) ret.y_ = otherParent.y_;
	if (hrand() % 2) ret.a_ = otherParent.a_;
	if (hrand() % 2) ret.b_ = otherParent.b_;
	if (hrand() % 2) ret.c_ = otherParent.c_;
	if (hrand() % 2) ret.d_ = otherParent.d_;

    ret.calcFitness();
    return ret;
}

void TestPhenotypeDiofant::calcFitness()
{
    fitness_ = 1.0 / (1.0 + fabs(456. * x_ + 7685. * y_ + 9876. * a_ + 56. * b_ + 3645. * c_ + 43655. * d_));
	if (fitness_ < 0) {
		std::cout << __FUNCTION__ << serialize() << std::endl;
		assert(fitness_ >= 0);
	}
}
