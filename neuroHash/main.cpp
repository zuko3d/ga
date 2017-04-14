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
                10000, 100000, 8, 1000, 0.25, 0.15, 0.4, 1.00000000001, 10, 86, true, 1);

    //best.simulate();

    std::cout << "Fitness: " << best.fitness() << std::endl;
    std::cout << "Result: " << best.serialize() << std::endl;
    return 0;
}
