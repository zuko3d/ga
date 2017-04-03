#ifndef GENETICTRAINER_H
#define GENETICTRAINER_H

#include "../global.h"

#include "phenotype.h"
#include "village.h"

#include <chrono>
#include <thread>
#include <future>
#include <vector>

template <class Creature>
class GeneticTrainer
{
public:
    static Creature survivalOfTheFittest(
            int maxEpochs,
            int64_t maxTimeMilliseconds,
            int nVillages,
            int villageSize,
            double mutaFactor = 0.75,
            double crossFactor = 0.85,
            double topX = 0.7,
            double stagnationCoef = 1.01,
            int innovationsProtectedEpochs = 5
            ) {
            std::vector<Village<Creature> > villages;
            villages.resize(nVillages);

            for(auto& village: villages) {
                village.resize(villageSize);
            }

            auto startTime = std::chrono::system_clock::now();

            int epoch = 0;
            for(epoch = 0; epoch < maxEpochs; epoch++) {
                if(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count() >= maxTimeMilliseconds) {
                    //std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count() << " msec elapsed, exiting trainig\n";
                    if(epoch >= innovationsProtectedEpochs * 2) {
                      break;
                    }
                }

                std::vector<std::thread> threads;
                for(auto& village: villages){
                    //threads.push_back(std::thread(handleVillage, std::ref(village), mutaFactor, crossFactor, topX));
                    handleVillage(village, mutaFactor, crossFactor, topX);
                }

                for(auto& _thread: threads){
                    _thread.join();
                }

                int stagnatedVillages = 0;
                for(auto& village: villages){
                    if(village.size() < villageSize) {
                        for(auto& vlg: villages) {
                            village.push_back(vlg[(int) (topX * hRnd() * ((double) vlg.size()))]);
                        }
                        village.innovationsProtected = innovationsProtectedEpochs;
                    }

                    double best = -1.0, worst = 1.0;
                    if(village.size() > 0) {
                      best = village.front().fitness();
                      worst = village.back().fitness();
                      if(!village.sorted){
                          for(auto& creature: village){
                              if(creature.fitness() > best) best = creature.fitness();
                              if(creature.fitness() < worst) worst = creature.fitness();
                          }
                      }
                    }
                    if((best / worst) < stagnationCoef) {
                        stagnatedVillages++;
                        for(auto& vlg: villages) {
                            if(vlg.size() > 0) {
                              village.push_back(vlg[(int) (topX * hRnd() * ((double) vlg.size()))]);
                            }
                        }
                        village.innovationsProtected = innovationsProtectedEpochs;
                    }
                }


                if(stagnatedVillages >= nVillages) {
                    std::cout << "All villages stagnated, exiting\n";
                    break;
                }
            }

            auto best = villages[0].front();
            for(auto& village: villages){
                auto _best = village.front();
                if(!village.sorted){
                    for(auto& creature: village){
                        if(creature.fitness() > _best.fitness()) _best = creature;
                    }
                }
                if(_best.fitness() > best.fitness()) best = _best;
            }


            /*
            std::cout << "Total epochs done: " << epoch << std::endl;
            std::cout << "Best creature: " << best.serialize() << std::endl;
            std::cout << "Fintess: " << best.fitness() << std::endl;
            */

            return best;
        }

private:
    static void handleVillage(
            Village<Creature>& village,
            double mutationFactor,
            double crossFactor,
            double topX
           );
};

template <class Creature>
void GeneticTrainer<Creature>::handleVillage(Village<Creature> &village, double mutationFactor, double crossFactor, double topX)
{
    village.sort();

    if(village.innovationsProtected){
        village.innovationsProtected--;
    } else {
      size_t newSize = static_cast<size_t>(static_cast<double>(village.size()) * topX);
      if(newSize < 1) newSize  =1;
      village.resize(newSize);
    }

    village.reserve(village.size() * (1.1 + mutationFactor + crossFactor));

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
