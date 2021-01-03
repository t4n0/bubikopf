#ifndef BITBOARD_LOOKUP_TABLE_ROOK_H
#define BITBOARD_LOOKUP_TABLE_ROOK_H

#include "bitboard/lookup_table/ray.h"

#include <array>

namespace Chess
{

template <Bitboard source>
struct RookAttacks
{
    constexpr static Bitboard value = RaySevenSquares<source, kNorth>::value | RaySevenSquares<source, kWest>::value |
                                      RaySevenSquares<source, kSouth>::value | RaySevenSquares<source, kEast>::value;
};

constexpr std::array<Bitboard, 64> kRookAttacks{
    RookAttacks<H1>::value,  //
    RookAttacks<G1>::value,  //
    RookAttacks<F1>::value,  //
    RookAttacks<E1>::value,  //
    RookAttacks<D1>::value,  //
    RookAttacks<C1>::value,  //
    RookAttacks<B1>::value,  //
    RookAttacks<A1>::value,  //

    RookAttacks<H2>::value,  //
    RookAttacks<G2>::value,  //
    RookAttacks<F2>::value,  //
    RookAttacks<E2>::value,  //
    RookAttacks<D2>::value,  //
    RookAttacks<C2>::value,  //
    RookAttacks<B2>::value,  //
    RookAttacks<A2>::value,  //

    RookAttacks<H3>::value,  //
    RookAttacks<G3>::value,  //
    RookAttacks<F3>::value,  //
    RookAttacks<E3>::value,  //
    RookAttacks<D3>::value,  //
    RookAttacks<C3>::value,  //
    RookAttacks<B3>::value,  //
    RookAttacks<A3>::value,  //

    RookAttacks<H4>::value,  //
    RookAttacks<G4>::value,  //
    RookAttacks<F4>::value,  //
    RookAttacks<E4>::value,  //
    RookAttacks<D4>::value,  //
    RookAttacks<C4>::value,  //
    RookAttacks<B4>::value,  //
    RookAttacks<A4>::value,  //

    RookAttacks<H5>::value,  //
    RookAttacks<G5>::value,  //
    RookAttacks<F5>::value,  //
    RookAttacks<E5>::value,  //
    RookAttacks<D5>::value,  //
    RookAttacks<C5>::value,  //
    RookAttacks<B5>::value,  //
    RookAttacks<A5>::value,  //

    RookAttacks<H6>::value,  //
    RookAttacks<G6>::value,  //
    RookAttacks<F6>::value,  //
    RookAttacks<E6>::value,  //
    RookAttacks<D6>::value,  //
    RookAttacks<C6>::value,  //
    RookAttacks<B6>::value,  //
    RookAttacks<A6>::value,  //

    RookAttacks<H7>::value,  //
    RookAttacks<G7>::value,  //
    RookAttacks<F7>::value,  //
    RookAttacks<E7>::value,  //
    RookAttacks<D7>::value,  //
    RookAttacks<C7>::value,  //
    RookAttacks<B7>::value,  //
    RookAttacks<A7>::value,  //

    RookAttacks<H8>::value,  //
    RookAttacks<G8>::value,  //
    RookAttacks<F8>::value,  //
    RookAttacks<E8>::value,  //
    RookAttacks<D8>::value,  //
    RookAttacks<C8>::value,  //
    RookAttacks<B8>::value,  //
    RookAttacks<A8>::value,  //
};

}  // namespace Chess

#endif
