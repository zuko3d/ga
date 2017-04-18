#include "HashTester.h"

#include <functional>
#include <unordered_set>

using namespace std;

double HashTester::collisionTester(function<string(uint32_t)> hash, uint64_t tests, uint64_t bitMask)
{
	if(bitMask == 0) bitMask = tests * (tests + 2);
	
	std::unordered_set<uint64_t> hashes;
	for (uint32_t i = 0; i < tests; i++) {
		uint64_t *h;
		auto ret = hash(i);
		h = reinterpret_cast<uint64_t*>(&ret[0]);
		hashes.insert((*h) & bitMask);
	}

	return ((double)tests - (double)hashes.size()) / ((double)tests);
}

double HashTester::avalancheTester(function<string(uint32_t)> hash, int tests)
{
	const auto bitsize = hash(0).size() * 8;
	uint32_t totalErr = 0;
	int totalTests = 0;
	int localtests = 1;

	for (int test = 0; test < tests; test++) {
		uint32_t in = hrand();
		std::vector<std::string> out;
		out.reserve(localtests + 1);
		out.resize(localtests);
		for (auto& digest : out) {
			auto tmpIn = in ^ (1 << (hrand() % (8 * sizeof(in))));
			digest = std::move(hash(tmpIn));
		}

		out.push_back(hash(in));
		for (int i = 0; i < out.size(); i++) {
			for (int j = i + 1; j < out.size(); j++) {
				int fit = 0;
				uint64_t *op1, *op2;
				op1 = reinterpret_cast<uint64_t*>(&out[i][0]);
				op2 = reinterpret_cast<uint64_t*>(&out[j][0]);
//std::cout << std::hex << *op1 << "\n";
//for (int qwe = 0; qwe < 8; qwe++) std::cout << std::hex << static_cast<uint8_t>(out[i][qwe]);
//std::cout << "\n";
//std::cout << std::hex << *op2 << "\n";
				for (int k = 0; k < (bitsize >> 6); k++) {
					fit += __popcnt64((*op1) ^ (*op2));
					op1++;
					op2++;
				}

				if (fit > bitsize / 2) {
					fit -= bitsize / 2;
				}
				else {
					fit = bitsize / 2 - fit;
				}
				totalErr += fit;
				totalTests++;
			}
		}
	}

	return ((double) totalErr) / ((double)totalTests);
}

void HashTester::overallTest(function<string(uint32_t)> hash)
{
	std::cout << "Collisions 4/4: " << HashTester::collisionTester(hash, 0xFFFF, 0xFFFFULL) << "\n";
	std::cout << "Collisions 3/5: " << HashTester::collisionTester(hash, 0xFFF, 0xFFFFFULL) << "\n";
	std::cout << "Collisions 4/5: " << HashTester::collisionTester(hash, 0xFFFF, 0xFFFFFULL) << "\n";
	std::cout << "Collisions 5/5: " << HashTester::collisionTester(hash, 0xFFFFF, 0xFFFFFULL) << "\n";
	std::cout << "Collisions 4/8: " << HashTester::collisionTester(hash, 0xFFFF, 0xFFFFFFFFULL) << "\n";
	std::cout << "Collisions 5/8: " << HashTester::collisionTester(hash, 0xFFFFF, 0xFFFFFFFFULL) << "\n";
	std::cout << "Collisions 6/8: " << HashTester::collisionTester(hash, 0xFFFFFF, 0xFFFFFFFFULL) << "\n";

	for (int i = 2; i < 19; i++) {
		std::cout << "Avalanche " << i << ": " << HashTester::avalancheTester(hash, 1 << i) << "\n";
	}
}
