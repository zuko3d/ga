#include "src/evolution/genetictrainer.h"
#include "src/evolution/testphenotype.h"
#include "src/evolution/testphenotypediofant.h"
#include "src/evolution/trainerphenotype.h"
#include "../NeuroGeneticHash/NeuroGeneticHash/Simple2x2x1Perceptron.h"
#include "../NeuroGeneticHash/NeuroGeneticHash/MaxEquation.h"
#include "../NeuroGeneticHash/NeuroGeneticHash/PerceptronHasher.h"

#include "../NeuroGeneticHash/NeuroGeneticHash/MultilayerPerceptron.h"
#include "../NeuroGeneticHash/NeuroGeneticHash/AnnHasher.h"

#include "../blake/blake2.h"

std::string blake512(uint32_t in) {
	std::string ret;
	ret.resize(64);

	blake2b(&ret[0], 64, &in, sizeof(in), "key", 3);

	return ret;
}

int main()
{
//    HashTester::overallTest(blake512);
//    return 0;

    auto best = GeneticTrainer<AnnHasher<MultilayerPerceptron> >::survivalOfTheFittest(
                10000, 100000, 800, 10, 0.5, 0.5, 0.1, 1.0 + 1e-15, 10, 86, false, 1.0);

	//std::cout << "Result: " << best.serialize() << std::endl;

	std::cout << "Fitness: " << best.fitness() << std::endl;

	HashTester::overallTest(best.ann_.getHashFunc());
    return 0;
}
