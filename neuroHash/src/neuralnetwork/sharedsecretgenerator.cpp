#include "sharedsecretgenerator.h"

#include "../global/globalstatistics.h"

#include <set>

SharedSecretGenerator::SharedSecretGenerator(
        std::vector<size_t> keyLayers,
        std::vector<size_t> secLayers) :
    keygen_(keyLayers),
    secgen_(secLayers)
{
    calcFitness();
    source = 0;
}

SharedSecretGenerator::SharedSecretGenerator(
        const Ann &_keygen,
        const Ann &_secgen) :
    keygen_(_keygen),
    secgen_(_secgen)
{
    calcFitness();
    source = 0;
}

SharedSecretGenerator SharedSecretGenerator::mutate() const
{
    SharedSecretGenerator ret(*this);
    for(int i = 0; i < 4; i++){
        ret.keygen_.randomlyChangeWeight();
        ret.secgen_.randomlyChangeWeight();
    }

    ret.keygen_.randomlyChangeBias();
    ret.secgen_.randomlyChangeBias();

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

SharedSecretGenerator SharedSecretGenerator::cross(const SharedSecretGenerator &p1, const SharedSecretGenerator &p2) const
{
    SharedSecretGenerator ret(*this);

    for (size_t i = 0; i < keygen_.barriers_.size(); i++) {
        for (size_t j = 0; j < keygen_.barriers_[i].size(); j++) {
            auto& r1 = keygen_.barriers_[i][j];
            auto& r2 = p1.keygen_.barriers_[i][j];
            auto& r3 = p2.keygen_.barriers_[i][j];
            ret.keygen_.barriers_[i][j] = (r1 | r2) & (r1 | r3) & (r2 | r3);
        }
    }

    for (size_t i = 0; i < keygen_.weights_.size(); i++) {
        for (size_t j = 0; j < keygen_.weights_[i].size(); j++) {
            for (size_t k = 0; k < keygen_.weights_[i][j].size(); k++) {
                auto& r1 = keygen_.weights_[i][j][k];
                auto& r2 = p1.keygen_.weights_[i][j][k];
                auto& r3 = p2.keygen_.weights_[i][j][k];
                ret.keygen_.weights_[i][j][k] = (r1 | r2) & (r1 | r3) & (r2 | r3);
            }
        }
    }

    for (size_t i = 0; i < secgen_.barriers_.size(); i++) {
        for (size_t j = 0; j < secgen_.barriers_[i].size(); j++) {
            auto& r1 = secgen_.barriers_[i][j];
            auto& r2 = p1.secgen_.barriers_[i][j];
            auto& r3 = p2.secgen_.barriers_[i][j];
            ret.secgen_.barriers_[i][j] = (r1 | r2) & (r1 | r3) & (r2 | r3);
        }
    }

    for (size_t i = 0; i < secgen_.weights_.size(); i++) {
        for (size_t j = 0; j < secgen_.weights_[i].size(); j++) {
            for (size_t k = 0; k < secgen_.weights_[i][j].size(); k++) {
                auto& r1 = secgen_.weights_[i][j][k];
                auto& r2 = p1.secgen_.weights_[i][j][k];
                auto& r3 = p2.secgen_.weights_[i][j][k];
                ret.secgen_.weights_[i][j][k] = (r1 | r2) & (r1 | r3) & (r2 | r3);
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

uint32_t SharedSecretGenerator::calcOut(const std::vector<uint32_t>& x1, std::vector<uint32_t> x2)
{
    auto y1 = keygen_.calcOutUint32(x1);
    x2.push_back(y1);

    return secgen_.calcOutUint32(x2);
}

void SharedSecretGenerator::calcFitness()
{
    double pts = 0.0;
    double total = 0.0;

    const uint32_t sz = 11;

    uint32_t fails = 0;

    std::set<uint32_t> outs;

    for(uint32_t i = 0; i < sz; i++) {
        for(uint32_t j = i + 1; j < sz; j++) {
            total++;
            auto out =        calcOut({j}, {i});
            auto res = out ^ (calcOut({i}, {j}));
            fails += __builtin_popcount(res);

            outs.insert(out);
        }
    }
    pts = 1.0 - static_cast<double>(fails) / 32.0 / total;

    double unique = static_cast<double>(outs.size()) / total;

    if(outs.size() > 2 * sz) unique = 1.0;

    fitness_ = pts * unique;
    // fitness_ = pts;
}
