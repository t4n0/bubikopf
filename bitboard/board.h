#ifndef BITBOARD_BOARD_H
#define BITBOARD_BOARD_H

#include "bitboard/basic_type_declarations.h"
#include "bitboard/squares.h"

#include <cstdint>

namespace Chess
{

constexpr std::size_t kExtrasBoard = 0;
constexpr std::size_t kBlackBoard = 1;
constexpr std::size_t kWhiteBoard = 9;
constexpr std::size_t kUnused1Board = 8;
constexpr std::size_t kUnused2Board = 16;
constexpr std::size_t kToggleSide = kWhiteBoard - kBlackBoard;

// clang-format off
constexpr Bitboard kBoardMaskStaticPlies =                 0b00000000'00000000'00000000'00000000'00000000'00000000'00111111'00000000;
constexpr Bitboard kBoardMaskEnPassant =                   0b00000000'00000000'11111111'00000000'00000000'11111111'00000000'00000000;
constexpr Bitboard kBoardMaskTotalPlies =                  0b00000000'00000000'00000000'11111111'11111111'00000000'00000000'00000000;
constexpr Bitboard kBoardMaskUnused =                      0b01110110'11111111'00000000'00000000'00000000'00000000'00000000'01110110;

constexpr Bitboard kIncrementStaticPlies =                 0b00000000'00000000'00000000'00000000'00000000'00000000'00000001'00000000;
constexpr Bitboard kIncrementTotalPlies =                  0b00000000'00000000'00000000'00000000'00000001'00000000'00000000'00000000;

// castling occured on last move, regardless of side
constexpr Bitboard kBoardMaskKingsideCastlingOnLastMove =  0b00000000'00000000'00000000'00000000'00000000'00000000'10000000'00000000;
constexpr Bitboard kBoardMaskQueensideCastlingOnLastMove = 0b00000000'00000000'00000000'00000000'00000000'00000000'01000000'00000000;
// current castling rights
constexpr Bitboard kBoardMaskCastling =                    A1 | A8 | H1 | H8 | E1 | E8;  // rook and king start positions
// clang-format on

constexpr Bitboard kCastlingWhiteKingside = H1 | E1;
constexpr Bitboard kCastlingWhiteQueenside = A1 | E1;
constexpr Bitboard kCastlingBlackKingside = H8 | E8;
constexpr Bitboard kCastlingBlackQueenside = A8 | E8;
constexpr Bitboard kCastlingStillPossible = E1 | E8;

constexpr int kBoardShiftStaticPlies = 8;
constexpr int kBoardShiftTotalPlies = 24;

}  // namespace Chess

#endif
