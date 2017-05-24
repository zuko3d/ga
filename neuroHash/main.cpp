#include "src/evolution/genetictrainer.h"
#include "src/evolution/testphenotype.h"
#include "src/evolution/testphenotypediofant.h"
#include "src/evolution/trainerphenotype.h"

#include "src/neuralnetwork/sharedsecretgenerator.h"
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

    GlobalStatistics::startPrime = 0;
    GlobalStatistics::mlpOrder = 4;

    for(size_t i = 0; i < GlobalStatistics::mlpOrder; i++){
        GlobalStatistics::primes_[i] = GlobalStatistics::primes_[400 + hrand() % 4000];
    }

    GlobalStatistics::mlpOrder++;

    auto bestSolution = GeneticTrainer<SharedSecretGenerator>::survivalOfTheFittest(
                100000, // max epochs
                14 * 3600 * 1000,  // max time (msec)
                1000,    // villages
                10,      // village population
                1.0,    // mutation probability
                0.1,   // cross probability
                0.52,   // top X cut-off ("elitism" factor)
                1.1,    // stagnation coefficient
                10,     // innovationsProtectedEpochs
                86,     // output level
                0.9999);  // target fitness

    auto best = bestSolution;

    /*
    const uint32_t x13 = 71;
    const uint32_t xB = 0xB;
    const uint32_t xD = 0xD;
    const uint32_t x3 = 0x3;
    const uint32_t x17 = 67;
    const uint32_t x5 = 0x5;
    const uint32_t x1D = 0x1D;
    const uint32_t x7 = 0xFF8;
    const uint32_t x11 = 0x11;

    auto mlp1 = MultilayerPerceptron(
    {
    {
    { xB, x13 }
    },
    {
    { xD },
    { x13 }
    }
    }
,
    {
    { x3 },
    { x13, x17 },
    { x13 }
    }
                );

    auto mlp2 = MultilayerPerceptron(
    {
    {
    { x13, x5 },
    { x1D, x13 }
    },
    {
    { xD },
    { xD }
    }
    }
,
    {
    { x7, x11 },
    { x13, x13 },
    { x17 }
    }
);
    auto best = SharedSecretGenerator(mlp1, mlp2);
    */

    std::cout << "best: " << best.serialize();
    std::cout << "Examples: " << std::endl;

    const uint32_t sz = 0xFFF;

    uint32_t fails = 0;
    std::set<uint32_t> outs;
    double total = 0;

    //for(uint32_t i = 0xFFFFFFFB - sz; i < 0xFFFFFFFB; i++) {
    for(uint32_t i = 0; i < sz; i++) {
        for(uint32_t j = 0; j < 0xF; j++) {
            if(j >= i) continue;
            auto out = best.calcOut({j}, {i});
            outs.insert(out);
            total++;
            auto out2 = best.calcOut({i}, {j});
            auto res = out ^ out2;
            fails += __builtin_popcount(res);

            if((i < 4) && (j < 4)){
                std::cout << i << ", " << j << ":\t" << out << " | " << out2 << std::endl;
            }
        }
    }

    std::cout << "fails = " << fails << std::endl;
    std::cout << "unique: " << outs.size() << " / " << static_cast<double>(outs.size()) / total << " of " << total << std::endl;
    std::cout << "fitness: " << bestSolution.fitness() << std::endl;

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
            { x5e2f1abb, 0x96ea1fa6 },
			{ 0xf3e8eb32, 0x6abdcb37 },
			{ 0x815edbe0, 0xca14d813 },
			{ 0x8d1c574c, 0xfd5bdf4a }
		},
		{
            { x3c1909c3, 0x847eb16e },
            { 0x77aeb9ca, x5cfb5a10 }
		}
	},
	{
        { x3bef2b41, xD4ec026, 0xe7b3c846, 0x1fcb0cf1 },
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
