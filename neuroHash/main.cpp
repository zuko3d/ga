#include "src/evolution/genetictrainer.h"
#include "src/evolution/testphenotype.h"
#include "src/evolution/testphenotypediofant.h"
#include "src/evolution/trainerphenotype.h"

#include "src/neuralnetwork/sharedsecretgenerator.h"
#include "src/neuralnetwork/annautoencoder.h"
#include "blake/blake2.h"

#include "src/testers/visualizationtester.h"

#include <vtkRenderWindow.h>

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

    GlobalStatistics::startingPrimeForWeights = 10;
    GlobalStatistics::startingPrimeForBarriers = 0;
    GlobalStatistics::mlpOrder = 4;

    static const char clr[] = { 0, 5, 4, 6, 2, 7, 3, 1};

//    for(size_t bpwr =22; bpwr < 25; bpwr ++){
//        std::cout << "\033[39mbar power: " << bpwr << " ==================================" << std::endl;
//        for(size_t wpwr = 25; wpwr < 35; wpwr += 3) {
//            std::cout << "\033[39mbar: " << bpwr << ", weight power: " << wpwr << std::endl;
//            std::cout << " \t";

//            for(int pop = 0; pop < 1000; pop *= 3) {
//                std::cout << pop << "\t";
//            }
//            std::cout << std::endl;

            for(int time = 100; time < 100000; time *= 2) {
                std::cout << time << ":\t";
                for(int trial = 0; trial < 10; trial++) {

                    auto totalBarriers = 2;
                    GlobalStatistics::barrierValues_.clear();
                    for(size_t i = 0; i < totalBarriers; i++){
                        GlobalStatistics::barrierValues_.push_back(GlobalStatistics::primes_[hrand() % 22]);
                    }

                    GlobalStatistics::weightValues_.clear();
                    auto totalWeights = 10;
                    for(size_t i = 0; i < totalWeights; i++){
                        GlobalStatistics::weightValues_.push_back(GlobalStatistics::primes_[hrand() % 30]);
                    }

        //GlobalStatistics::mlpOrder++;

                    auto bestSolution = GeneticTrainer<SharedSecretGenerator>::survivalOfTheFittest(
                                100000, // max epochs
                                time,  // max time (msec)
                                80,    // villages
                                15,      // village population
                                1.0,    // mutation probability
                                0.1,   // cross probability
                                0.52,   // top X cut-off ("elitism" factor)
                                1.1,    // stagnation coefficient
                                2,     // innovationsProtectedEpochs
                                0,     // output level
                                0.9999);  // target fitness

                    auto best = bestSolution;
                    char color = '0' + clr[static_cast<uint8_t>(floor(best.fitness_ * 7))];

                    std::cout << "\033[3" << color << "m" << std::setprecision(4) << best.fitness_ << "\t";
                    std::cout.flush();
                }
                std::cout << std::endl;
            }
//        }
//    }
    return 0;


    auto bestSolution = GeneticTrainer<SharedSecretGenerator>::survivalOfTheFittest(
                100000, // max epochs
                1 * 1000,  // max time (msec)
                100,    // villages
                5,      // village population
                1.0,    // mutation probability
                0.1,   // cross probability
                0.52,   // top X cut-off ("elitism" factor)
                1.1,    // stagnation coefficient
                2,     // innovationsProtectedEpochs
                0,     // output level
                0.9999);  // target fitness

    auto best = bestSolution;

    /*
    auto keygen = MultilayerPerceptron(
    {
    {
    { 0xB, 0x13 }
    },
    {
    { 0xD },
    { 0x13 }
    }
    }
,
    {
    { 0x3 },
    { 0x13, 0x17 },
    { 0x13 }
    }

                );
    auto best = keygen;
    auto secgen = MultilayerPerceptron(
    {
    {
    { 0x13, 0x5 },
    { 0x1D, 0x13 }
    },
    {
    { 0xD },
    { 0xD }
    }
    }
,
    {
    { 0x7, 0x11 },
    { 0x13, 0x13 },
    { 0x17 }
    }
    );
*/
    auto keygen = bestSolution.keygen_;
    auto secgen = bestSolution.secgen_;
    keygen.reduceWeights();
    secgen.reduceWeights();

    std::vector<std::vector<std::vector<vtkSmartPointer<vtkGraphLayoutView> > > > views;
    views.resize(10);
    for(int i = 0; i < 4; i++) {
        views[i].resize(10);
        for(auto& v: views[i]){
            v.resize(10);
        }
    }

    views[0][0][0] = vtkSmartPointer<vtkGraphLayoutView>::New();
    views[0][0][0]->GetRenderWindow()->SetPosition(0, 0);
    VisualizationTester::visNetwork(keygen, views[0][0][0]);

    views[0][0][1] = vtkSmartPointer<vtkGraphLayoutView>::New();
    views[0][0][1]->GetRenderWindow()->SetPosition(400, 0);
    VisualizationTester::visNetwork(secgen, views[0][0][1]);

    for(int i = 0; i < 3; i++) {
        for(int j = i + 1; j < 3; j++) {
            views[i][j][0] = vtkSmartPointer<vtkGraphLayoutView>::New();
            views[i][j][0]->GetRenderWindow()->SetPosition(i * 800, j * 400);

            views[i][j][1] = vtkSmartPointer<vtkGraphLayoutView>::New();
            views[i][j][1]->GetRenderWindow()->SetPosition(i * 800 + 300, j * 400);

            views[j][i][0] = vtkSmartPointer<vtkGraphLayoutView>::New();
            views[j][i][0]->GetRenderWindow()->SetPosition(j * 800, i * 400);

            views[j][i][1] = vtkSmartPointer<vtkGraphLayoutView>::New();
            views[j][i][1]->GetRenderWindow()->SetPosition(j * 800 + 300, i * 400);

            uint32_t r1 = hrand() % 10;
            uint32_t r2 = hrand() % 10;
            while(r2 == r1) r2 = hrand() % 10;

            VisualizationTester::visVector(keygen.getInternalResults({r1}), views[i][j][0]);
            auto outr1 = keygen.calcOutUint32({r1});

            VisualizationTester::visVector(secgen.getInternalResults({r2, outr1}), views[i][j][1]);


            VisualizationTester::visVector(keygen.getInternalResults({r2}), views[j][i][0]);
            auto outr2 = keygen.calcOutUint32({r2});

            VisualizationTester::visVector(secgen.getInternalResults({r1, outr2}), views[j][i][1]);

            std::this_thread::yield();
            views[j][i][0]->Render();
            std::this_thread::yield();
            views[j][i][1]->Render();
            std::this_thread::yield();
            views[i][j][0]->Render();
            std::this_thread::yield();
            views[i][j][1]->Render();
            std::this_thread::yield();
        }
    }

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

    int i;
    std::cin >> i;
    return 0;
}
