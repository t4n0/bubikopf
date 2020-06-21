#ifndef BITBOARD_MOVE_GENERATION_H
#define BITBOARD_MOVE_GENERATION_H

#include "bitboard/position.h"

#include <type_traits>

namespace Chess {

/// @brief Type to preallocate array for move generation
using MoveList = std::array<Bitmove, 1000>;

/// @brief Type to configure behavior of GenerateMoves at compile time
///
/// This class is substituted for a mock in tests.
struct GenerateAllLegalMoves {
  static constexpr bool generate_all_legal_moves{true};
};

/// @brief Generates all legal moves from position
///
/// @returns An iterator pointing to the element past the last generated move
template <typename Behavior = GenerateAllLegalMoves>
std::enable_if_t<Behavior::generate_all_legal_moves, MoveList::iterator>
GenerateMoves(const PositionWithBitboards& /*unused*/,
              MoveList::iterator move_generation_insertion_iterator) {
  // TODO: implement
  return move_generation_insertion_iterator;
}

}  // namespace Chess

#endif
