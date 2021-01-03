#ifndef BITBOARD_LOOKUP_TABLE_PAWN_H
#define BITBOARD_LOOKUP_TABLE_PAWN_H

#include "bitboard/lookup_table/ray.h"

#include <array>

namespace Chess
{

constexpr std::size_t kPawnCaptureLookupTableOffsetForSecondOption = 56;
constexpr std::size_t kPawnCaptureLookupTableOffsetForBlack =
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
    kPawnCaptureLookupTable{
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

/// @brief Given an target bitboard the value of this struct represents a bitboard with the squares set that a black
/// pawn could attack from.
template <Bitboard target>
struct BlackPawnAttacks
{
    constexpr static Bitboard value = RayOneSquare<target, north_west>::value | RayOneSquare<target, north_east>::value;
};

/// @brief Given an target bitboard the value of this struct represents a bitboard with the squares set that a white
/// pawn could attack from.
template <Bitboard target>
struct WhitePawnAttacks
{
    constexpr static Bitboard value = RayOneSquare<target, south_west>::value | RayOneSquare<target, south_east>::value;
};

constexpr std::size_t kPawnAttacksOffsetForWhite = 64;

constexpr std::array<Bitboard, 128> kPawnAttacks{
    BlackPawnAttacks<H1>::value,  //
    BlackPawnAttacks<G1>::value,  //
    BlackPawnAttacks<F1>::value,  //
    BlackPawnAttacks<E1>::value,  //
    BlackPawnAttacks<D1>::value,  //
    BlackPawnAttacks<C1>::value,  //
    BlackPawnAttacks<B1>::value,  //
    BlackPawnAttacks<A1>::value,  //

    BlackPawnAttacks<H2>::value,  //
    BlackPawnAttacks<G2>::value,  //
    BlackPawnAttacks<F2>::value,  //
    BlackPawnAttacks<E2>::value,  //
    BlackPawnAttacks<D2>::value,  //
    BlackPawnAttacks<C2>::value,  //
    BlackPawnAttacks<B2>::value,  //
    BlackPawnAttacks<A2>::value,  //

    BlackPawnAttacks<H3>::value,  //
    BlackPawnAttacks<G3>::value,  //
    BlackPawnAttacks<F3>::value,  //
    BlackPawnAttacks<E3>::value,  //
    BlackPawnAttacks<D3>::value,  //
    BlackPawnAttacks<C3>::value,  //
    BlackPawnAttacks<B3>::value,  //
    BlackPawnAttacks<A3>::value,  //

    BlackPawnAttacks<H4>::value,  //
    BlackPawnAttacks<G4>::value,  //
    BlackPawnAttacks<F4>::value,  //
    BlackPawnAttacks<E4>::value,  //
    BlackPawnAttacks<D4>::value,  //
    BlackPawnAttacks<C4>::value,  //
    BlackPawnAttacks<B4>::value,  //
    BlackPawnAttacks<A4>::value,  //

    BlackPawnAttacks<H5>::value,  //
    BlackPawnAttacks<G5>::value,  //
    BlackPawnAttacks<F5>::value,  //
    BlackPawnAttacks<E5>::value,  //
    BlackPawnAttacks<D5>::value,  //
    BlackPawnAttacks<C5>::value,  //
    BlackPawnAttacks<B5>::value,  //
    BlackPawnAttacks<A5>::value,  //

    BlackPawnAttacks<H6>::value,  //
    BlackPawnAttacks<G6>::value,  //
    BlackPawnAttacks<F6>::value,  //
    BlackPawnAttacks<E6>::value,  //
    BlackPawnAttacks<D6>::value,  //
    BlackPawnAttacks<C6>::value,  //
    BlackPawnAttacks<B6>::value,  //
    BlackPawnAttacks<A6>::value,  //

    BlackPawnAttacks<H7>::value,  //
    BlackPawnAttacks<G7>::value,  //
    BlackPawnAttacks<F7>::value,  //
    BlackPawnAttacks<E7>::value,  //
    BlackPawnAttacks<D7>::value,  //
    BlackPawnAttacks<C7>::value,  //
    BlackPawnAttacks<B7>::value,  //
    BlackPawnAttacks<A7>::value,  //

    BlackPawnAttacks<H8>::value,  //
    BlackPawnAttacks<G8>::value,  //
    BlackPawnAttacks<F8>::value,  //
    BlackPawnAttacks<E8>::value,  //
    BlackPawnAttacks<D8>::value,  //
    BlackPawnAttacks<C8>::value,  //
    BlackPawnAttacks<B8>::value,  //
    BlackPawnAttacks<A8>::value,  //

    WhitePawnAttacks<H1>::value,  //
    WhitePawnAttacks<G1>::value,  //
    WhitePawnAttacks<F1>::value,  //
    WhitePawnAttacks<E1>::value,  //
    WhitePawnAttacks<D1>::value,  //
    WhitePawnAttacks<C1>::value,  //
    WhitePawnAttacks<B1>::value,  //
    WhitePawnAttacks<A1>::value,  //

    WhitePawnAttacks<H2>::value,  //
    WhitePawnAttacks<G2>::value,  //
    WhitePawnAttacks<F2>::value,  //
    WhitePawnAttacks<E2>::value,  //
    WhitePawnAttacks<D2>::value,  //
    WhitePawnAttacks<C2>::value,  //
    WhitePawnAttacks<B2>::value,  //
    WhitePawnAttacks<A2>::value,  //

    WhitePawnAttacks<H3>::value,  //
    WhitePawnAttacks<G3>::value,  //
    WhitePawnAttacks<F3>::value,  //
    WhitePawnAttacks<E3>::value,  //
    WhitePawnAttacks<D3>::value,  //
    WhitePawnAttacks<C3>::value,  //
    WhitePawnAttacks<B3>::value,  //
    WhitePawnAttacks<A3>::value,  //

    WhitePawnAttacks<H4>::value,  //
    WhitePawnAttacks<G4>::value,  //
    WhitePawnAttacks<F4>::value,  //
    WhitePawnAttacks<E4>::value,  //
    WhitePawnAttacks<D4>::value,  //
    WhitePawnAttacks<C4>::value,  //
    WhitePawnAttacks<B4>::value,  //
    WhitePawnAttacks<A4>::value,  //

    WhitePawnAttacks<H5>::value,  //
    WhitePawnAttacks<G5>::value,  //
    WhitePawnAttacks<F5>::value,  //
    WhitePawnAttacks<E5>::value,  //
    WhitePawnAttacks<D5>::value,  //
    WhitePawnAttacks<C5>::value,  //
    WhitePawnAttacks<B5>::value,  //
    WhitePawnAttacks<A5>::value,  //

    WhitePawnAttacks<H6>::value,  //
    WhitePawnAttacks<G6>::value,  //
    WhitePawnAttacks<F6>::value,  //
    WhitePawnAttacks<E6>::value,  //
    WhitePawnAttacks<D6>::value,  //
    WhitePawnAttacks<C6>::value,  //
    WhitePawnAttacks<B6>::value,  //
    WhitePawnAttacks<A6>::value,  //

    WhitePawnAttacks<H7>::value,  //
    WhitePawnAttacks<G7>::value,  //
    WhitePawnAttacks<F7>::value,  //
    WhitePawnAttacks<E7>::value,  //
    WhitePawnAttacks<D7>::value,  //
    WhitePawnAttacks<C7>::value,  //
    WhitePawnAttacks<B7>::value,  //
    WhitePawnAttacks<A7>::value,  //

    WhitePawnAttacks<H8>::value,  //
    WhitePawnAttacks<G8>::value,  //
    WhitePawnAttacks<F8>::value,  //
    WhitePawnAttacks<E8>::value,  //
    WhitePawnAttacks<D8>::value,  //
    WhitePawnAttacks<C8>::value,  //
    WhitePawnAttacks<B8>::value,  //
    WhitePawnAttacks<A8>::value,  //
};

}  // namespace Chess

#endif
