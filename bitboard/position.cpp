#include "bitboard/position.h"

#include "bitboard/pieces.h"
#include "bitboard/squares.h"

#include <iostream>

namespace Chess {

bool PositionWithBitboards::WhiteToMove() {
  return boards_[BOARD_IDX_EXTRAS] & BOARD_MASK_WHITE_TURN;
}

void PositionWithBitboards::MakeMove(move_t move) {
  *extras_history_insertion_iterator_ = boards_[BOARD_IDX_EXTRAS];
  extras_history_insertion_iterator_++;

  const std::size_t board_idx_attacking_side =
      boards_[BOARD_IDX_EXTRAS] & BOARD_MASK_WHITE_TURN ? BOARD_IDX_BLACK
                                                        : BOARD_IDX_WHITE;
  const std::size_t board_idx_attacking_piece_kind =
      board_idx_attacking_side +
      ((move & MOVE_MASK_MOVED_PIECE) >> MOVE_SHIFT_MOVED_PIECE);

  const bitboard_t source = BOARD_ONE << (move & MOVE_MASK_SOURCE);
  const bitboard_t target = BOARD_ONE
                            << ((move & MOVE_MASK_TARGET) >> MOVE_SHIFT_TARGET);
  const bitboard_t source_and_target = source ^ target;

  boards_[BOARD_IDX_EXTRAS] ^= BOARD_MASK_WHITE_TURN;
  boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_EN_PASSENT;
  boards_[board_idx_attacking_side] ^= source_and_target;

  const move_t move_type = move & MOVE_MASK_TYPE;
  switch (move_type) {
    case MOVE_VALUE_TYPE_QUIET_NON_PAWN: {
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      boards_[BOARD_IDX_EXTRAS]++;
      return;
    }
    case MOVE_VALUE_TYPE_CAPTURE: {
      const std::size_t board_idx_harmed_side =
          BOARD_IDX_BLACK_AND_WHITE_ADDED - board_idx_attacking_side;
      const std::size_t board_idx_captured_piece_kind =
          board_idx_harmed_side +
          ((move & MOVE_MASK_CAPTURED_PIECE) >> MOVE_SHIFT_CAPTURED_PIECE);
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      boards_[board_idx_harmed_side] &= ~target;
      boards_[board_idx_captured_piece_kind] &= ~target;
      boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_STATIC_PLIES;
      return;
    }
    case MOVE_VALUE_TYPE_PAWN_PUSH: {
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_STATIC_PLIES;
      return;
    }
    case MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH: {
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      boards_[BOARD_IDX_EXTRAS] |= (source_and_target >> 1)  // source+target/2
                                   << BOARD_SHIFT_EN_PASSENT;
      boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_STATIC_PLIES;
      return;
    }
    case MOVE_VALUE_TYPE_EN_PASSENT_CAPTURE: {
      const std::size_t board_idx_harmed_side =
          BOARD_IDX_BLACK_AND_WHITE_ADDED - board_idx_attacking_side;
      const std::size_t board_idx_captured_piece_kind =
          board_idx_harmed_side +
          ((move & MOVE_MASK_CAPTURED_PIECE) >> MOVE_SHIFT_CAPTURED_PIECE);
      const bitboard_t en_passant_victim =
          (source ^ (source << 1) ^ (source >> 1)) &
          (target ^ (target << 8) ^
           ((target >> 8)));  // intersection of vertical and horizontal
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      boards_[board_idx_harmed_side] &= ~en_passant_victim;
      boards_[board_idx_captured_piece_kind] &= ~en_passant_victim;
      boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_STATIC_PLIES;
      return;
    }
    case MOVE_VALUE_TYPE_KINGSIDE: {
      constexpr bitboard_t source_and_target_rook_jump_white = F1 ^ H1;
      constexpr bitboard_t source_and_target_rook_jump_black = F8 ^ H8;
      const bitboard_t source_and_target_rook_jump =
          board_idx_attacking_side == BOARD_IDX_WHITE
              ? source_and_target_rook_jump_white
              : source_and_target_rook_jump_black;
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      boards_[board_idx_attacking_side + ROOK] ^= source_and_target_rook_jump;
      boards_[BOARD_IDX_EXTRAS]++;
      return;
    }
    case MOVE_VALUE_TYPE_QUEENSIDE: {
      constexpr bitboard_t source_and_target_rook_jump_white = A1 ^ D1;
      constexpr bitboard_t source_and_target_rook_jump_black = A8 ^ D8;
      const bitboard_t source_and_target_rook_jump =
          board_idx_attacking_side == BOARD_IDX_WHITE
              ? source_and_target_rook_jump_white
              : source_and_target_rook_jump_black;
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      boards_[board_idx_attacking_side + ROOK] ^= source_and_target_rook_jump;
      boards_[BOARD_IDX_EXTRAS]++;
      return;
    }
    case MOVE_VALUE_TYPE_PROMOTION: {
      const std::size_t board_idx_added_piece_kind =
          board_idx_attacking_side +
          ((move & MOVE_MASK_PROMOTION) >> MOVE_SHIFT_PROMOTION);
      boards_[board_idx_attacking_piece_kind] &= ~source;
      boards_[board_idx_added_piece_kind] ^= target;
      boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_STATIC_PLIES;
      return;
    }
    case MOVE_VALUE_TYPE_PROMOTION_CAPTURE: {
      const std::size_t board_idx_harmed_side =
          BOARD_IDX_BLACK_AND_WHITE_ADDED - board_idx_attacking_side;
      const std::size_t board_idx_captured_piece_kind =
          board_idx_harmed_side +
          ((move & MOVE_MASK_CAPTURED_PIECE) >> MOVE_SHIFT_CAPTURED_PIECE);
      const std::size_t board_idx_added_piece_kind =
          board_idx_attacking_side +
          ((move & MOVE_MASK_PROMOTION) >> MOVE_SHIFT_PROMOTION);
      boards_[board_idx_attacking_piece_kind] &= ~source;
      boards_[board_idx_added_piece_kind] ^= target;
      boards_[board_idx_harmed_side] &= ~target;
      boards_[board_idx_captured_piece_kind] &= ~target;
      boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_STATIC_PLIES;
      return;
    }
  }
}

void PositionWithBitboards::UnmakeMove(move_t /*unused*/) {
  // TODO: implement
}

bitboard_t& PositionWithBitboards::operator[](const std::size_t index) {
  return boards_[index];
}

bitboard_t PositionWithBitboards::operator[](const std::size_t index) const {
  return boards_[index];
}

}  // namespace Chess
