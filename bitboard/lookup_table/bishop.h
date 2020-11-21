#ifndef BITBOARD_LOOKUP_TABLE_BISHOP_H
#define BITBOARD_LOOKUP_TABLE_BISHOP_H

#include "bitboard/lookup_table/ray.h"

#include <array>

namespace Chess
{

template <Bitboard source>
struct BishopAttacks
{
    constexpr static Bitboard value =
        RaySevenSquares<source, north_west>::value | RaySevenSquares<source, north_east>::value |
        RaySevenSquares<source, south_west>::value | RaySevenSquares<source, south_east>::value;
};

constexpr std::array<Bitboard, 64> bishop_attacks{
    BishopAttacks<H1>::value,  //
    BishopAttacks<G1>::value,  //
    BishopAttacks<F1>::value,  //
    BishopAttacks<E1>::value,  //
    BishopAttacks<D1>::value,  //
    BishopAttacks<C1>::value,  //
    BishopAttacks<B1>::value,  //
    BishopAttacks<A1>::value,  //

    BishopAttacks<H2>::value,  //
    BishopAttacks<G2>::value,  //
    BishopAttacks<F2>::value,  //
    BishopAttacks<E2>::value,  //
    BishopAttacks<D2>::value,  //
    BishopAttacks<C2>::value,  //
    BishopAttacks<B2>::value,  //
    BishopAttacks<A2>::value,  //

    BishopAttacks<H3>::value,  //
    BishopAttacks<G3>::value,  //
    BishopAttacks<F3>::value,  //
    BishopAttacks<E3>::value,  //
    BishopAttacks<D3>::value,  //
    BishopAttacks<C3>::value,  //
    BishopAttacks<B3>::value,  //
    BishopAttacks<A3>::value,  //

    BishopAttacks<H4>::value,  //
    BishopAttacks<G4>::value,  //
    BishopAttacks<F4>::value,  //
    BishopAttacks<E4>::value,  //
    BishopAttacks<D4>::value,  //
    BishopAttacks<C4>::value,  //
    BishopAttacks<B4>::value,  //
    BishopAttacks<A4>::value,  //

    BishopAttacks<H5>::value,  //
    BishopAttacks<G5>::value,  //
    BishopAttacks<F5>::value,  //
    BishopAttacks<E5>::value,  //
    BishopAttacks<D5>::value,  //
    BishopAttacks<C5>::value,  //
    BishopAttacks<B5>::value,  //
    BishopAttacks<A5>::value,  //

    BishopAttacks<H6>::value,  //
    BishopAttacks<G6>::value,  //
    BishopAttacks<F6>::value,  //
    BishopAttacks<E6>::value,  //
    BishopAttacks<D6>::value,  //
    BishopAttacks<C6>::value,  //
    BishopAttacks<B6>::value,  //
    BishopAttacks<A6>::value,  //

    BishopAttacks<H7>::value,  //
    BishopAttacks<G7>::value,  //
    BishopAttacks<F7>::value,  //
    BishopAttacks<E7>::value,  //
    BishopAttacks<D7>::value,  //
    BishopAttacks<C7>::value,  //
    BishopAttacks<B7>::value,  //
    BishopAttacks<A7>::value,  //

    BishopAttacks<H8>::value,  //
    BishopAttacks<G8>::value,  //
    BishopAttacks<F8>::value,  //
    BishopAttacks<E8>::value,  //
    BishopAttacks<D8>::value,  //
    BishopAttacks<C8>::value,  //
    BishopAttacks<B8>::value,  //
    BishopAttacks<A8>::value,  //
};

}  // namespace Chess

#endif
