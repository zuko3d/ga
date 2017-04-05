#include "src/evolution/genetictrainer.h"
#include "src/evolution/testphenotype.h"
#include "src/evolution/testphenotypediofant.h"
#include "src/evolution/trainerphenotype.h"
#include "../NeuroGeneticHash/NeuroGeneticHash/Simple2x2x1Perceptron.h"

int main()
{
    auto best = GeneticTrainer<Simple2x2x1Perceptron>::survivalOfTheFittest(10000, 300000, 1000000, 10, 0.5, 0.5, 0.2, 1.00002, 20, 86, false);

    std::cout << "Fitness: " << best.fitness() << std::endl;
    std::cout << "Result: " << best.serialize() << std::endl;
    return 0;
}
