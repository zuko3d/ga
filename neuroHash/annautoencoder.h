#pragma once

#include "src/global.h"
#include <unordered_set>
#include <set>
#include "../NeuroGeneticHash/NeuroGeneticHash/HashTester.h"
#include "globalstatistics.h"

template<class Ann> class AnnAutoEncoder
{
public:
    AnnAutoEncoder<Ann>();
    ~AnnAutoEncoder<Ann>();

    double fitness() const {
        if (fitness_ < 0) {
            std::cout << __FUNCTION__ << " fitness_ = " << fitness_ << "\n";
            assert(fitness_ >= 0);
        }
        return fitness_;
    }

    AnnAutoEncoder<Ann> mutate() const;

    AnnAutoEncoder<Ann> cross(const AnnAutoEncoder<Ann> & p1, const AnnAutoEncoder<Ann> & p2) const;

    std::string serialize() const {
        return ann_.serialize();
    }

    bool operator>(const AnnAutoEncoder<Ann>& op) const {
        return fitness_ < op.fitness_; // We compare to sort in "strange" order
    }

    bool operator<(const AnnAutoEncoder<Ann>& op) const {
        return fitness_ > op.fitness_;
    }

    int source = -1;

//protected:
    void calcFitness();
    double fitness_;

    Ann ann_;
};

#include "../NeuroGeneticHash/NeuroGeneticHash/MultilayerPerceptron.h"

template<class Ann>
AnnAutoEncoder<Ann>::AnnAutoEncoder() : ann_({ 2, 3, 2, 1 })
{
    calcFitness();
    source = 0;
}

template<class Ann>
AnnAutoEncoder<Ann>::~AnnAutoEncoder()
{
}

template<class Ann>
AnnAutoEncoder<Ann> AnnAutoEncoder<Ann>::mutate() const
{
    AnnAutoEncoder<Ann> ret(*this);

    for(int i = 0; i < 2; i++){
        size_t layer = hrand() % (ret.ann_.barriers_.size() - 1);
        size_t neuron = hrand() % ret.ann_.barriers_[layer].size();

        ret.ann_.barriers_[layer][neuron] = GlobalStatistics::primes_[ hrand() % GlobalStatistics::primes_.size() ];
    }

    auto r = 6;
    for(int i = 0; i < r; i++){
        size_t layer = hrand() % (ret.ann_.weights_.size());
        size_t neuron = hrand() % ret.ann_.weights_[layer].size();
        size_t w = hrand() % ret.ann_.weights_[layer][neuron].size();

        ret.ann_.weights_[layer][neuron][w] = hrand(); // & ret.ann_.order_;
        //ret.ann_.weights_[layer][neuron][w] = GlobalStatistics::primes_[ hrand() % GlobalStatistics::primes_.size() ];
    }

    ret.source = 1;
    ret.calcFitness();

    #pragma omp atomic
    GlobalStatistics::totalMutations++;

    if(ret.fitness_ > fitness_) {
        #pragma omp atomic
        GlobalStatistics::positiveMutations++;
    }

    return ret;
}

template<class Ann>
AnnAutoEncoder<Ann> AnnAutoEncoder<Ann>::cross(const AnnAutoEncoder<Ann> & p1, const AnnAutoEncoder<Ann>& p2) const
{
    // TODO: don't copy! just resize it before usage!
    AnnAutoEncoder<Ann> ret(*this);

    for (size_t i = 0; i < ann_.barriers_.size(); i++) {
        for (size_t j = 0; j < ann_.barriers_[i].size(); j++) {
            auto& r1 = ann_.barriers_[i][j];
            auto& r2 = p1.ann_.barriers_[i][j];
            auto& r3 = p2.ann_.barriers_[i][j];
            ret.ann_.barriers_[i][j] = (r1 | r2) & (r1 | r3) & (r2 | r3);
        }
    }

    for (size_t i = 0; i < ann_.weights_.size(); i++) {
        for (size_t j = 0; j < ann_.weights_[i].size(); j++) {
            for (size_t k = 0; k < ann_.weights_[i][j].size(); k++) {
                auto& r1 = ann_.weights_[i][j][k];
                auto& r2 = p1.ann_.weights_[i][j][k];
                auto& r3 = p2.ann_.weights_[i][j][k];
                ret.ann_.weights_[i][j][k] = (r1 | r2) & (r1 | r3) & (r2 | r3);
            }
        }
    }

    ret.source = 2;
    ret.calcFitness();

    #pragma omp atomic
    GlobalStatistics::totalCross++;

    if(ret.fitness_ > fitness_) {
        if(ret.fitness_ > p1.fitness_) {
            if(ret.fitness_ > p2.fitness_) {
                #pragma omp atomic
                GlobalStatistics::positiveCross++;
            }
        }
    }

    return ret;
}

template<class Ann>
void AnnAutoEncoder<Ann>::calcFitness()
{
    double pts = 0.0;
    double total = 0.0;

    std::set<char> outs;

    const uint32_t sz = 8;

    uint32_t fails = 0;

    for(uint32_t i = 0; i < sz; i++) {

        for(uint32_t j = i + 1; j < sz; j++) {
            total++;
            auto out = static_cast<uint8_t>(ann_.calcOut({j, i})[0]);

            uint8_t res = out ^ static_cast<uint8_t>(ann_.calcOut({i, j})[0]);
            fails += __builtin_popcount(static_cast<uint32_t>(res));
            outs.insert(out);
        }
    }
    pts = 1.0 - static_cast<double>(fails) / 8.0 / total;
    fitness_ = pts * outs.size() / total;
}

