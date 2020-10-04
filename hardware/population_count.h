#ifndef HARDWARE_POPULATION_COUNT_H
#define HARDWARE_POPULATION_COUNT_H

#include <nmmintrin.h>

namespace Chess
{

inline int popcnt(const unsigned long long board)
{
    return static_cast<int>(_mm_popcnt_u64(board));
}

}  // namespace Chess

#endif
