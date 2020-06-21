#ifndef BITBOARD_MOVE_GENERATION_H
#define BITBOARD_MOVE_GENERATION_H

#include "bitboard/position.h"
#include "hardware/trailing_zeros_count.h"

#include <array>
#include <type_traits>

namespace Chess {

namespace {

Bitmove ComposeMove(const Bitmove source, const Bitmove target,
                    const Bitmove moved_piece, const Bitmove captured_piece,
                    const Bitmove promotion, const Bitmove move_type) {
  return source |                                         //
         (target << MOVE_SHIFT_TARGET) |                  //
         (moved_piece << MOVE_SHIFT_MOVED_PIECE) |        //
         (captured_piece << MOVE_SHIFT_CAPTURED_PIECE) |  //
         (promotion << MOVE_SHIFT_PROMOTION) |            //
         move_type;
}

}  // namespace

/// @brief Type to preallocate array for move generation
using MoveList = std::array<Bitmove, 1000>;

/// @brief Type to configure behavior of GenerateMoves at compile time
///
/// This class is substituted for a mock in tests.
struct GenerateAllLegalMoves {
  static constexpr bool generate_all_legal_moves{true};
};

/// @brief Generates all pseudo legal moves from position
///
/// @returns An iterator pointing to the element past the last generated move
template <typename Behavior = GenerateAllLegalMoves>
std::enable_if_t<Behavior::generate_all_legal_moves, MoveList::iterator>
GenerateMoves(const PositionWithBitboards& position,
              MoveList::iterator move_generation_insertion_iterator) {
  const std::size_t board_idx_attacking_side =
      BOARD_IDX_BLACK + BOARD_IDX_BLACK_WHITE_DIFF * position.WhiteToMove();
  const std::size_t board_idx_defending_side =
      BOARD_IDX_BLACK_WHITE_SUM - board_idx_attacking_side;

  // swap boards for black pawns?

  // pawns
  Bitboard pawn_board = position[board_idx_attacking_side + PAWN];
  Bitmove source_bit = tzcnt(pawn_board);
  while (source_bit < 64) {
    const Bitboard source = 1ULL << source_bit;
    pawn_board &= ~source;

    // capture
    // en passent
    // double push

    // single push
    const Bitboard target_single_push = source << 8;
    const bool target_single_push_is_occupied =
        (position[board_idx_attacking_side] & target_single_push) ||
        (position[board_idx_defending_side] & target_single_push);
    if (!target_single_push_is_occupied) {
      const Bitmove target_bit_single_push = source_bit + 8;
      *move_generation_insertion_iterator++ =
          ComposeMove(source_bit, target_bit_single_push, PAWN, NO_PIECE,
                      NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_PUSH);
    }

    source_bit = tzcnt(pawn_board);
  }

  // swap boards for black pawns?

  // knight

  // bishop

  // rook

  // queen

  // king

  return move_generation_insertion_iterator;
}

}  // namespace Chess

#endif
