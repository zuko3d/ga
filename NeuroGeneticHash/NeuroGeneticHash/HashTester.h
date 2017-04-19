#pragma once

#include "../../neuroHash/src/global.h"
#include <functional>

using namespace std;

class HashTester
{
public:
	static double collisionTester(function<string(uint32_t)> hash, uint64_t tests, uint64_t bitMask = 0);
	static double avalancheTester(function<string(uint32_t)> hash, int tests);

    static double speedTest(function<string(uint32_t)> hash, uint32_t tests = 100000);

	static void overallTest(function<string(uint32_t)> hash);
};

