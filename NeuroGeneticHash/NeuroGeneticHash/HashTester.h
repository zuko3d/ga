#pragma once

#include "../../neuroHash/src/global.h"
#include <functional>

using namespace std;

class HashTester
{
public:
	static double collisionTester(function<string(uint32_t)> hash, uint64_t tests);
	static double avalancheTester(function<string(uint32_t)> hash, int tests);
};

