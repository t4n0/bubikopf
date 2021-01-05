#ifndef BITBOARD_BOARD_H
#define BITBOARD_BOARD_H

#include "bitboard/basic_type_declarations.h"

#include <cstdint>

namespace Chess
{

constexpr std::size_t kExtrasBoard = 0;
constexpr std::size_t kBlackBoard = 1;
constexpr std::size_t kWhiteBoard = 9;
constexpr std::size_t kUnused1Board = 8;
constexpr std::size_t kUnused2Board = 16;
constexpr std::size_t kToggleSide = kWhiteBoard - kBlackBoard;

constexpr Bitboard kBoardMaskStaticPlies = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00111111;
constexpr Bitboard kBoardMaskEnPassant = 0b00000000'00000000'00000000'00000000'00000000'00000000'00111111'00000000;
constexpr Bitboard kBoardMaskCastling = 0b00000000'00000000'00000000'00000000'00000000'00001111'00000000'00000000;
constexpr Bitboard kBoardMaskTotalPlies = 0b11111111'11111111'00000000'00000000'00000000'00000000'00000000'00000000;
constexpr Bitboard kBoardMaskUnused = 0b00000000'00000000'11111111'11111111'11111111'11110000'11000000'11000000;

constexpr Bitboard kIncrementTotalPlies = 0b00000000'00000001'00000000'00000000'00000000'00000000'00000000'00000000;

// current castling rights
constexpr Bitboard kCastlingWhiteKingside = 0b00000000'00000000'00000000'00000000'00000000'00001000'00000000'00000000;
constexpr Bitboard kCastlingWhiteQueenside = 0b00000000'00000000'00000000'00000000'00000000'00000100'00000000'00000000;
constexpr Bitboard kCastlingBlackKingside = 0b00000000'00000000'00000000'00000000'00000000'00000010'00000000'00000000;
constexpr Bitboard kCastlingBlackQueenside = 0b00000000'00000000'00000000'00000000'00000000'00000001'00000000'00000000;

// castling occured on last move, regardless of side
constexpr Bitboard kKingsideCastlingOnLastMove =
    0b00000000'00000000'00000000'00000000'00000000'00100000'00000000'00000000;
constexpr Bitboard kQueensideCastlingOnLastMove =
    0b00000000'00000000'00000000'00000000'00000000'00010000'00000000'00000000;

constexpr int kBoardShiftEnPassant = 8;
constexpr int kBoardShiftTotalPlies = 48;

}  // namespace Chess

#endif
