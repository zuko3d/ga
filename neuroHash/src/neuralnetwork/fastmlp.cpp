#include "fastmlp.h"

FastMLP::FastMLP(std::vector<size_t> layers)
{
    blayers_.resize(layers.size());
    /*l
    GlobalStatistics::primes_.resize(order_);

    blayers_[0] = 0;

    for (size_t i = 0; i < layers.size() - 1; i++) {
        blayers_[i + 1] = blayers_[i] + layers[i];

    }

    barriers_.reserve(blayers_.back());

    for (auto& b : barriers_) {
        b = GlobalStatistics::primes_[ hrand() % GlobalStatistics::primes_.size() ];
    }

    for(int i = blayers_[blayers_.size() - 2]; i < blayers_.back(); i++){
        barriers_[i] = 0xFFFFFFFF - 4; // the nearest prime number to MAX_UINT32
    }

    weights_.resize(barriers_.size() - 1);
    for (size_t i = 0; i < weights_.size(); i++) {
        weights_[i].resize(barriers_[i].size());
        for (auto& w : weights_[i]) {
            w.resize(barriers_[i + 1].size());
            for (auto& weight : w) {
                //weight = hrand() & order_;
                weight = GlobalStatistics::primes_[ hrand() % GlobalStatistics::primes_.size() ];
            }
        }
    }
    */
}
