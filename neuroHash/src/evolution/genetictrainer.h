#ifndef GENETICTRAINER_H
#define GENETICTRAINER_H

#include "../global.h"

#include "phenotype.h"
#include "village.h"

#include <chrono>
#include <thread>
#include <future>
#include <vector>
#include <cmath>

template <class Creature>
class GeneticTrainer
{
public:
	static Creature survivalOfTheFittest(
		const int maxEpochs,
		const int64_t maxTimeMilliseconds,
		const size_t nVillages,
		const size_t villageSize,
		const double mutaFactor = 0.75,
		const double crossFactor = 0.85,
		const double topX = 0.7,
		const double stagnationCoef = 1.02,
		const int innovationsProtectedEpochs = 5,
		const int outputLevel = 0, // 0 for nothing, 100 for everything
		const bool threaded = false
	) {
		size_t maxPopulation = villageSize * 4;
        std::vector<Village<Creature> > villages;
        villages.resize((size_t)nVillages);

		if (outputLevel > 20) {
			std::cout << __FUNCTION__ << " Generating initial population..." << std::endl;
		}

        for(auto& village: villages) {
            village.resize(villageSize);
        }

        auto startTime = std::chrono::system_clock::now();

        int epoch = 0;
        for(epoch = 0; epoch < maxEpochs; epoch++) {
			auto elapsedMillis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count();

			if (outputLevel > 80) {
				std::cout << __FUNCTION__ << "Epoch: " << epoch << ", time elapsed: " << elapsedMillis << std::endl;
			}

            if(elapsedMillis >= maxTimeMilliseconds) {
                if(epoch >= innovationsProtectedEpochs * 2) {
					if (outputLevel > 30) {
						std::cout << __FUNCTION__ << elapsedMillis << " msec elapsed, exiting training\n"; std::cout.flush();
					}
					break;
                }
            }

            std::vector<std::thread> threads;
            for(auto& village: villages){
				if (threaded) {
					threads.push_back(std::thread(handleVillage, std::ref(village), mutaFactor, crossFactor, topX, maxPopulation));
				}
				else {
					handleVillage(village, mutaFactor, crossFactor, topX, maxPopulation);
				}
            }

            for(auto& _thread: threads){
                _thread.join();
            }

            size_t stagnatedVillages = 0;
            for(auto& village: villages){
                double nearBest = -1.0;
                if(village.size() > 0) {
					village.sort();
					nearBest = village[std::min(villageSize / 2, village.size() - 1)].fitness();
					if (outputLevel > 86) {
						std::cout << "village\n\tnear-best = " << nearBest;
						std::cout << "\n\tbest = " << village[0].fitness();
						std::cout << "\n\tsize = " << village.size();
						std::cout << "\n\tinnovationsProtected = " << village.innovationsProtected;
						std::cout << std::endl;
					}
                }
				if (nearBest < village.prevFitness * stagnationCoef) {
					if(!village.innovationsProtected) stagnatedVillages++;
					if ((village.size() < villageSize)  && (outputLevel > 95)) {
						std::cout << "Appending " << villageSize - village.size() << " values" << std::endl;
					}
					while(village.size() < villageSize) {
						auto& vlg = villages[hrand() % nVillages];
						village.push_back(vlg[(size_t)(topX * hRnd() * ((double)vlg.size()))]);
					}
					village.innovationsProtected = 1;
				}
				village.prevFitness = std::max(village.prevFitness, nearBest);
            }

			if (outputLevel > 85) {
				std::cout << __FUNCTION__ << " stagnatedVillages = " << stagnatedVillages << std::endl;
			}
            if(stagnatedVillages >= nVillages) {
				if (outputLevel > 30) {
					std::cout << "All villages stagnated, exiting\n"; std::cout.flush();
				}
                break;
            }
        }

		auto best = villages[0].front();
        for(auto& village: villages){
			if (!village.empty()) {
				auto _best = village.front();
				if (!village.sorted) {
					for (auto& creature : village) {
						if (creature.fitness() > _best.fitness()) _best = creature;
					}
				}
				if (_best.fitness() > best.fitness()) best = _best;
			}
        }


		if (outputLevel > 20) {
			std::cout << "Time for training: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count() << " msec\n";
			std::cout << "Total epochs done: " << epoch << std::endl;
			std::cout << "Best creature: " << best.serialize() << std::endl;
			std::cout << "Fintess: " << best.fitness() << std::endl;
		}

        return best;
    }

private:
    static void handleVillage(
            Village<Creature>& village,
            double mutationFactor,
            double crossFactor,
            double topX,
			size_t maxPopulation
           );
};

template <class Creature>
void GeneticTrainer<Creature>::handleVillage(Village<Creature> &village, double mutationFactor, double crossFactor, double topX, size_t maxPopulation)
{
    village.sort();
	if (village.size() > maxPopulation) { // we've grown too big
		village.innovationsProtected = 0;
	}

    if(village.innovationsProtected){
        village.innovationsProtected--;
    } else {
      size_t newSize = static_cast<size_t>(static_cast<double>(village.size()) * topX);
      if (newSize < 1) newSize = 1;
	  if (newSize > maxPopulation) newSize = maxPopulation;
      village.resize(newSize);
    }

    village.reserve(((size_t) (((double) village.size()) * (1.1 + mutationFactor + crossFactor))));

    auto initialSize = village.size();
    for(size_t i = 0; i < initialSize; i++){
        if(hRnd() < mutationFactor){
            village.push_back(village[i].mutate());
        }

        if(hRnd() < crossFactor) {
            village.push_back(village[i] + village[hrand() % initialSize]);
        }
    }
}

#endif // GENETICTRAINER_H
