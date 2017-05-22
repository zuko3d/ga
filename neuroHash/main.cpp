#include "src/evolution/genetictrainer.h"
#include "src/evolution/testphenotype.h"
#include "src/evolution/testphenotypediofant.h"
#include "src/evolution/trainerphenotype.h"

#include "src/neuralnetwork/annautoencoder.h"
#include "blake/blake2.h"

#include <iomanip>

std::string blake512(uint32_t in) {
	std::string ret;
	ret.resize(64);

	blake2b(&ret[0], 64, &in, sizeof(in), "key", 3);

	return ret;
}

int main()
{
//    HashTester::overallTest(blake512);
//    return 0;

    std::cout << GlobalStatistics::primes_.size() << "\n";

    auto best = GeneticTrainer<AnnAutoEncoder<MultilayerPerceptron> >::survivalOfTheFittest(
                100000,   // max epochs
                38 * 3600 * 1000, // max time (msec)
                1000,   // villages
                10,    // village population
                1.0,    // mutation probability
                0.05,   // cross probability
                0.52,   // top X cut-off ("elitism" factor)
                1.1,    // stagnation coefficient
                10,     // innovationsProtectedEpochs
                86,     // output level
                0.99);   // target fitness

    std::cout << "best: " << best.serialize();
    std::cout << "Examples: " << std::endl;

    const uint32_t sz = 0xFF;

    uint32_t fails = 0;
    std::set<std::string> outs;
    double total = 0;

    for(uint32_t i = 0; i < sz; i++) {

        for(uint32_t j = i + 1; j < sz; j++) {
            auto out_ = best.ann_.calcOut({j, i});
            auto out = static_cast<uint8_t>(out_[0]);
            outs.insert(out_);
            total++;
            uint8_t res = out ^ static_cast<uint8_t>(best.ann_.calcOut({i, j})[0]);
            fails += __builtin_popcount(static_cast<uint32_t>(res));
        }
    }

    std::cout << "fails = " << fails << std::endl;
    std::cout << "unique: " << static_cast<double>(outs.size()) / total << " of " << total << std::endl;

    auto annHash = best.ann_.getHashFunc();
    std::cout << "Avalanche32: " << HashTester::avalancheTester32(annHash, 0xFFF) << std::endl;

    /*
    for(GlobalStatistics::mutRlayer = 1; GlobalStatistics::mutRlayer < 10; GlobalStatistics::mutRlayer+=2) {
        for(GlobalStatistics::mutRweight = 1; GlobalStatistics::mutRweight < 10; GlobalStatistics::mutRweight += 2) {
        //std::cout << "mutRlayer = " << GlobalStatistics::mutRlayer << "\t\tmutRweight = " << GlobalStatistics::mutRweight << std::endl;
        GlobalStatistics::positiveCross = 0;
        GlobalStatistics::positiveMutations = 0;
        GlobalStatistics::totalCross = 0;
        GlobalStatistics::totalMutations = 0;

    auto best = GeneticTrainer<AnnAutoEncoder<MultilayerPerceptron> >::survivalOfTheFittest(
                40,      // max epochs
                10000000,     // max time (msec)
                4,          // villages
                1000,      // village population
                0.95,       // cross probability
                0.95,       // mutation probability
                0.37,        // top X cut-off ("elitism" factor)
                1e-15,// stagnation coefficient
                10,         // innovationsProtectedEpochs
                0, //86,         // output level
                true,       // multithreaded?
                256.0);     // target fitness
            std::cout << std::setprecision(6) << static_cast<double>(best.fitness()) << "\t";
            std::cout.flush();
        }
        std:: cout << "|" << std::endl;
    }
    */

    /*
	auto best = MultilayerPerceptron(
	{
		{
			{ 0x5e2f1abb, 0x96ea1fa6 },
			{ 0xf3e8eb32, 0x6abdcb37 },
			{ 0x815edbe0, 0xca14d813 },
			{ 0x8d1c574c, 0xfd5bdf4a }
		},
		{
			{ 0x3c1909c3, 0x847eb16e },
			{ 0x77aeb9ca, 0x5cfb5a10 }
		}
	},
	{
		{ 0x3bef2b41, 0xd4ec026, 0xe7b3c846, 0x1fcb0cf1 },
		{ 0x7eed0d4d, 0x4abe99c4 },
		{ 0xffffffff, 0xffffffff }
	}
	);
	
	std::cout << best.serialize() << "\n";

	//std::cout << "Fitness: " << best.fitness() << std::endl;

	HashTester::overallTest(best.getHashFunc());

	auto hasher = best.getHashFunc();

	uint32_t in[] = { 143223, 1, 0x12345678, 9821376 };
	std::cout << toHex(hasher(in, 4)) << std::endl;

	uint32_t in2[] = { 0x12345678, 0x90ABCDEF, 0x12345678, 0x90ABCDEF };
	std::cout << toHex(hasher(in2, 4)) << std::endl;

	char text[] = ">> NeuroHash! <<";
	std::cout << toHex(hasher((uint32_t*) text, 4)) << std::endl;

	HashTester::collisionFinder(hasher);
    */
    return 0;
}
