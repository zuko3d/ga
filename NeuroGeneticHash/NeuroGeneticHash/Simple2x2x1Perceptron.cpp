#include "Simple2x2x1Perceptron.h"
#include <QDebug>
std::vector<std::vector<double> > Simple2x2x1Perceptron::trainData = { {0,0,0}, {0,1,1}, {1, 0, 1}, {1, 1, 0} };

Simple2x2x1Perceptron::Simple2x2x1Perceptron()
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


Simple2x2x1Perceptron::~Simple2x2x1Perceptron()
{
}

Simple2x2x1Perceptron Simple2x2x1Perceptron::cross(const Simple2x2x1Perceptron & p1, const Simple2x2x1Perceptron & p2) const
{
	Simple2x2x1Perceptron ret(*this);
    /*
	for (int i = 0; i < weights.size(); i++) {
		ret.weights[i] = (weights[i] + p1.weights[i] + p2.weights[i]) / 3.0;
	}
    */

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

        *resultAllele =    ((*allele1) | (*allele2)) &
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

void Simple2x2x1Perceptron::calcFitness()
{
	double error = 1e-12;
	for (auto& data : trainData) {
		double n1 = costFunction(data[0] * weights[0] + data[1] * weights[2]);
		double n2 = costFunction(data[0] * weights[1] + data[1] * weights[3]);
		double out = costFunction(n1 * weights[4] + n2 * weights[5]);
		double err = data[2] - out;
		error += err * err;
	}

	fitness_ = 1.0 / std::sqrt(error);
}

double Simple2x2x1Perceptron::costFunction(double x)
{
	/*
	if (x > 10.0) return x;
	if (x < -10.0) return 0.0;
	return log(1.0 + exp(x));
	*/
	if (x > 10.0) return 1.0;
	if (x < -10.0) return 0.0;
	return 1.0 / (1.0 + exp(-x));
}

Simple2x2x1Perceptron Simple2x2x1Perceptron::mutate() const
{
	Simple2x2x1Perceptron ret(*this);

	static std::mt19937 generator;
    std::normal_distribution<double> distribution(0.0, 2.0);

	ret.weights[hrand() % weights.size()] += distribution(generator);
	
	ret.calcFitness();
	ret.source = 2;
	return ret;
}
