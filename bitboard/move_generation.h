#ifndef BITBOARD_MOVE_GENERATION_H
#define BITBOARD_MOVE_GENERATION_H

#include "bitboard/move_lookup_table.h"
#include "bitboard/position.h"
#include "bitboard/squares.h"
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

  // pawn moves
  Bitboard pawn_board = position[board_idx_attacking_side + PAWN];
  Bitmove source_bit = tzcnt(pawn_board);
  while (source_bit < 64) {
    const Bitboard source = 1ULL << source_bit;
    const Bitboard target_capture1_white =
        pawn_capture_lookup_table_white[source_bit];
    const Bitmove target_bit_capture1_white = tzcnt(target_capture1_white);
    const Bitboard target_capture2_white = pawn_capture_lookup_table_white
        [source_bit + pawn_capture_lookup_table_offset_for_second_option];
    const Bitmove target_bit_capture2_white = tzcnt(target_capture2_white);
    const Bitboard target_single_push = source << 8;

    // captures
    if (position[board_idx_defending_side] & target_capture1_white) {
      const Bitmove captured_piece = position.GetPieceKind(
          board_idx_defending_side, target_capture1_white);
      *move_generation_insertion_iterator++ =
          ComposeMove(source_bit, target_bit_capture1_white, PAWN,
                      captured_piece, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE);
    }
    if (position[board_idx_defending_side] & target_capture2_white) {
      const Bitmove captured_piece = position.GetPieceKind(
          board_idx_defending_side, target_capture2_white);
      *move_generation_insertion_iterator++ =
          ComposeMove(source_bit, target_bit_capture2_white, PAWN,
                      captured_piece, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE);
    }

    // en passent

    // single push
    const bool target_single_push_is_occupied =
        (position[board_idx_attacking_side] & target_single_push) ||
        (position[board_idx_defending_side] & target_single_push);
    if (!target_single_push_is_occupied) {
      *move_generation_insertion_iterator++ =
          ComposeMove(source_bit, tzcnt(target_single_push), PAWN, NO_PIECE,
                      NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_PUSH);
    }

    // double push
    constexpr Bitboard start_row_white = A2 | B2 | C2 | D2 | E2 | F2 | G2 | H2;
    const bool source_is_on_start_row = source & start_row_white;
    if (source_is_on_start_row) {
      const Bitboard target_double_push = source << 16;
      const bool target_double_push_is_occupied =
          (position[board_idx_attacking_side] & target_double_push) ||
          (position[board_idx_defending_side] & target_double_push);
      if (!target_single_push_is_occupied && !target_double_push_is_occupied) {
        *move_generation_insertion_iterator++ =
            ComposeMove(source_bit, tzcnt(target_double_push), PAWN, NO_PIECE,
                        NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH);
      }
    }

    // promotion

    // prepare next iteration
    pawn_board &= ~source;
    source_bit = tzcnt(pawn_board);
  }

  // knight moves

  // bishop moves

  // rook moves

  // queen moves

  // king moves

  return move_generation_insertion_iterator;
}

}  // namespace Chess

#endif
