#include "src/evolution/genetictrainer.h"
#include "src/evolution/testphenotype.h"
#include "src/evolution/testphenotypediofant.h"
#include "src/evolution/trainerphenotype.h"
#include "../NeuroGeneticHash/NeuroGeneticHash/Simple2x2x1Perceptron.h"
#include "../NeuroGeneticHash/NeuroGeneticHash/MaxEquation.h"

int main()
{
    auto best = GeneticTrainer<MaxEquation>::survivalOfTheFittest(
                10000, 100000, 4, 10000, 0.5, 0.5, 0.4, 1.0000001, 20, 86, true, 1e5);

    //best.simulate();

    std::cout << "Fitness: " << best.fitness() << std::endl;
    std::cout << "Result: " << best.serialize() << std::endl;
    return 0;
}
