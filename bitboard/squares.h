#ifndef BITBOARD_SQUARES_H
#define BITBOARD_SQUARES_H

#include "bitboard/basic_type_declarations.h"

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

constexpr Bitboard kRank1 = A1 | B1 | C1 | D1 | E1 | F1 | G1 | H1;
constexpr Bitboard kRank2 = A2 | B2 | C2 | D2 | E2 | F2 | G2 | H2;
constexpr Bitboard kRank3 = A3 | B3 | C3 | D3 | E3 | F3 | G3 | H3;
constexpr Bitboard kRank4 = A4 | B4 | C4 | D4 | E4 | F4 | G4 | H4;
constexpr Bitboard kRank5 = A5 | B5 | C5 | D5 | E5 | F5 | G5 | H5;
constexpr Bitboard kRank6 = A6 | B6 | C6 | D6 | E6 | F6 | G6 | H6;
constexpr Bitboard kRank7 = A7 | B7 | C7 | D7 | E7 | F7 | G7 | H7;
constexpr Bitboard kRank8 = A8 | B8 | C8 | D8 | E8 | F8 | G8 | H8;

constexpr Bitboard kFileA = A1 | A2 | A3 | A4 | A5 | A6 | A7 | A8;
constexpr Bitboard kFileB = B1 | B2 | B3 | B4 | B5 | B6 | B7 | B8;
constexpr Bitboard kFileC = C1 | C2 | C3 | C4 | C5 | C6 | C7 | C8;
constexpr Bitboard kFileD = D1 | D2 | D3 | D4 | D5 | D6 | D7 | D8;
constexpr Bitboard kFileE = E1 | E2 | E3 | E4 | E5 | E6 | E7 | E8;
constexpr Bitboard kFileF = F1 | F2 | F3 | F4 | F5 | F6 | F7 | F8;
constexpr Bitboard kFileG = G1 | G2 | G3 | G4 | G5 | G6 | G7 | G8;
constexpr Bitboard kFileH = H1 | H2 | H3 | H4 | H5 | H6 | H7 | H8;

constexpr Bitboard kStartRankWhite = kRank2;
constexpr Bitboard kStartRankBlack = kRank7;
constexpr Bitboard kPromotionRanks = kRank1 | kRank8;

constexpr std::array<Bitboard, 64> kAllSquares{H1, G1, F1, E1, D1, C1, B1, A1,  //
                                               H2, G2, F2, E2, D2, C2, B2, A2,  //
                                               H3, G3, F3, E3, D3, C3, B3, A3,  //
                                               H4, G4, F4, E4, D4, C4, B4, A4,  //
                                               H5, G5, F5, E5, D5, C5, B5, A5,  //
                                               H6, G6, F6, E6, D6, C6, B6, A6,  //
                                               H7, G7, F7, E7, D7, C7, B7, A7,  //
                                               H8, G8, F8, E8, D8, C8, B8, A8};

constexpr std::array<const char* const, 64> kSquareLabels{"h1", "g1", "f1", "e1", "d1", "c1", "b1", "a1",  //
                                                          "h2", "g2", "f2", "e2", "d2", "c2", "b2", "a2",  //
                                                          "h3", "g3", "f3", "e3", "d3", "c3", "b3", "a3",  //
                                                          "h4", "g4", "f4", "e4", "d4", "c4", "b4", "a4",  //
                                                          "h5", "g5", "f5", "e5", "d5", "c5", "b5", "a5",  //
                                                          "h6", "g6", "f6", "e6", "d6", "c6", "b6", "a6",  //
                                                          "h7", "g7", "f7", "e7", "d7", "c7", "b7", "a7",  //
                                                          "h8", "g8", "f8", "e8", "d8", "c8", "b8", "a8"};

}  // namespace Chess

#endif
