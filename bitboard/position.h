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
    /// @brief Updates the position wrt. to given move and returns the "extras" bitboard prior to the move.
    ///
    /// Returned "extras" bitboard is necessary to unmake the same move.
    Bitboard MakeMove(Bitmove move);

    /// @brief Takes back given move and restores given extras bitboard.
    void UnmakeMove(Bitmove move, Bitboard extras);

    Bitmove GetPieceKind(const std::size_t side, const Bitboard location) const;

    /// @brief Evaluates whether king of given side is in check.
    ///
    /// A search is started from the kings position and only relevant squares are checked for attackers.
    /// (Rather then calculating a complete attack board).
    bool KingIsInCheck(const std::size_t side) const;

    std::size_t GetNumberOfPlayedPlies() const;

    Bitboard& operator[](const std::size_t index);
    Bitboard operator[](const std::size_t index) const;

    std::array<Bitboard, 17> boards_{};
    bool white_to_move_{true};
    std::size_t attacking_side_{BOARD_IDX_WHITE};
    std::size_t defending_side_{BOARD_IDX_BLACK};
};

bool operator==(const Position& a, const Position& b);

}  // namespace Chess

#endif
