#ifndef GLOBAL_H
#define GLOBAL_H

#include <cstdint>
#include <iostream>
#include <cassert>
#include <string>
#include <random>

inline double hRnd() // random double [0 ... 1]
{
    static unsigned int m_z=1,m_w=1;
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return 2.3283064365386962890625e-10 * (double) ((m_z << 16) + m_w);
}

inline unsigned int hrand(void)
{
	static thread_local std::mt19937 generator;
	std::uniform_int_distribution<unsigned int> distribution(0, INT_MAX);
	return distribution(generator);	
	/*
	static thread_local uint64_t __s__[2];
	uint64_t x = __s__[0];
	uint64_t const y = __s__[1];
	__s__[0] = y;
	x ^= x << 23; // a
	__s__[1] = x ^ y ^ (x >> 17) ^ (y >> 26); // b, c
	return __s__[1] + y;
	*/
}

#endif // GLOBAL_H
