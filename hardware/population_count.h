#ifndef HARDWARE_POPULATION_COUNT_H
#define HARDWARE_POPULATION_COUNT_H

#include <nmmintrin.h>

#include <cstdint>

namespace Chess {

inline long long popcnt(const std::uint64_t board) {
  return _mm_popcnt_u64(board);
}

}  // namespace Chess

#endif