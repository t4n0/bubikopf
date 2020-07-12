#ifndef BITBOARD_MOVE_GENERATION_H
#define BITBOARD_MOVE_GENERATION_H

#include "bitboard/move_lookup_table.h"
#include "bitboard/position.h"
#include "bitboard/squares.h"
#include "hardware/trailing_zeros_count.h"

#include <array>
#include <type_traits>

namespace Chess {

/// @brief Type to preallocate array for move generation
using MoveList = std::array<Bitmove, 1000>;

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

void PushBackAllPromotions(
    MoveList::iterator& move_generation_insertion_iterator,
    const Bitmove source, const Bitmove target, const Bitmove captured_piece) {
  *move_generation_insertion_iterator++ = ComposeMove(
      source, target, PAWN, captured_piece, QUEEN, MOVE_VALUE_TYPE_PROMOTION);
  *move_generation_insertion_iterator++ = ComposeMove(
      source, target, PAWN, captured_piece, ROOK, MOVE_VALUE_TYPE_PROMOTION);
  *move_generation_insertion_iterator++ = ComposeMove(
      source, target, PAWN, captured_piece, KNIGHT, MOVE_VALUE_TYPE_PROMOTION);
  *move_generation_insertion_iterator++ = ComposeMove(
      source, target, PAWN, captured_piece, BISHOP, MOVE_VALUE_TYPE_PROMOTION);
}

}  // namespace

/// @brief Type to configure behavior of GenerateMoves at compile time
///
/// This class is substituted for a mock in tests.
struct GenerateAllPseudoLegalMoves {
  static constexpr bool generate_all_legal_moves{true};
};

/// @brief Generates all pseudo legal moves from position
///
/// "Pseudo" in the sense that own king may be in check after generated move.
///
/// @returns An iterator pointing to the element past the last generated move
template <typename Behavior = GenerateAllPseudoLegalMoves>
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
      const bool is_promotion = target_capture1_white & PROMOTION_RANKS;
      if (!is_promotion) {
        *move_generation_insertion_iterator++ =
            ComposeMove(source_bit, target_bit_capture1_white, PAWN,
                        captured_piece, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE);
      } else {
        // promotion (with capture)
        PushBackAllPromotions(move_generation_insertion_iterator, source_bit,
                              tzcnt(target_capture1_white), captured_piece);
      }
    }
    if (position[board_idx_defending_side] & target_capture2_white) {
      const Bitmove captured_piece = position.GetPieceKind(
          board_idx_defending_side, target_capture2_white);
      const bool is_promotion = target_capture2_white & PROMOTION_RANKS;
      if (!is_promotion) {
        *move_generation_insertion_iterator++ =
            ComposeMove(source_bit, target_bit_capture2_white, PAWN,
                        captured_piece, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE);
      } else {
        // promotion (with capture)
        PushBackAllPromotions(move_generation_insertion_iterator, source_bit,
                              tzcnt(target_capture2_white), captured_piece);
      }
    }

    // en passent
    const Bitboard en_passent_mask =
        (position[BOARD_IDX_EXTRAS] & BOARD_MASK_EN_PASSENT);
    if (en_passent_mask) {
      const Bitboard en_passent_bit = en_passent_mask >> BOARD_SHIFT_EN_PASSENT;
      if (en_passent_bit == target_bit_capture1_white) {
        *move_generation_insertion_iterator++ =
            ComposeMove(source_bit, target_bit_capture1_white, PAWN, PAWN,
                        NO_PROMOTION, MOVE_VALUE_TYPE_EN_PASSENT_CAPTURE);
      }
      if (en_passent_bit == target_bit_capture2_white) {
        *move_generation_insertion_iterator++ =
            ComposeMove(source_bit, target_bit_capture2_white, PAWN, PAWN,
                        NO_PROMOTION, MOVE_VALUE_TYPE_EN_PASSENT_CAPTURE);
      }
    }

    // single push
    const bool target_single_push_is_occupied =
        (position[BOARD_IDX_BLACK] | position[BOARD_IDX_WHITE]) &
        target_single_push;
    if (!target_single_push_is_occupied) {
      const bool is_promotion = target_single_push & PROMOTION_RANKS;
      if (!is_promotion) {
        *move_generation_insertion_iterator++ =
            ComposeMove(source_bit, tzcnt(target_single_push), PAWN, NO_PIECE,
                        NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_PUSH);
      } else {
        // promotion (without capture)
        PushBackAllPromotions(move_generation_insertion_iterator, source_bit,
                              tzcnt(target_single_push), NO_PIECE);
      }
    }

    // double push
    const bool source_is_on_start_row = source & START_RANK_WHITE;
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
