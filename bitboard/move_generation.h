#ifndef BITBOARD_MOVE_GENERATION_H
#define BITBOARD_MOVE_GENERATION_H

#include "bitboard/move_list.h"
#include "bitboard/position.h"

#include <type_traits>

namespace Chess {

struct FindAllLegalMoves {
  static constexpr bool find_all_legal_moves{true};
};

/// @brief Generates all legal moves from position
/// @returns An iterator pointing to the element past the last generated move
template <typename Behavior = FindAllLegalMoves>
std::enable_if_t<Behavior::find_all_legal_moves, move_list_t::iterator>
GenerateMoves(const PositionWithBitboards& /*unused*/,
              const move_list_t::iterator end_iterator_before_move_generation) {
  // TODO: implement
  return end_iterator_before_move_generation;
}

struct GenerateTwoMovesWithUniqueDebugId {
  static constexpr bool generate_two_moves_with_unique_debug_id{true};
};

template <typename Behavior = GenerateTwoMovesWithUniqueDebugId>
std::enable_if_t<Behavior::generate_two_moves_with_unique_debug_id,
                 move_list_t::iterator>
GenerateMoves(const PositionWithBitboards& /*unused*/,
              const move_list_t::iterator end_iterator_before_move_generation) {
  static move_t unique_id{1};
  move_list_t::iterator next_to_insert_iterator =
      end_iterator_before_move_generation;
  *next_to_insert_iterator = unique_id++;
  next_to_insert_iterator++;
  *next_to_insert_iterator = unique_id++;
  next_to_insert_iterator++;
  return next_to_insert_iterator;
}

}  // namespace Chess

#endif
