#include "src/evolution/genetictrainer.h"
#include "src/evolution/testphenotype.h"
#include "src/evolution/testphenotypediofant.h"
#include "src/evolution/trainerphenotype.h"
#include "../NeuroGeneticHash/NeuroGeneticHash/Simple2x2x1Perceptron.h"

#include <QDebug>

int main()
{
    auto best = GeneticTrainer<Simple2x2x1Perceptron>::survivalOfTheFittest(
                10000, 100000, 4, 10000, 0.75, 0.25, 0.2, 1.0000001, 20, 86, true, 1e4);

    best.simulate();

    std::cout << "Fitness: " << best.fitness() << std::endl;
    std::cout << "Result: " << best.serialize() << std::endl;
    return 0;
}
