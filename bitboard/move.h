#ifndef BITBOARD_MOVE_H
#define BITBOARD_MOVE_H

#include "bitboard/basic_type_declarations.h"
#include "bitboard/pieces.h"
#include "bitboard/squares.h"

#include <array>
#include <sstream>

namespace Chess
{

constexpr Bitmove kBitNullMove = 0;

// clang-format off
constexpr Bitmove kMoveMaskSource =             0b00000000'00000000'00000000'00111111;
constexpr Bitmove kMoveMaskTarget =             0b00000000'00000000'00001111'11000000;
constexpr Bitmove kMoveMaskMovedPiece =         0b00000000'00011100'00000000'00000000;
constexpr Bitmove kMoveMaskCapturedPiece =      0b00000000'11100000'00000000'00000000;
constexpr Bitmove kMoveMaskPromotion =          0b00000111'00000000'00000000'00000000;
constexpr Bitmove kMoveMaskType =               0b01111000'00000000'00000000'00000000;
constexpr Bitmove kMoveMaskUnused =             0b10000000'00000011'11110000'00000000;

constexpr Bitmove kMoveTypeQuietNonPawn =       0b00000000'00000000'00000000'00000000;
constexpr Bitmove kMoveTypeCapture =            0b00001000'00000000'00000000'00000000;
constexpr Bitmove kMoveTypePawnSinglePush =     0b00010000'00000000'00000000'00000000;
constexpr Bitmove kMoveTypePawnDoublePush =     0b00011000'00000000'00000000'00000000;
constexpr Bitmove kMoveTypeEnPassantCapture =   0b00100000'00000000'00000000'00000000;
constexpr Bitmove kMoveTypeKingsideCastling =   0b00101000'00000000'00000000'00000000;
constexpr Bitmove kMoveTypeQueensideCastling =  0b00110000'00000000'00000000'00000000;
constexpr Bitmove kMoveTypePromotion =          0b00111000'00000000'00000000'00000000;
// clang-format on

constexpr std::array<const char* const, 8> kMoveTypeLabels = {
    "QUIET_NON_PAWN",
    "CAPTURE",
    "PAWN_PUSH",
    "PAWN_DOUBLE_PUSH",
    "EN_PASSANT_CAPTURE",
    "KINGSIDE_CASTLING",
    "QUEENSIDE_CASTLING",
    "PROMOTION",
};

constexpr int kMoveShiftTarget = 6;
constexpr int kMoveShiftStaticPlies = 12;
constexpr int kMoveShiftMovedPiece = 18;
constexpr int kMoveShiftCapturedPiece = 21;
constexpr int kMoveShiftPromotion = 24;
constexpr int kMoveShiftType = 27;

constexpr std::size_t kNoCapture = 0;    // for clarity in tests
constexpr std::size_t kNoPromotion = 0;  // for clarity in tests

inline Bitmove ComposeMove(const Bitmove source,
                           const Bitmove target,
                           const Bitmove moved_piece,
                           const Bitmove captured_piece,
                           const Bitmove promotion,
                           const Bitmove move_type)
{
    return source |                                       //
           (target << kMoveShiftTarget) |                 //
           (moved_piece << kMoveShiftMovedPiece) |        //
           (captured_piece << kMoveShiftCapturedPiece) |  //
           (promotion << kMoveShiftPromotion) |           //
           move_type;
}

inline std::string ToString(const Bitmove& move)
{
    std::stringstream move_print_out{};

    const bool move_is_empty = !move;
    if (move_is_empty)
    {
        return {};
    }

    const std::size_t source_bit = move & kMoveMaskSource;
    const std::string source = kSquareLabels.at(source_bit);
    const std::size_t target_bit = (move & kMoveMaskTarget) >> kMoveShiftTarget;
    const std::string target = kSquareLabels.at(target_bit);
    const std::size_t moved_piece_kind = (move & kMoveMaskMovedPiece) >> kMoveShiftMovedPiece;
    const char moved_piece = kPieceLabels.at(moved_piece_kind);
    const std::size_t captured_piece_kind = (move & kMoveMaskCapturedPiece) >> kMoveShiftCapturedPiece;
    const char captured_piece = kPieceLabels.at(captured_piece_kind);
    const std::size_t promotion_kind = (move & kMoveMaskPromotion) >> kMoveShiftPromotion;
    const char promotion = kPieceLabels.at(promotion_kind);
    const std::size_t move_value_kind = (move & kMoveMaskType) >> kMoveShiftType;
    const std::string move_value = kMoveTypeLabels.at(move_value_kind);

    move_print_out << moved_piece << " on " << source << " to " << target << " promoting to " << promotion
                   << " capturing " << captured_piece << " with move value " << move_value << "\n";

    return move_print_out.str();
}

}  // namespace Chess

#endif
