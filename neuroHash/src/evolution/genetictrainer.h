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

#include <omp.h>

#include "globalstatistics.h"

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
        const bool threaded = false,
        const double enoughFitness = 1e10
	) {
		size_t maxPopulation = villageSize * 4;
        std::vector<Village<Creature> > villages;
        villages.resize((size_t)nVillages);

        if (outputLevel > 30) {
			std::cout << __FUNCTION__ << " Generating initial population..." << std::endl;
		}
		auto ts = std::chrono::system_clock::now();

        for(auto& village: villages) {
            village.resize(villageSize);
        }

        if (outputLevel > 31) {
			std::cout << " Generation took " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - ts).count()  << " msec" << std::endl;
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

#pragma omp parallel for
            for(size_t i = 0; i < villages.size(); i++) {
                handleVillage(villages[i], mutaFactor, crossFactor, topX, maxPopulation);
            }

/*
            for(auto& village: villages){
				if (threaded) {
					threads.push_back(std::thread(handleVillage, std::ref(village), mutaFactor, crossFactor, topX, maxPopulation));
				}
                else {
                    handleVillage(village, mutaFactor, crossFactor, topX, maxPopulation);
                }
            }
*/
            for(auto& _thread: threads){
                _thread.join();
            }

            size_t stagnatedVillages = 0;
			Creature bestCreature;
            for(auto& village: villages){
                double nearBest = -1.0;
                if(village.size() > 0) {
					village.sort();
					if (village[0].fitness() > bestCreature.fitness()) bestCreature = village[0];
                    nearBest = village[std::min(villageSize, village.size() - 1)].fitness();
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
					village.sort();
					size_t newSize = static_cast<size_t>(static_cast<double>(village.size()) * topX);
					if (newSize < 1) newSize = 1;
					if (newSize > maxPopulation) newSize = maxPopulation;
					village.resize(newSize);

					if ((village.size() < villageSize)  && (outputLevel > 95)) {
						std::cout << "Appending " << villageSize - village.size() << " values" << std::endl;
					}
					while(village.size() < villageSize) {
						if (hrand() % 2) {
							auto& vlg1 = villages[hrand() % nVillages];
							auto& vlg2 = villages[hrand() % nVillages];
							village.push_back(village[0].cross(vlg1[(size_t)(topX * hRnd() * ((double)vlg1.size()))], vlg2[(size_t)(topX * hRnd() * ((double)vlg2.size()))]));
						} else {
							village.push_back(Creature());
						}
					}
					village.innovationsProtected = innovationsProtectedEpochs;
				}
				village.prevFitness = std::max(village.prevFitness, nearBest);
            }
			if (outputLevel > 80) {
				std::cout << " bestScore = " << bestCreature.fitness() << std::endl;
				std::cout << " source = " << bestCreature.source << std::endl;

                std::cout << "Cross:  " << static_cast<double>(GlobalStatistics::positiveCross) / GlobalStatistics::totalCross << " / " << GlobalStatistics::totalCross << std::endl;
                std::cout << "Mutate: " << static_cast<double>(GlobalStatistics::positiveMutations) / GlobalStatistics::totalMutations << " / " << GlobalStatistics::totalMutations << std::endl;
			}
			if (outputLevel > 85) {
				std::cout << __FUNCTION__ << " stagnatedVillages = " << stagnatedVillages << std::endl;
			}
            stagnatedVillages = 0;
            if(stagnatedVillages >= nVillages) {
				if (outputLevel > 30) {
					std::cout << "All villages stagnated, exiting\n"; std::cout.flush();
				}
                break;
            }
            if(bestCreature.fitness() >= enoughFitness) {
                if (outputLevel > 30) {
                    std::cout << "Fitness is enough, exiting\n"; std::cout.flush();
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
            //std::cout << "Total epochs done: " << epoch << std::endl;
            //std::cout << "Best creature: " << best.serialize() << std::endl;
			std::cout << "Fintess: " << best.fitness() << std::endl;

            std::cout << "Cross: " << static_cast<double>(GlobalStatistics::positiveCross) / GlobalStatistics::totalCross << " / " << GlobalStatistics::totalCross << std::endl;
            std::cout << "Mutate: " << static_cast<double>(GlobalStatistics::positiveMutations) / GlobalStatistics::totalMutations << " / " << GlobalStatistics::totalMutations << std::endl;
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

    size_t newSize = std::max(maxPopulation / 4, village.size() * 2);

    while(village.size() < newSize) {
        int i = hrand() % village.size();
        if(hRnd() < mutationFactor){
            village.push_back(village[i].mutate());
        }

        if(hRnd() < crossFactor) {
            village.push_back(village[i].cross(village[hrand() % (village.size())], village[hrand() % (village.size())]));
        }
    }
    /*
    auto initialSize = village.size();
    for(size_t i = 0; i < initialSize; i++){
        if(hRnd() < mutationFactor){
            village.push_back(village[i].mutate());
        }

        if(hRnd() < crossFactor) {
            village.push_back(village[i].cross(village[hrand() % initialSize], village[hrand() % initialSize]));
        }
    }
    */
}

#endif // GENETICTRAINER_H
