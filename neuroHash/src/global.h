#ifndef GLOBAL_H
#define GLOBAL_H

#include <cstdint>
#include <iostream>
#include <cassert>
#include <string>
#include <random>
#include <functional>

typedef std::function<std::string(uint32_t*, size_t size)> hashFunc_t;


inline std::string toHex(std::string str) {
	static char hex[] = "0123456789ABCDEF";
	std::string ret = "0x";

	for (auto& c : str) {
		unsigned char uc = static_cast<unsigned char>(c);
		ret += hex[uc / 16];
		ret += hex[uc % 16];
	}

	return ret;
}

#ifdef _MSC_VER 
#define __builtin_popcountll __popcnt64
#endif

inline double hRnd() // random double [0 ... 1]
{
    static unsigned int m_z=1,m_w=1;
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return 2.3283064365386962890625e-10 * (double) ((m_z << 16) + m_w);
}

inline uint32_t hrand(void)
{
    static std::mt19937 generator;
    std::uniform_int_distribution<uint32_t> distribution(0, UINT32_MAX);
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
