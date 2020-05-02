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

struct CreateTwoNullMoves {
  static constexpr bool create_two_null_moves{true};
};

template <typename Behavior = CreateTwoNullMoves>
std::enable_if_t<Behavior::create_two_null_moves, move_list_t::iterator>
GenerateMoves(const PositionWithBitboards& /*unused*/,
              const move_list_t::iterator end_iterator_before_move_generation) {
  move_list_t::iterator next_to_insert_iterator =
      end_iterator_before_move_generation;
  *next_to_insert_iterator = move_t{1};
  next_to_insert_iterator++;
  *next_to_insert_iterator = move_t{1};
  next_to_insert_iterator++;
  return next_to_insert_iterator;
}

}  // namespace Chess

#endif
