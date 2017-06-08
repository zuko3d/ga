#include "MaxEquation.h"

MaxEquation::MaxEquation()
{
	static std::mt19937 generator;
	std::normal_distribution<double> distribution(0.0, 10.0);

	weights.resize(8);
	for (auto& w : weights) {
		w = distribution(generator);
	}

	calcFitness();
	source = 0;
}


MaxEquation::~MaxEquation()
{
}

MaxEquation MaxEquation::mutate() const
{
	MaxEquation ret(*this);

	static std::mt19937 generator;
	std::normal_distribution<double> distribution(0.0, 1.0);

	ret.weights[hrand() % weights.size()] += distribution(generator);

	ret.calcFitness();
	ret.source = 2;
	return ret;
}

MaxEquation MaxEquation::cross(const MaxEquation & p1, const MaxEquation & p2) const
{
	MaxEquation ret(*this);

	typedef unsigned char uchar;
	uchar* allele1, *allele2, *allele3, *resultAllele;
	allele1 = reinterpret_cast<uchar*>(const_cast<double*>(weights.data()));
	allele2 = reinterpret_cast<uchar*>(const_cast<double*>(p1.weights.data()));
	allele3 = reinterpret_cast<uchar*>(const_cast<double*>(p2.weights.data()));

	resultAllele = reinterpret_cast<uchar*>(const_cast<double*>(ret.weights.data()));

    for (size_t i = 0; i < weights.size() * sizeof(double) / sizeof(uchar); i++) {
//		auto result = ((*allele1) | (*allele2)) &
//			((*allele1) | (*allele3)) &
//			((*allele2) | (*allele3));

		*resultAllele = ((*allele1) | (*allele2)) &
			((*allele1) | (*allele3)) &
			((*allele2) | (*allele3));

		allele1++;
		allele2++;
		allele3++;
		resultAllele++;
	}

	ret.calcFitness();
	ret.source = 1;
	return ret;
}

void MaxEquation::calcFitness()
{
	/*
	v = {x1*(x5 - x2) + x2*(x7 - x4) + x3*x6 - x4*x5 ==
	0, -x1*x6 - x4*x6 + x5*(x5 - x8) + 2*x5*x7 == 0,
	x1*x4 - 2*x2*x3 + x3*(x5 - x8) - x4*x4 ==
	0, -x2*x7 - x3*x5 + x4*(x5 - x8) + x7*(x5 + x8) == 0}
	*/
	auto err1 = weights[0] * (weights[4] - weights[1]) + weights[1] * (weights[6] - weights[3]) + weights[2] * weights[5] - weights[3] * weights[4];
	auto err2 = -weights[0] * weights[5] - weights[3] * weights[5] + weights[4] * (weights[4] - weights[7]) + 2 * weights[4] * weights[6];
	auto err3 = weights[0] * weights[3] - 2 * weights[1] * weights[2] + weights[2] * (weights[4] - weights[7]) - weights[3] * weights[3];
	auto err4 = -weights[1] * weights[6] - weights[2] * weights[4] + weights[3] * (weights[4] - weights[7]) + weights[6] * (weights[4] + weights[7]);

	fitness_ = 1.0 / sqrt(err1 * err1 + err2 * err2 + err3 * err3 + err4 * err4);
}
