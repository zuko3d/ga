#include "PerceptronHasher.h"

PerceptronHasher::PerceptronHasher()
{
	static std::mt19937 generator;
	std::normal_distribution<double> distribution(0.0, 10.0);

	weights.resize(6);
	for (auto& w : weights) {
		w = distribution(generator);
	}

	calcFitness();
	source = 0;
}


PerceptronHasher::~PerceptronHasher()
{
}

PerceptronHasher PerceptronHasher::cross(const PerceptronHasher & p1, const PerceptronHasher & p2) const
{
	PerceptronHasher ret(*this);
	/*
	for (int i = 0; i < weights.size(); i++) {
	ret.weights[i] = (weights[i] + p1.weights[i] + p2.weights[i]) / 3.0;
	}
	*/
	typedef unsigned char uchar;
	uchar* allele1, *allele2, *allele3, *resultAllele;
	allele1 = reinterpret_cast<uchar*>(const_cast<double*>(weights.data()));
	allele2 = reinterpret_cast<uchar*>(const_cast<double*>(p1.weights.data()));
	allele3 = reinterpret_cast<uchar*>(const_cast<double*>(p2.weights.data()));

	resultAllele = reinterpret_cast<uchar*>(const_cast<double*>(ret.weights.data()));

	for (int i = 0; i < weights.size() * sizeof(double) / sizeof(uchar); i++) {
		//        qDebug() << QString::number( *allele1, 2 );
		//        qDebug() << QString::number( *allele2, 2 );
		//        qDebug() << QString::number( *allele3, 2 );

		auto result = ((*allele1) | (*allele2)) &
			((*allele1) | (*allele3)) &
			((*allele2) | (*allele3));

		//        qDebug() << QString::number( result, 2 );

		*resultAllele = ((*allele1) | (*allele2)) &
			((*allele1) | (*allele3)) &
			((*allele2) | (*allele3));

		allele1++;
		allele2++;
		allele3++;
		resultAllele++;
	}

	//    for(int i = 0;i < weights.size(); i++){
	//        qDebug() << weights[i];
	//        qDebug() << p1.weights[i];
	//        qDebug() << p2.weights[i];
	//        qDebug() << "-> "<< ret.weights[i];
	//    }

	ret.calcFitness();
	ret.source = 1;
	return ret;
}

double PerceptronHasher::calcOut(double x1, double x2) const {
	double n1 = costFunction(x1 * weights[0] + x2 * weights[2]);
	double n2 = costFunction(x1 * weights[1] + x2 * weights[3]);
	return costFunction(n1 * weights[4] + n2 * weights[5]);
}

void PerceptronHasher::calcFitness()
{
	uint64_t ret = 0;
	double num1 = hRnd();
	double num2 = hRnd();
	for (int i = 0; i < 100; i++) {
		double out1 = calcOut(num1, num2);
		num2 *= 2;

		double out2 = calcOut(num1, num2);

		uint64_t* u1, *u2;
		u1 = reinterpret_cast<uint64_t*>(&out1);
		u2 = reinterpret_cast<uint64_t*>(&out2);

		uint64_t res = (*u1) ^ (*u2);
		res = __popcnt64(res);
		if (res >= 32) {
			res -= 32;
		}
		else {
			res = 32 - res;
		}
		ret += res;
	}
	fitness_ = 1.0 / (1.0 + ret);
}

double PerceptronHasher::costFunction(double x) const
{
	if (x > 10.0) return x;
	if (x < -10.0) return 0.0;
	return log(1.0 + exp(x));

	/*
	if (x > 10.0) return 1.0;
	if (x < -10.0) return 0.0;
	return 1.0 / (1.0 + exp(-x));
	*/
}

PerceptronHasher PerceptronHasher::mutate() const
{
	PerceptronHasher ret(*this);

	static std::mt19937 generator;
	std::normal_distribution<double> distribution(0.0, 2.0);

	ret.weights[hrand() % weights.size()] += distribution(generator);

	ret.calcFitness();
	ret.source = 2;
	return ret;
}
