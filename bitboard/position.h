#ifndef BITBOARD_POSITION_H
#define BITBOARD_POSITION_H

#include "bitboard/board.h"
#include "bitboard/move.h"
#include "bitboard/pieces.h"

#include <array>
#include <cstdint>

namespace Chess
{

class PositionWithBitboards
{
  public:
    PositionWithBitboards() {}
    PositionWithBitboards(const std::array<Bitboard, 17>& boards) : boards_(boards) {}

    void MakeMove(Bitmove move);
    void UnmakeMove(Bitmove move);

    bool WhiteToMove() const;
    Bitmove GetPieceKind(const std::size_t side, const Bitboard location) const;

    /// @brief Function to evaluate if defending sides king is in check.
    ///
    /// A search is started from the kings position and only relevant squares are checked for attackers.
    /// (Rather then calculating a complete attack board).
    bool DefendersKingIsInCheck() const;

    Bitboard& operator[](const std::size_t index);
    Bitboard operator[](const std::size_t index) const;

    std::array<Bitboard, 17> boards_{};

    // ToDo: Move this to MoveList type
    std::array<Bitboard, 40> extras_history_{};
    std::size_t extras_history_insertion_index_{0};
};

bool operator==(const PositionWithBitboards& a, const PositionWithBitboards& b);

}  // namespace Chess

#endif
