#pragma once

#include "../../neuroHash/src/global.h"
#include <functional>

using namespace std;

class HashTester
{
public:
	static double collisionTester(hashFunc_t hash, uint64_t tests, uint64_t bitMask = 0);
	static double collisionFinder(hashFunc_t hash);
	static double avalancheTester(hashFunc_t hash, int tests);

    static double speedTest(hashFunc_t hash, uint32_t tests = 100000);

	static void overallTest(hashFunc_t hash);
};

