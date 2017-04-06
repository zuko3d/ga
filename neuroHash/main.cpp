#include "src/evolution/genetictrainer.h"
#include "src/evolution/testphenotype.h"
#include "src/evolution/testphenotypediofant.h"
#include "src/evolution/trainerphenotype.h"
#include "../NeuroGeneticHash/NeuroGeneticHash/Simple2x2x1Perceptron.h"

#include <QDebug>

int main()
{
    auto best = GeneticTrainer<Simple2x2x1Perceptron>::survivalOfTheFittest(
                10000, 100000, 10, 100000, 0.5, 0.5, 0.2, 1.00002, 20, 86, true);

    std::cout << "Fitness: " << best.fitness() << std::endl;
    std::cout << "Result: " << best.serialize() << std::endl;
    return 0;
}
