#ifndef BITBOARD_SQUARES_H
#define BITBOARD_SQUARES_H

#include "bitboard/basic_type_declarations.h"

#include <limits>
#include <array>

namespace Chess
{

constexpr Bitboard XX = 0;  // empty bitboard

constexpr Bitboard H1 = 1ULL << 0;
constexpr Bitboard G1 = 1ULL << 1;
constexpr Bitboard F1 = 1ULL << 2;
constexpr Bitboard E1 = 1ULL << 3;
constexpr Bitboard D1 = 1ULL << 4;
constexpr Bitboard C1 = 1ULL << 5;
constexpr Bitboard B1 = 1ULL << 6;
constexpr Bitboard A1 = 1ULL << 7;

constexpr Bitboard H2 = 1ULL << 8;
constexpr Bitboard G2 = 1ULL << 9;
constexpr Bitboard F2 = 1ULL << 10;
constexpr Bitboard E2 = 1ULL << 11;
constexpr Bitboard D2 = 1ULL << 12;
constexpr Bitboard C2 = 1ULL << 13;
constexpr Bitboard B2 = 1ULL << 14;
constexpr Bitboard A2 = 1ULL << 15;

constexpr Bitboard H3 = 1ULL << 16;
constexpr Bitboard G3 = 1ULL << 17;
constexpr Bitboard F3 = 1ULL << 18;
constexpr Bitboard E3 = 1ULL << 19;
constexpr Bitboard D3 = 1ULL << 20;
constexpr Bitboard C3 = 1ULL << 21;
constexpr Bitboard B3 = 1ULL << 22;
constexpr Bitboard A3 = 1ULL << 23;

constexpr Bitboard H4 = 1ULL << 24;
constexpr Bitboard G4 = 1ULL << 25;
constexpr Bitboard F4 = 1ULL << 26;
constexpr Bitboard E4 = 1ULL << 27;
constexpr Bitboard D4 = 1ULL << 28;
constexpr Bitboard C4 = 1ULL << 29;
constexpr Bitboard B4 = 1ULL << 30;
constexpr Bitboard A4 = 1ULL << 31;

constexpr Bitboard H5 = 1ULL << 32;
constexpr Bitboard G5 = 1ULL << 33;
constexpr Bitboard F5 = 1ULL << 34;
constexpr Bitboard E5 = 1ULL << 35;
constexpr Bitboard D5 = 1ULL << 36;
constexpr Bitboard C5 = 1ULL << 37;
constexpr Bitboard B5 = 1ULL << 38;
constexpr Bitboard A5 = 1ULL << 39;

constexpr Bitboard H6 = 1ULL << 40;
constexpr Bitboard G6 = 1ULL << 41;
constexpr Bitboard F6 = 1ULL << 42;
constexpr Bitboard E6 = 1ULL << 43;
constexpr Bitboard D6 = 1ULL << 44;
constexpr Bitboard C6 = 1ULL << 45;
constexpr Bitboard B6 = 1ULL << 46;
constexpr Bitboard A6 = 1ULL << 47;

constexpr Bitboard H7 = 1ULL << 48;
constexpr Bitboard G7 = 1ULL << 49;
constexpr Bitboard F7 = 1ULL << 50;
constexpr Bitboard E7 = 1ULL << 51;
constexpr Bitboard D7 = 1ULL << 52;
constexpr Bitboard C7 = 1ULL << 53;
constexpr Bitboard B7 = 1ULL << 54;
constexpr Bitboard A7 = 1ULL << 55;

constexpr Bitboard H8 = 1ULL << 56;
constexpr Bitboard G8 = 1ULL << 57;
constexpr Bitboard F8 = 1ULL << 58;
constexpr Bitboard E8 = 1ULL << 59;
constexpr Bitboard D8 = 1ULL << 60;
constexpr Bitboard C8 = 1ULL << 61;
constexpr Bitboard B8 = 1ULL << 62;
constexpr Bitboard A8 = 1ULL << 63;

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
