#ifndef BITBOARD_MOVE_GENERATION_H
#define BITBOARD_MOVE_GENERATION_H

#include "bitboard/position.h"

#include <type_traits>

namespace Chess {

/// @brief Type to preallocate array for move generation
using move_list_t = std::array<Bitmove, 1000>;

/// @brief Type to configure behavior of GenerateMoves at compile time
///
/// This class is substitutet for a moock in some tests
struct GenerateAllLegalMoves {
  static constexpr bool generate_all_legal_moves{true};
};

/// @brief Generates all legal moves from position
/// @returns An iterator pointing to the element past the last generated move
template <typename Behavior = GenerateAllLegalMoves>
std::enable_if_t<Behavior::generate_all_legal_moves, move_list_t::iterator>
GenerateMoves(const PositionWithBitboards& /*unused*/,
              const move_list_t::iterator end_iterator_before_move_generation) {
  // TODO: implement
  return end_iterator_before_move_generation;
}

}  // namespace Chess

#endif
