#ifndef HARDWARE_TRAILING_ZEROS_COUNT_H
#define HARDWARE_TRAILING_ZEROS_COUNT_H

#include <immintrin.h>

namespace Chess {

inline unsigned long tzcnt(const unsigned long long board) {
  return static_cast<unsigned long>(_tzcnt_u64(board));
}

}  // namespace Chess

#endif
