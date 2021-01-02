#ifndef BITBOARD_FEN_CONVERSION_H
#define BITBOARD_FEN_CONVERSION_H

#include "bitboard/position.h"

#include <string>
#include <vector>

namespace Chess
{

constexpr const char* const kStandardStartingPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

Position PositionFromFen(const std::string& fen);

std::string FenFromPosition(const Position& position);

void PrettyPrintFen(const std::string& fen);

std::vector<std::string> TokenizeFen(const std::string& fen);

constexpr std::size_t kFenTokenPieces = 0;
constexpr std::size_t kFenTokenSide = 1;
constexpr std::size_t kFenTokenCastling = 2;
constexpr std::size_t kFenTokenEnPassant = 3;
constexpr std::size_t kFenTokenStaticPlies = 4;
constexpr std::size_t kFenTokenMoves = 5;

}  // namespace Chess

#endif