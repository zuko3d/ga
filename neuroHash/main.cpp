#include "src/evolution/genetictrainer.h"
#include "src/evolution/testphenotype.h"
#include "src/evolution/testphenotypediofant.h"
#include "src/evolution/trainerphenotype.h"
#include "../NeuroGeneticHash/NeuroGeneticHash/Simple2x2x1Perceptron.h"
#include "../NeuroGeneticHash/NeuroGeneticHash/MaxEquation.h"
#include "../NeuroGeneticHash/NeuroGeneticHash/PerceptronHasher.h"

#include "../NeuroGeneticHash/NeuroGeneticHash/MultilayerPerceptron.h"
#include "../NeuroGeneticHash/NeuroGeneticHash/AnnHasher.h"

#include "../blake/blake2.h"

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

	/*
    auto best = GeneticTrainer<AnnHasher<MultilayerPerceptron> >::survivalOfTheFittest(
                10000, 100000, 4, 100, 0.5, 0.5, 0.1, 1.0 + 1e-15, 10, 86, true, 1.0);
	*/
	
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
    return 0;
}
