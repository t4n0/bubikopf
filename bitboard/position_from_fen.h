#ifndef BITBOARD_POSITION_FROM_FEN_H
#define BITBOARD_POSITION_FROM_FEN_H

#include "bitboard/position.h"

#include <string>

namespace Chess
{

const char* const kStandardStartingPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

PositionWithBitboards PositionFromFen(const std::string& fen);

}  // namespace Chess

#endif