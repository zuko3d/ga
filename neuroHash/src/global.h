#ifndef GLOBAL_H
#define GLOBAL_H

#include <cstdint>
#include <iostream>
#include <cassert>

inline double hRnd() // random double [0 ... 1]
{
    static unsigned int m_z=1,m_w=1;
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return 2.3283064365386962890625e-10 * (double) ((m_z << 16) + m_w);
}

inline int hrand() // random int [0 ... 32767]
{
    static unsigned int m_w=1;
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return m_w & 32767;
}

#endif // GLOBAL_H
