#ifndef BITBOARD_LOOKUP_TABLE_PIECE_H
#define BITBOARD_LOOKUP_TABLE_PIECE_H

#include "bitboard/lookup_table/ray.h"

#include <array>

namespace Chess
{

template <Bitboard source>
struct BishopAttacks
{
    constexpr static Bitboard value =
        RaySevenSquares<source, kNorthWest>::value | RaySevenSquares<source, kNorthEast>::value |
        RaySevenSquares<source, kSouthWest>::value | RaySevenSquares<source, kSouthEast>::value;
};

constexpr auto kBishopAttacks = LoopOverAllSquares<BishopAttacks>();

template <Bitboard target>
struct KingAttacks
{
    constexpr static Bitboard value = RayOneSquare<target, kNorth>::value | RayOneSquare<target, kNorthWest>::value |
                                      RayOneSquare<target, kWest>::value | RayOneSquare<target, kNorthEast>::value |
                                      RayOneSquare<target, kSouth>::value | RayOneSquare<target, kSouthWest>::value |
                                      RayOneSquare<target, kEast>::value | RayOneSquare<target, kSouthEast>::value;
};

constexpr auto kKingAttacks = LoopOverAllSquares<KingAttacks>();

template <Bitboard source>
struct RookAttacks
{
    constexpr static Bitboard value = RaySevenSquares<source, kNorth>::value | RaySevenSquares<source, kWest>::value |
                                      RaySevenSquares<source, kSouth>::value | RaySevenSquares<source, kEast>::value;
};

constexpr auto kRookAttacks = LoopOverAllSquares<RookAttacks>();

}  // namespace Chess

#endif
