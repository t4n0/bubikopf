#ifndef BITBOARD_MOVE_LOOKUP_TABLE_H
#define BITBOARD_MOVE_LOOKUP_TABLE_H

#include "bitboard/basic_type_declarations.h"
#include "bitboard/squares.h"

#include <array>

namespace Chess
{

constexpr std::size_t PAWN_CAPTURE_LOOKUP_TABLE_OFFSET_FOR_SECOND_OPTION = 56;
constexpr std::size_t PAWN_CAPUTRE_LOOKUP_TABLE_OFFSET_FOR_BLACK =
    104;  // This is actually less than the actual number of elements for white
          // move options. 8 less exactly. This exploits the fact that black
          // pawns never occupy the first rank. So the actually accessed indeces
          // for black only start from 8 onwards.

/// Index corresponds to source bit. I.e. value at(0) corresponds to "first
/// target bitboard for white pawn on H1".
/// at(0+SECOND_OPTION_OFFSET) corresponds to "second target bitboard for white
/// pawn on H1". As there is no second option for pawn on H1 the square XX
/// (invalid) is accessed.
constexpr std::array<Bitboard, 232>
    pawn_capture_lookup_table{
        // clang-format off
    G2, F2, E2, D2, C2, B2, A2, B2,
    G3, F3, E3, D3, C3, B3, A3, B3,
    G4, F4, E4, D4, C4, B4, A4, B4,
    G5, F5, E5, D5, C5, B5, A5, B5,
    G6, F6, E6, D6, C6, B6, A6, B6,
    G7, F7, E7, D7, C7, B7, A7, B7,
    G8, F8, E8, D8, C8, B8, A8, B8,
    XX, H2, G2, F2, E2, D2, C2, XX,
    XX, H3, G3, F3, E3, D3, C3, XX,
    XX, H4, G4, F4, E4, D4, C4, XX,
    XX, H5, G5, F5, E5, D5, C5, XX,
    XX, H6, G6, F6, E6, D6, C6, XX,
    XX, H7, G7, F7, E7, D7, C7, XX,
    XX, H8, G8, F8, E8, D8, C8, XX, // previous entries are white pawn options.
    G1, F1, E1, D1, C1, B1, A1, B1,
    G2, F2, E2, D2, C2, B2, A2, B2,
    G3, F3, E3, D3, C3, B3, A3, B3,
    G4, F4, E4, D4, C4, B4, A4, B4,
    G5, F5, E5, D5, C5, B5, A5, B5,
    G6, F6, E6, D6, C6, B6, A6, B6,
    G7, F7, E7, D7, C7, B7, A7, B7,
    XX, H1, G1, F1, E1, D1, C1, XX,
    XX, H2, G2, F2, E2, D2, C2, XX,
    XX, H3, G3, F3, E3, D3, C3, XX,
    XX, H4, G4, F4, E4, D4, C4, XX,
    XX, H5, G5, F5, E5, D5, C5, XX,
    XX, H6, G6, F6, E6, D6, C6, XX,
    XX, H7, G7, F7, E7, D7, C7, XX,
        // clang-format on
    };

}  // namespace Chess

#endif
