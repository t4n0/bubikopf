#ifndef BITBOARD_LOOKUP_TABLE_KNIGHT_H
#define BITBOARD_LOOKUP_TABLE_KNIGHT_H

#include "bitboard/basic_type_declarations.h"
#include "bitboard/lookup_table/utilities.h"
#include "bitboard/squares.h"

#include <array>

namespace Chess
{

/// @brief Indizes to acces various aspects of a knight jump
///
/// kNorthWest means two steps kNorth, one step kWest. In contrast west_north
/// means two steps kWest and one step kNorth.
constexpr std::size_t kKnightNorthWest = 0;
constexpr std::size_t kKnightNorthEast = 1;
constexpr std::size_t kKnightEastNorth = 2;
constexpr std::size_t kKnightEastSouth = 3;
constexpr std::size_t kKnightSouthEast = 4;
constexpr std::size_t kKnightSouthWest = 5;
constexpr std::size_t kKnightWestSouth = 6;
constexpr std::size_t kKnightWestNorth = 7;

constexpr std::array<std::size_t, 8> kKnightDirections{kKnightNorthWest,
                                                       kKnightNorthEast,
                                                       kKnightEastNorth,
                                                       kKnightEastSouth,
                                                       kKnightSouthEast,
                                                       kKnightSouthWest,
                                                       kKnightWestSouth,
                                                       kKnightWestNorth};

// Values provide the necessary bitshift for respective knight jump. E.g.
// kJumpBits.at(kKnightWestNorth).
constexpr std::array<int, 8> kJumpBits{17, 15, 6, -10, -17, -15, -6, 10};

// Values priovdes a bitboard specifying the legal landing area after a jump.
// (Used to null illegal moves that wrap around the board). E.g.
// legal_jump_areas_without_wrapping.at(kKnightWestNorth).
constexpr std::array<Bitboard, 8> kLegalLandingAreasWithoutWrapping{~(kRank1 | kRank2 | kFileH),
                                                                    ~(kRank1 | kRank2 | kFileA),
                                                                    ~(kFileA | kFileB | kRank1),
                                                                    ~(kFileA | kFileB | kRank8),
                                                                    ~(kRank7 | kRank8 | kFileA),
                                                                    ~(kRank7 | kRank8 | kFileH),
                                                                    ~(kFileH | kFileG | kRank8),
                                                                    ~(kFileH | kFileG | kRank1)};

/// @brief Creates a bitboard from source_bit, shifts it in direction and NULLs
/// the board if "wrap around" occurs.
/// @pre Assumes that only a single bit is set
template <int source_bit, std::size_t direction>
struct KnightJump
{
    constexpr static Bitboard value =
        std::get<direction>(kLegalLandingAreasWithoutWrapping) &
        (std::get<direction>(kJumpBits) > 0
             ? ToBitboard<source_bit>::value << std::get<direction>(kJumpBits)
             : ToBitboard<source_bit>::value >> Absolute<std::get<direction>(kJumpBits)>::value);
};

template <int source_bit, std::size_t direction = 0>
struct KnightJumps
{
    constexpr static Bitboard value =
        KnightJump<source_bit, direction>::value | KnightJumps<source_bit, direction + 1>::value;
};

template <int source_bit>
struct KnightJumps<source_bit, 8>
{
    constexpr static Bitboard value = 0;
};

/// @brief When accessing this lookup table with the source bit as index (e.g. 0
/// is H1 , 7 is A1, etc.) a bitboard is returned with all possible landing
/// squares for a knight jump
constexpr std::array<Bitboard, 64> kKnightJumps{
    KnightJumps<0>::value,  KnightJumps<1>::value,  KnightJumps<2>::value,  KnightJumps<3>::value,
    KnightJumps<4>::value,  KnightJumps<5>::value,  KnightJumps<6>::value,  KnightJumps<7>::value,
    KnightJumps<8>::value,  KnightJumps<9>::value,  KnightJumps<10>::value, KnightJumps<11>::value,
    KnightJumps<12>::value, KnightJumps<13>::value, KnightJumps<14>::value, KnightJumps<15>::value,
    KnightJumps<16>::value, KnightJumps<17>::value, KnightJumps<18>::value, KnightJumps<19>::value,
    KnightJumps<20>::value, KnightJumps<21>::value, KnightJumps<22>::value, KnightJumps<23>::value,
    KnightJumps<24>::value, KnightJumps<25>::value, KnightJumps<26>::value, KnightJumps<27>::value,
    KnightJumps<28>::value, KnightJumps<29>::value, KnightJumps<30>::value, KnightJumps<31>::value,
    KnightJumps<32>::value, KnightJumps<33>::value, KnightJumps<34>::value, KnightJumps<35>::value,
    KnightJumps<36>::value, KnightJumps<37>::value, KnightJumps<38>::value, KnightJumps<39>::value,
    KnightJumps<40>::value, KnightJumps<41>::value, KnightJumps<42>::value, KnightJumps<43>::value,
    KnightJumps<44>::value, KnightJumps<45>::value, KnightJumps<46>::value, KnightJumps<47>::value,
    KnightJumps<48>::value, KnightJumps<49>::value, KnightJumps<50>::value, KnightJumps<51>::value,
    KnightJumps<52>::value, KnightJumps<53>::value, KnightJumps<54>::value, KnightJumps<55>::value,
    KnightJumps<56>::value, KnightJumps<57>::value, KnightJumps<58>::value, KnightJumps<59>::value,
    KnightJumps<60>::value, KnightJumps<61>::value, KnightJumps<62>::value, KnightJumps<63>::value};

}  // namespace Chess

#endif
