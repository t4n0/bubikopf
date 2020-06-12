#include "bitboard/position.h"

#include "bitboard/pieces.h"
#include "bitboard/squares.h"

namespace Chess {

bool PositionWithBitboards::WhiteToMove() {
  return boards_[BOARD_IDX_EXTRAS] & BOARD_MASK_WHITE_TURN;
}

void PositionWithBitboards::MakeMove(Bitmove move) {
  extras_history_[extras_history_insertion_index_] = boards_[BOARD_IDX_EXTRAS];
  extras_history_insertion_index_++;

  const std::size_t board_idx_attacking_side =
      boards_[BOARD_IDX_EXTRAS] & BOARD_MASK_WHITE_TURN ? BOARD_IDX_WHITE
                                                        : BOARD_IDX_BLACK;
  const std::size_t board_idx_attacking_piece_kind =
      board_idx_attacking_side +
      ((move & MOVE_MASK_MOVED_PIECE) >> MOVE_SHIFT_MOVED_PIECE);

  const Bitboard source = BOARD_ONE << (move & MOVE_MASK_SOURCE);
  const Bitboard target = BOARD_ONE
                          << ((move & MOVE_MASK_TARGET) >> MOVE_SHIFT_TARGET);
  const Bitboard source_and_target = source | target;

  boards_[BOARD_IDX_EXTRAS] ^= BOARD_MASK_WHITE_TURN;
  boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_EN_PASSENT;
  boards_[board_idx_attacking_side] ^= source_and_target;

  const Bitmove move_type = move & MOVE_MASK_TYPE;
  switch (move_type) {
    case MOVE_VALUE_TYPE_QUIET_NON_PAWN: {
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      boards_[BOARD_IDX_EXTRAS]++;
      return;
    }
    case MOVE_VALUE_TYPE_CAPTURE: {
      const std::size_t board_idx_harmed_side =
          BOARD_IDX_BLACK_WHITE_SUM - board_idx_attacking_side;
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
      const Bitmove source_bit = move & MOVE_MASK_SOURCE;
      const Bitmove target_bit = (move & MOVE_MASK_TARGET) >> MOVE_SHIFT_TARGET;
      boards_[BOARD_IDX_EXTRAS] |=
          ((source_bit + target_bit) >> 1)
          << BOARD_SHIFT_EN_PASSENT;  // (source_bit+target_bit)/2
      boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_STATIC_PLIES;
      return;
    }
    case MOVE_VALUE_TYPE_EN_PASSENT_CAPTURE: {
      const std::size_t board_idx_harmed_side =
          BOARD_IDX_BLACK_WHITE_SUM - board_idx_attacking_side;
      const Bitboard en_passant_victim =
          BOARD_ONE
          << ((extras_history_[extras_history_insertion_index_ - 1] &
               BOARD_MASK_EN_PASSENT) >>
              BOARD_SHIFT_EN_PASSENT);  // En-passent bit mask erased at
                                        // function entry. Get from history.
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      boards_[board_idx_harmed_side] &= ~en_passant_victim;
      boards_[board_idx_harmed_side + PAWN] &= ~en_passant_victim;
      boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_STATIC_PLIES;
      return;
    }
    case MOVE_VALUE_TYPE_KINGSIDE_CASTLING: {
      const Bitboard source_rook = target >> 1;
      const Bitboard target_rook = target << 1;
      const Bitboard source_and_target_rook_jump = source_rook | target_rook;
      const Bitboard board_mask_castling =
          board_idx_attacking_side == BOARD_IDX_WHITE
              ? BOARD_VALUE_CASTLING_WHITE_KINGSIDE
              : BOARD_VALUE_CASTLING_BLACK_KINGSIDE;
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      boards_[board_idx_attacking_side + ROOK] ^= source_and_target_rook_jump;
      boards_[BOARD_IDX_EXTRAS]++;
      boards_[BOARD_IDX_EXTRAS] &= ~board_mask_castling;
      return;
    }
    case MOVE_VALUE_TYPE_QUEENSIDE_CASTLING: {
      const Bitboard source_rook = target << 2;
      const Bitboard target_rook = target >> 1;
      const Bitboard source_and_target_rook_jump = source_rook | target_rook;
      const Bitboard board_mask_castling =
          board_idx_attacking_side == BOARD_IDX_WHITE
              ? BOARD_VALUE_CASTLING_WHITE_QUEENSIDE
              : BOARD_VALUE_CASTLING_BLACK_QUEENSIDE;
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      boards_[board_idx_attacking_side + ROOK] ^= source_and_target_rook_jump;
      boards_[BOARD_IDX_EXTRAS]++;
      boards_[BOARD_IDX_EXTRAS] &= ~board_mask_castling;
      return;
    }
    case MOVE_VALUE_TYPE_KINGSIDE_ROOK: {
      const Bitboard board_mask_castling =
          board_idx_attacking_side == BOARD_IDX_WHITE
              ? BOARD_VALUE_CASTLING_WHITE_KINGSIDE
              : BOARD_VALUE_CASTLING_BLACK_KINGSIDE;
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      boards_[BOARD_IDX_EXTRAS]++;
      boards_[BOARD_IDX_EXTRAS] &= ~board_mask_castling;
      return;
    }
    case MOVE_VALUE_TYPE_QUEENSIDE_ROOK: {
      const Bitboard board_mask_castling =
          board_idx_attacking_side == BOARD_IDX_WHITE
              ? BOARD_VALUE_CASTLING_WHITE_QUEENSIDE
              : BOARD_VALUE_CASTLING_BLACK_QUEENSIDE;
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      boards_[BOARD_IDX_EXTRAS]++;
      boards_[BOARD_IDX_EXTRAS] &= ~board_mask_castling;
      return;
    }
    case MOVE_VALUE_TYPE_PROMOTION: {
      const std::size_t board_idx_added_piece_kind =
          board_idx_attacking_side +
          ((move & MOVE_MASK_PROMOTION) >> MOVE_SHIFT_PROMOTION);
      boards_[board_idx_attacking_piece_kind] &= ~source;
      boards_[board_idx_added_piece_kind] |= target;
      boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_STATIC_PLIES;
      const Bitmove capture = move & MOVE_MASK_CAPTURED_PIECE;
      if (capture) {
        const std::size_t board_idx_harmed_side =
            BOARD_IDX_BLACK_WHITE_SUM - board_idx_attacking_side;
        const std::size_t board_idx_captured_piece_kind =
            board_idx_harmed_side + (capture >> MOVE_SHIFT_CAPTURED_PIECE);
        boards_[board_idx_harmed_side] &= ~target;
        boards_[board_idx_captured_piece_kind] &= ~target;
      }
      return;
    }
  }
}

void PositionWithBitboards::UnmakeMove(Bitmove move) {
  extras_history_insertion_index_--;
  boards_[BOARD_IDX_EXTRAS] = extras_history_[extras_history_insertion_index_];

  const std::size_t board_idx_attacking_side =
      boards_[BOARD_IDX_EXTRAS] & BOARD_MASK_WHITE_TURN ? BOARD_IDX_WHITE
                                                        : BOARD_IDX_BLACK;
  const std::size_t board_idx_attacking_piece_kind =
      board_idx_attacking_side +
      ((move & MOVE_MASK_MOVED_PIECE) >> MOVE_SHIFT_MOVED_PIECE);

  const Bitboard source = BOARD_ONE << (move & MOVE_MASK_SOURCE);
  const Bitboard target = BOARD_ONE
                          << ((move & MOVE_MASK_TARGET) >> MOVE_SHIFT_TARGET);
  const Bitboard source_and_target = source | target;

  boards_[board_idx_attacking_side] ^= source_and_target;

  const Bitmove move_type = move & MOVE_MASK_TYPE;
  switch (move_type) {
    case MOVE_VALUE_TYPE_QUIET_NON_PAWN: {
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      return;
    }
    case MOVE_VALUE_TYPE_CAPTURE: {
      const std::size_t board_idx_harmed_side =
          BOARD_IDX_BLACK_WHITE_SUM - board_idx_attacking_side;
      const std::size_t board_idx_captured_piece_kind =
          board_idx_harmed_side +
          ((move & MOVE_MASK_CAPTURED_PIECE) >> MOVE_SHIFT_CAPTURED_PIECE);
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      boards_[board_idx_harmed_side] |= target;
      boards_[board_idx_captured_piece_kind] |= target;
      return;
    }
    case MOVE_VALUE_TYPE_PAWN_PUSH: {
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      return;
    }
    case MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH: {
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      return;
    }
    case MOVE_VALUE_TYPE_EN_PASSENT_CAPTURE: {
      const std::size_t board_idx_harmed_side =
          BOARD_IDX_BLACK_WHITE_SUM - board_idx_attacking_side;
      const Bitboard en_passant_victim =
          BOARD_ONE
          << ((boards_[BOARD_IDX_EXTRAS] & BOARD_MASK_EN_PASSENT) >>
              BOARD_SHIFT_EN_PASSENT);  // En-passent bit mask erased at
                                        // function entry. Get from history.
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      boards_[board_idx_harmed_side] |= en_passant_victim;
      boards_[board_idx_harmed_side + PAWN] |= en_passant_victim;
      return;
    }
    case MOVE_VALUE_TYPE_KINGSIDE_CASTLING: {
      const Bitboard source_rook = target >> 1;
      const Bitboard target_rook = target << 1;
      const Bitboard source_and_target_rook_jump = source_rook | target_rook;
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      boards_[board_idx_attacking_side + ROOK] ^= source_and_target_rook_jump;
      return;
    }
    case MOVE_VALUE_TYPE_QUEENSIDE_CASTLING: {
      const Bitboard source_rook = target << 2;
      const Bitboard target_rook = target >> 1;
      const Bitboard source_and_target_rook_jump = source_rook | target_rook;
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      boards_[board_idx_attacking_side + ROOK] ^= source_and_target_rook_jump;
      return;
    }
    case MOVE_VALUE_TYPE_KINGSIDE_ROOK: {
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      return;
    }
    case MOVE_VALUE_TYPE_QUEENSIDE_ROOK: {
      boards_[board_idx_attacking_piece_kind] ^= source_and_target;
      return;
    }
    case MOVE_VALUE_TYPE_PROMOTION: {
      const std::size_t board_idx_added_piece_kind =
          board_idx_attacking_side +
          ((move & MOVE_MASK_PROMOTION) >> MOVE_SHIFT_PROMOTION);
      boards_[board_idx_attacking_piece_kind] |= source;
      boards_[board_idx_added_piece_kind] &= ~target;
      const Bitmove capture = move & MOVE_MASK_CAPTURED_PIECE;
      if (capture) {
        const std::size_t board_idx_harmed_side =
            BOARD_IDX_BLACK_WHITE_SUM - board_idx_attacking_side;
        const std::size_t board_idx_captured_piece_kind =
            board_idx_harmed_side + (capture >> MOVE_SHIFT_CAPTURED_PIECE);
        boards_[board_idx_harmed_side] |= target;
        boards_[board_idx_captured_piece_kind] |= target;
      }
      return;
    }
  }
}

Bitboard& PositionWithBitboards::operator[](const std::size_t index) {
  return boards_[index];
}

Bitboard PositionWithBitboards::operator[](const std::size_t index) const {
  return boards_[index];
}

bool operator==(const PositionWithBitboards& a,
                const PositionWithBitboards& b) {
  const bool boards_are_equal = a.boards_ == b.boards_;
  const bool iterators_are_equal =
      a.extras_history_insertion_index_ == b.extras_history_insertion_index_;
  const bool histories_are_equal =
      std::equal(begin(a.extras_history_),
                 begin(a.extras_history_) + a.extras_history_insertion_index_,
                 begin(b.extras_history_));  // Don't consider obsolete data at
                                             // insertion index and beyond

  return boards_are_equal && iterators_are_equal && histories_are_equal;
}

}  // namespace Chess
