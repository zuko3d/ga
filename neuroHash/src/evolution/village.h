#ifndef VILLAGE_H
#define VILLAGE_H

#include "../global.h"

#include <vector>
#include <algorithm>

const int kEpochsToProtectInnovations = 5;

template <class Creature>
class Village : public std::vector<Creature>{
public:
    void sort(){
        if(!sorted){
            std::sort(this->begin(), this->end());
            sorted = true;
        }
    }

    int innovationsProtected = kEpochsToProtectInnovations;

    void push_back (const Creature& val) {
        if(std::vector<Creature>::size() > 0) {
          if(val.fitness() > std::vector<Creature>::back().fitness()) {
              sorted = false;
          }
        } else {
              sorted = true;
        }
        std::vector<Creature>::push_back(val);
    }

    Creature front() const {
        if(std::vector<Creature>::size() == 0) {
            std::cout << "vector is empty!\n";
            assert(false);
        }

        return std::vector<Creature>::front();
    }

    Creature back() const {
        if(std::vector<Creature>::size() == 0) {
            std::cout << "vector is empty!\n";
            assert(false);
        }

        return std::vector<Creature>::back();
    }

    bool sorted = false;
	double prevFitness = 0.0;
};

#endif // VILLAGE_H
