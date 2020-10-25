#ifndef BITBOARD_SQUARES_H
#define BITBOARD_SQUARES_H

#include "bitboard/basic_type_declarations.h"

#include <limits>
#include <array>

namespace Chess
{

constexpr Bitboard XX = 0;  // empty bitboard

constexpr Bitboard H1 = Bitboard{1} << 0;
constexpr Bitboard G1 = Bitboard{1} << 1;
constexpr Bitboard F1 = Bitboard{1} << 2;
constexpr Bitboard E1 = Bitboard{1} << 3;
constexpr Bitboard D1 = Bitboard{1} << 4;
constexpr Bitboard C1 = Bitboard{1} << 5;
constexpr Bitboard B1 = Bitboard{1} << 6;
constexpr Bitboard A1 = Bitboard{1} << 7;

constexpr Bitboard H2 = Bitboard{1} << 8;
constexpr Bitboard G2 = Bitboard{1} << 9;
constexpr Bitboard F2 = Bitboard{1} << 10;
constexpr Bitboard E2 = Bitboard{1} << 11;
constexpr Bitboard D2 = Bitboard{1} << 12;
constexpr Bitboard C2 = Bitboard{1} << 13;
constexpr Bitboard B2 = Bitboard{1} << 14;
constexpr Bitboard A2 = Bitboard{1} << 15;

constexpr Bitboard H3 = Bitboard{1} << 16;
constexpr Bitboard G3 = Bitboard{1} << 17;
constexpr Bitboard F3 = Bitboard{1} << 18;
constexpr Bitboard E3 = Bitboard{1} << 19;
constexpr Bitboard D3 = Bitboard{1} << 20;
constexpr Bitboard C3 = Bitboard{1} << 21;
constexpr Bitboard B3 = Bitboard{1} << 22;
constexpr Bitboard A3 = Bitboard{1} << 23;

constexpr Bitboard H4 = Bitboard{1} << 24;
constexpr Bitboard G4 = Bitboard{1} << 25;
constexpr Bitboard F4 = Bitboard{1} << 26;
constexpr Bitboard E4 = Bitboard{1} << 27;
constexpr Bitboard D4 = Bitboard{1} << 28;
constexpr Bitboard C4 = Bitboard{1} << 29;
constexpr Bitboard B4 = Bitboard{1} << 30;
constexpr Bitboard A4 = Bitboard{1} << 31;

constexpr Bitboard H5 = Bitboard{1} << 32;
constexpr Bitboard G5 = Bitboard{1} << 33;
constexpr Bitboard F5 = Bitboard{1} << 34;
constexpr Bitboard E5 = Bitboard{1} << 35;
constexpr Bitboard D5 = Bitboard{1} << 36;
constexpr Bitboard C5 = Bitboard{1} << 37;
constexpr Bitboard B5 = Bitboard{1} << 38;
constexpr Bitboard A5 = Bitboard{1} << 39;

constexpr Bitboard H6 = Bitboard{1} << 40;
constexpr Bitboard G6 = Bitboard{1} << 41;
constexpr Bitboard F6 = Bitboard{1} << 42;
constexpr Bitboard E6 = Bitboard{1} << 43;
constexpr Bitboard D6 = Bitboard{1} << 44;
constexpr Bitboard C6 = Bitboard{1} << 45;
constexpr Bitboard B6 = Bitboard{1} << 46;
constexpr Bitboard A6 = Bitboard{1} << 47;

constexpr Bitboard H7 = Bitboard{1} << 48;
constexpr Bitboard G7 = Bitboard{1} << 49;
constexpr Bitboard F7 = Bitboard{1} << 50;
constexpr Bitboard E7 = Bitboard{1} << 51;
constexpr Bitboard D7 = Bitboard{1} << 52;
constexpr Bitboard C7 = Bitboard{1} << 53;
constexpr Bitboard B7 = Bitboard{1} << 54;
constexpr Bitboard A7 = Bitboard{1} << 55;

constexpr Bitboard H8 = Bitboard{1} << 56;
constexpr Bitboard G8 = Bitboard{1} << 57;
constexpr Bitboard F8 = Bitboard{1} << 58;
constexpr Bitboard E8 = Bitboard{1} << 59;
constexpr Bitboard D8 = Bitboard{1} << 60;
constexpr Bitboard C8 = Bitboard{1} << 61;
constexpr Bitboard B8 = Bitboard{1} << 62;
constexpr Bitboard A8 = Bitboard{1} << 63;

constexpr Bitboard START_RANK_WHITE = A2 | B2 | C2 | D2 | E2 | F2 | G2 | H2;
constexpr Bitboard START_RANK_BLACK = A7 | B7 | C7 | D7 | E7 | F7 | G7 | H7;
constexpr Bitboard PROMOTION_RANKS = A8 | B8 | C8 | D8 | E8 | F8 | G8 | H8 | A1 | B1 | C1 | D1 | E1 | F1 | G1 | H1;

constexpr std::array<const char* const, 64> SQUARE_LABEL{
    "H1", "G1", "F1", "E1", "D1", "C1", "B1", "A1",  //
    "H2", "G2", "F2", "E2", "D2", "C2", "B2", "A2",  //
    "H3", "G3", "F3", "E3", "D3", "C3", "B3", "A3",  //
    "H4", "G4", "F4", "E4", "D4", "C4", "B4", "A4",  //
    "H5", "G5", "F5", "E5", "D5", "C5", "B5", "A5",  //
    "H6", "G6", "F6", "E6", "D6", "C6", "B6", "A6",  //
    "H7", "G7", "F7", "E7", "D7", "C7", "B7", "A7",  //
    "H8", "G8", "F8", "E8", "D8", "C8", "B8", "A8"   //
};

}  // namespace Chess

#endif
