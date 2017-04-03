#include "src/evolution/genetictrainer.h"
#include "src/evolution/testphenotype.h"
#include "src/evolution/testphenotypediofant.h"
#include "src/evolution/trainerphenotype.h"

int main()
{
    auto best = GeneticTrainer<TrainerPhenotype>::survivalOfTheFittest(10000, 60000, 3, 30);

    std::cout << "Fitness: " << best.fitness() << std::endl;
    std::cout << "Result: " << best.serialize() << std::endl;
    return 0;
}
