#ifndef BITBOARD_MOVE_H
#define BITBOARD_MOVE_H

#include "bitboard/basic_type_declarations.h"
#include "bitboard/pieces.h"
#include "bitboard/squares.h"

#include <array>
#include <limits>
#include <sstream>

namespace Chess
{

constexpr Bitmove MOVE_ONES = std::numeric_limits<Bitmove>::max();

// clang-format off
constexpr Bitmove MOVE_MASK_SOURCE =                   0b00000000'00000000'00000000'00111111;
constexpr Bitmove MOVE_MASK_TARGET =                   0b00000000'00000000'00001111'11000000;
constexpr Bitmove MOVE_MASK_MOVED_PIECE =              0b00000000'00011100'00000000'00000000;
constexpr Bitmove MOVE_MASK_CAPTURED_PIECE =           0b00000000'11100000'00000000'00000000;
constexpr Bitmove MOVE_MASK_PROMOTION =                0b00000111'00000000'00000000'00000000;
constexpr Bitmove MOVE_MASK_TYPE =                     0b01111000'00000000'00000000'00000000;
constexpr Bitmove MOVE_MASK_UNUSED =                   0b10000000'00000011'11110000'00000000;

constexpr Bitmove MOVE_VALUE_TYPE_QUIET_NON_PAWN =     0b00000000'00000000'00000000'00000000;
constexpr Bitmove MOVE_VALUE_TYPE_CAPTURE =            0b00001000'00000000'00000000'00000000;
constexpr Bitmove MOVE_VALUE_TYPE_PAWN_PUSH =          0b00010000'00000000'00000000'00000000;
constexpr Bitmove MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH =   0b00011000'00000000'00000000'00000000;
constexpr Bitmove MOVE_VALUE_TYPE_EN_PASSANT_CAPTURE = 0b00100000'00000000'00000000'00000000;
constexpr Bitmove MOVE_VALUE_TYPE_KINGSIDE_CASTLING =  0b00101000'00000000'00000000'00000000;
constexpr Bitmove MOVE_VALUE_TYPE_QUEENSIDE_CASTLING = 0b00110000'00000000'00000000'00000000;
constexpr Bitmove MOVE_VALUE_TYPE_PROMOTION =          0b00111000'00000000'00000000'00000000;
// clang-format on

constexpr std::array<const char* const, 8> MOVE_VALUE_LABELS = {
    "QUIET_NON_PAWN",
    "CAPTURE",
    "PAWN_PUSH",
    "PAWN_DOUBLE_PUSH",
    "EN_PASSANT_CAPTURE",
    "KINGSIDE_CASTLING",
    "QUEENSIDE_CASTLING",
    "PROMOTION",
};

constexpr int MOVE_SHIFT_TARGET = 6;
constexpr int MOVE_SHIFT_STATIC = 12;
constexpr int MOVE_SHIFT_MOVED_PIECE = 18;
constexpr int MOVE_SHIFT_CAPTURED_PIECE = 21;
constexpr int MOVE_SHIFT_PROMOTION = 24;
constexpr int MOVE_SHIFT_TYPE = 27;

constexpr std::size_t NO_CAPTURE = 0;    // for clarity in tests
constexpr std::size_t NO_PROMOTION = 0;  // for clarity in tests

inline Bitmove ComposeMove(const Bitmove source,
                           const Bitmove target,
                           const Bitmove moved_piece,
                           const Bitmove captured_piece,
                           const Bitmove promotion,
                           const Bitmove move_type)
{
    return source |                                         //
           (target << MOVE_SHIFT_TARGET) |                  //
           (moved_piece << MOVE_SHIFT_MOVED_PIECE) |        //
           (captured_piece << MOVE_SHIFT_CAPTURED_PIECE) |  //
           (promotion << MOVE_SHIFT_PROMOTION) |            //
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

    const std::size_t source_bit = move & MOVE_MASK_SOURCE;
    const std::string source = SQUARE_LABEL.at(source_bit);
    const std::size_t target_bit = (move & MOVE_MASK_TARGET) >> MOVE_SHIFT_TARGET;
    const std::string target = SQUARE_LABEL.at(target_bit);
    const std::size_t moved_piece_kind = (move & MOVE_MASK_MOVED_PIECE) >> MOVE_SHIFT_MOVED_PIECE;
    const std::string moved_piece = PIECE_LABEL.at(moved_piece_kind);
    const std::size_t captured_piece_kind = (move & MOVE_MASK_CAPTURED_PIECE) >> MOVE_SHIFT_CAPTURED_PIECE;
    const std::string captured_piece = PIECE_LABEL.at(captured_piece_kind);
    const std::size_t promotion_kind = (move & MOVE_MASK_PROMOTION) >> MOVE_SHIFT_PROMOTION;
    const std::string promotion = PIECE_LABEL.at(promotion_kind);
    const std::size_t move_value_kind = (move & MOVE_MASK_TYPE) >> MOVE_SHIFT_TYPE;
    const std::string move_value = MOVE_VALUE_LABELS.at(move_value_kind);

    move_print_out << moved_piece << " on " << source << " to " << target << " promoting to " << promotion
                   << " capturing " << captured_piece << " with move value " << move_value << "\n";

    return move_print_out.str();
}

inline std::string ToUciString(const Bitmove& move)
{
    const bool is_null_move = !move;
    if (is_null_move)
    {
        constexpr const char* const null_move = "0000";
        return null_move;
    }

    std::stringstream uci_move{};
    const std::size_t source_bit = move & MOVE_MASK_SOURCE;
    const std::size_t target_bit = (move & MOVE_MASK_TARGET) >> MOVE_SHIFT_TARGET;
    uci_move << SQUARE_LABEL.at(source_bit) << SQUARE_LABEL.at(target_bit);
    const std::size_t promotion_kind = (move & MOVE_MASK_PROMOTION) >> MOVE_SHIFT_PROMOTION;
    if (promotion_kind)
    {
        uci_move << PIECE_LABEL.at(promotion_kind);
    }

    return uci_move.str();
}

}  // namespace Chess

#endif
