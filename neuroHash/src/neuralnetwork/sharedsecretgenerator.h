#ifndef SHAREDSECRETGENERATOR_H
#define SHAREDSECRETGENERATOR_H

#include "../global/global.h"

#include "MultilayerPerceptron.h"

#include <vector>

typedef MultilayerPerceptron Ann;

class SharedSecretGenerator
{
public:
    SharedSecretGenerator(
            std::vector<size_t> keyLayers = {1, 2, 1} ,
            std::vector<size_t> secLayers = {2, 2, 1}
            );

    SharedSecretGenerator(
            const Ann& _keygen,
            const Ann& _secgen
            );


    double fitness() const {
        return fitness_;
    }

    SharedSecretGenerator  mutate() const;

    SharedSecretGenerator  cross(const SharedSecretGenerator  & p1, const SharedSecretGenerator  & p2) const;

    std::string serialize() const {
        return "keygen_: \n" + keygen_.serialize() + "\nsecgen_: \n" + secgen_.serialize();
    }

    bool operator>(const SharedSecretGenerator & op) const {
        return fitness_ < op.fitness_; // We compare to sort in "strange" order
    }

    bool operator<(const SharedSecretGenerator & op) const {
        return fitness_ > op.fitness_;
    }

    uint32_t calcOut(const std::vector<uint32_t> &x1, std::vector<uint32_t> x2);

    int source = -1;

// protected:
    void calcFitness();

    double fitness_;

    Ann keygen_;
    Ann secgen_;
};

#endif // SHAREDSECRETGENERATOR_H
