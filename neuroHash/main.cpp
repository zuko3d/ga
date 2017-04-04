#include "src/evolution/genetictrainer.h"
#include "src/evolution/testphenotype.h"
#include "src/evolution/testphenotypediofant.h"
#include "src/evolution/trainerphenotype.h"

int main()
{
    auto best = GeneticTrainer<TrainerPhenotype>::survivalOfTheFittest(10000, 1000000, 4, 100, 0.85, 0.6, 0.1, 1.02, 5, 100, true);

    std::cout << "Fitness: " << best.fitness() << std::endl;
    std::cout << "Result: " << best.serialize() << std::endl;
    return 0;
}
