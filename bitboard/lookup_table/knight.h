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
// kJumpBits[kKnightWestNorth].
constexpr std::array<int, 8> kJumpBits{17, 15, 6, -10, -17, -15, -6, 10};

// Values priovdes a bitboard specifying the legal landing area after a jump.
// (Used to null illegal moves that wrap around the board). E.g.
// legal_jump_areas_without_wrapping[kKnightWestNorth].
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
template <Bitboard source, std::size_t direction>
struct KnightJump
{
    constexpr static Bitboard value =
        std::get<direction>(kLegalLandingAreasWithoutWrapping) &
        (std::get<direction>(kJumpBits) > 0 ? source << std::get<direction>(kJumpBits)
                                            : source >> Absolute<std::get<direction>(kJumpBits)>::value);
};

template <Bitboard source, std::size_t direction = 0>
struct KnightJumps
{
    constexpr static Bitboard value = KnightJump<source, direction>::value | KnightJumps<source, direction + 1>::value;
};

template <Bitboard source>
struct KnightJumps<source, 7>
{
    constexpr static Bitboard value = KnightJump<source, 7>::value;
};

constexpr auto kKnightJumps = LoopOverAllSquares<KnightJumps>();

}  // namespace Chess

#endif
