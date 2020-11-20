#ifndef BITBOARD_LOOKUP_TABLE_KING_H
#define BITBOARD_LOOKUP_TABLE_KING_H

#include "bitboard/lookup_table/ray.h"

#include <array>

namespace Chess
{

template <Bitboard target>
struct KingAttacks
{
    constexpr static Bitboard value = RayOneSquare<target, north>::value | RayOneSquare<target, north_west>::value |
                                      RayOneSquare<target, west>::value | RayOneSquare<target, north_east>::value |
                                      RayOneSquare<target, south>::value | RayOneSquare<target, south_west>::value |
                                      RayOneSquare<target, east>::value | RayOneSquare<target, south_east>::value;
};

constexpr std::array<Bitboard, 64> king_attacks{
    KingAttacks<H1>::value,  //
    KingAttacks<G1>::value,  //
    KingAttacks<F1>::value,  //
    KingAttacks<E1>::value,  //
    KingAttacks<D1>::value,  //
    KingAttacks<C1>::value,  //
    KingAttacks<B1>::value,  //
    KingAttacks<A1>::value,  //

    KingAttacks<H2>::value,  //
    KingAttacks<G2>::value,  //
    KingAttacks<F2>::value,  //
    KingAttacks<E2>::value,  //
    KingAttacks<D2>::value,  //
    KingAttacks<C2>::value,  //
    KingAttacks<B2>::value,  //
    KingAttacks<A2>::value,  //

    KingAttacks<H3>::value,  //
    KingAttacks<G3>::value,  //
    KingAttacks<F3>::value,  //
    KingAttacks<E3>::value,  //
    KingAttacks<D3>::value,  //
    KingAttacks<C3>::value,  //
    KingAttacks<B3>::value,  //
    KingAttacks<A3>::value,  //

    KingAttacks<H4>::value,  //
    KingAttacks<G4>::value,  //
    KingAttacks<F4>::value,  //
    KingAttacks<E4>::value,  //
    KingAttacks<D4>::value,  //
    KingAttacks<C4>::value,  //
    KingAttacks<B4>::value,  //
    KingAttacks<A4>::value,  //

    KingAttacks<H5>::value,  //
    KingAttacks<G5>::value,  //
    KingAttacks<F5>::value,  //
    KingAttacks<E5>::value,  //
    KingAttacks<D5>::value,  //
    KingAttacks<C5>::value,  //
    KingAttacks<B5>::value,  //
    KingAttacks<A5>::value,  //

    KingAttacks<H6>::value,  //
    KingAttacks<G6>::value,  //
    KingAttacks<F6>::value,  //
    KingAttacks<E6>::value,  //
    KingAttacks<D6>::value,  //
    KingAttacks<C6>::value,  //
    KingAttacks<B6>::value,  //
    KingAttacks<A6>::value,  //

    KingAttacks<H7>::value,  //
    KingAttacks<G7>::value,  //
    KingAttacks<F7>::value,  //
    KingAttacks<E7>::value,  //
    KingAttacks<D7>::value,  //
    KingAttacks<C7>::value,  //
    KingAttacks<B7>::value,  //
    KingAttacks<A7>::value,  //

    KingAttacks<H8>::value,  //
    KingAttacks<G8>::value,  //
    KingAttacks<F8>::value,  //
    KingAttacks<E8>::value,  //
    KingAttacks<D8>::value,  //
    KingAttacks<C8>::value,  //
    KingAttacks<B8>::value,  //
    KingAttacks<A8>::value,  //
};

}  // namespace Chess

#endif
