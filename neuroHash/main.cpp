#include "src/evolution/genetictrainer.h"
#include "src/evolution/testphenotype.h"
#include "src/evolution/testphenotypediofant.h"
#include "src/evolution/trainerphenotype.h"
#include "../NeuroGeneticHash/NeuroGeneticHash/Simple2x2x1Perceptron.h"
#include "../NeuroGeneticHash/NeuroGeneticHash/MaxEquation.h"
#include "../NeuroGeneticHash/NeuroGeneticHash/PerceptronHasher.h"

#include "../NeuroGeneticHash/NeuroGeneticHash/MultilayerPerceptron.h"
#include "../NeuroGeneticHash/NeuroGeneticHash/AnnHasher.h"

int main()
{
    auto best = GeneticTrainer<AnnHasher<MultilayerPerceptron> >::survivalOfTheFittest(
                10000, 10000, 4, 500, 1.0, 0.5, 0.3, 1.0 + 1e-6, 10, 86, true, 1.0);

	auto hash = best.ann_.getHashFunc();

	std::cout << "Result: " << best.serialize() << std::endl;

	std::cout << "Fitness: " << best.fitness() << std::endl;
	std::cout << "Collisions: " << HashTester::collisionTester(hash, 0xFFFFFF) << "\n";
	
	for (int i = 2; i < 19; i++) {
		std::cout << "Avalanche " << i << ": " << HashTester::avalancheTester(hash, 1 << i) << "\n";
	}
		
    return 0;
}
