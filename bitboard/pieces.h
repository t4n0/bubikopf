#ifndef BITBOAD_PIECES_H
#define BITBOAD_PIECES_H

#include <array>
#include <cstdint>

namespace Chess
{

/// @brief Values refer to pieces in different contexts
///
/// E.g. as array index or id in bit masks. It does not represent piece value for evaluation.
constexpr std::size_t kNoPiece = 0;
constexpr std::size_t kPawn = 1;
constexpr std::size_t kKnight = 2;
constexpr std::size_t kBishop = 3;
constexpr std::size_t kRook = 4;
constexpr std::size_t kQueen = 5;
constexpr std::size_t kKing = 6;

// UCI does not differentiate between white and black
constexpr std::array<char, 7> kPieceLabels{'-', 'p', 'n', 'b', 'r', 'q', 'k'};
// FEN differentiates between white and black
constexpr std::array<char, 7> kPieceLabelsFenWhite{'-', 'P', 'N', 'B', 'R', 'Q', 'K'};

}  // namespace Chess

#endif
