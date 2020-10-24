#ifndef BITBOAD_PIECES_H
#define BITBOAD_PIECES_H

#include <cstdint>
#include <array>

namespace Chess
{

/// @brief Numbers to refer to pieces in different contexts
///
/// E.g. as array index or id in bit masks.
/// Number does not represent piece values for evaluation.
constexpr std::size_t NO_PIECE = 0;
constexpr std::size_t PAWN = 1;
constexpr std::size_t KNIGHT = 2;
constexpr std::size_t BISHOP = 3;
constexpr std::size_t ROOK = 4;
constexpr std::size_t QUEEN = 5;
constexpr std::size_t KING = 6;

constexpr std::array<const char* const, 7> PIECE_LABEL{"NONE", "PAWN", "KNIGHT", "BISHOP", "ROOK", "QUEEN", "KING"};

}  // namespace Chess

#endif
