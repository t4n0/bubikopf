#ifndef BITBOARD_MOVE_H
#define BITBOARD_MOVE_H

#include "bitboard/basic_type_declarations.h"

#include <string>

namespace Chess
{

constexpr Bitmove kBitNullMove = 0;

// clang-format off
constexpr Bitmove kMoveMaskSource =            0b00000000'00000000'00000000'00111111;
constexpr Bitmove kMoveMaskTarget =            0b00000000'00000000'00001111'11000000;
constexpr Bitmove kMoveMaskMovedPiece =        0b00000000'00011100'00000000'00000000;
constexpr Bitmove kMoveMaskCapturedPiece =     0b00000000'11100000'00000000'00000000;
constexpr Bitmove kMoveMaskPromotion =         0b00000111'00000000'00000000'00000000;
constexpr Bitmove kMoveMaskType =              0b01111000'00000000'00000000'00000000;
constexpr Bitmove kMoveMaskUnused =            0b10000000'00000011'11110000'00000000;

constexpr Bitmove kMoveTypeQuietNonPawn =      0b00000000'00000000'00000000'00000000;
constexpr Bitmove kMoveTypeCapture =           0b00001000'00000000'00000000'00000000;
constexpr Bitmove kMoveTypePawnSinglePush =    0b00010000'00000000'00000000'00000000;
constexpr Bitmove kMoveTypePawnDoublePush =    0b00011000'00000000'00000000'00000000;
constexpr Bitmove kMoveTypeEnPassantCapture =  0b00100000'00000000'00000000'00000000;
constexpr Bitmove kMoveTypeKingsideCastling =  0b00101000'00000000'00000000'00000000;
constexpr Bitmove kMoveTypeQueensideCastling = 0b00110000'00000000'00000000'00000000;
constexpr Bitmove kMoveTypePromotion =         0b00111000'00000000'00000000'00000000;
// clang-format on

constexpr int kMoveShiftTarget = 6;
constexpr int kMoveShiftMovedPiece = 18;
constexpr int kMoveShiftCapturedPiece = 21;
constexpr int kMoveShiftPromotion = 24;
constexpr int kMoveShiftType = 27;

constexpr std::size_t kNoCapture = 0;    // for clarity in tests
constexpr std::size_t kNoPromotion = 0;  // for clarity in tests

Bitmove ComposeMove(const Bitmove source,
                    const Bitmove target,
                    const Bitmove moved_piece,
                    const Bitmove captured_piece,
                    const Bitmove promotion,
                    const Bitmove move_type);

std::string ToString(const Bitmove move);

Bitmove ExtractSource(const Bitmove move);
Bitmove ExtractTarget(const Bitmove move);
Bitmove ExtractMovedPiece(const Bitmove move);
Bitmove ExtractCapturedPiece(const Bitmove move);
Bitmove ExtractPromotion(const Bitmove move);
Bitmove ExtractType(const Bitmove move);

}  // namespace Chess

#endif
