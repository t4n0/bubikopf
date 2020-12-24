#ifndef BITBOARD_POSITION_H
#define BITBOARD_POSITION_H

#include "bitboard/board.h"
#include "bitboard/move.h"
#include "bitboard/pieces.h"

#include <array>
#include <cstdint>

namespace Chess
{

class Position
{
  public:
    void MakeMove(Bitmove move);
    void UnmakeMove(Bitmove move);

    Bitmove GetPieceKind(const std::size_t side, const Bitboard location) const;

    /// @brief Function to evaluate if defending sides king is in check.
    ///
    /// A search is started from the kings position and only relevant squares are checked for attackers.
    /// (Rather then calculating a complete attack board).
    bool DefendersKingIsInCheck() const;

    Bitboard& operator[](const std::size_t index);
    Bitboard operator[](const std::size_t index) const;

    std::array<Bitboard, 17> boards_{};
    bool white_to_move_{true};
    std::size_t attacking_side_{BOARD_IDX_WHITE};
    std::size_t defending_side_{BOARD_IDX_BLACK};

    // ToDo: Move this to MoveList type
    std::array<Bitboard, 40> extras_history_{};
    std::array<Bitboard, 40>::iterator extras_history_insertion_index_{extras_history_.begin()};
};

bool operator==(const Position& a, const Position& b);

}  // namespace Chess

#endif
