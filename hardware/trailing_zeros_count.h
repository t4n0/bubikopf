#ifndef HARDWARE_TRAILING_ZEROS_COUNT_H
#define HARDWARE_TRAILING_ZEROS_COUNT_H

#include <immintrin.h>

#include <cstdint>

namespace Chess {

inline unsigned long tzcnt(const std::uint64_t board) {
  return _tzcnt_u64(board);
}

}  // namespace Chess

#endif
