#ifndef VILLAGE_H
#define VILLAGE_H

#include "../global.h"

#include <vector>
#include <algorithm>

const int kEpochsToProtectInnovations = 20;

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
        if(val.fitness() > std::vector<Creature>::back().fitness()) {
            sorted = false;
        }
        std::vector<Creature>::push_back(val);
    }

    bool sorted = false;
};

#endif // VILLAGE_H
