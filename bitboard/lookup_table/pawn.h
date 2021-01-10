#ifndef BITBOARD_LOOKUP_TABLE_PAWN_H
#define BITBOARD_LOOKUP_TABLE_PAWN_H

#include "bitboard/lookup_table/ray.h"

#include <array>

namespace Chess
{

/// @brief Given source the value of this type represents the target a white pawn could capture towards the queenside.
template <Bitboard source>
using WhitePawnCaptureQueenside = RayOneSquare<source, kNorthWest>;

/// @brief Given source the value of this type represents the target a white pawn could capture towards the kingside.
template <Bitboard source>
using WhitePawnCaptureKingside = RayOneSquare<source, kNorthEast>;

/// @brief Given source the value of this type represents the target a black pawn could capture towards the queenside.
template <Bitboard source>
using BlackPawnCaptureQueenside = RayOneSquare<source, kSouthWest>;

/// @brief Given source the value of this type represents the target a black pawn could capture towards the kingside.
template <Bitboard source>
using BlackPawnCaptureKingside = RayOneSquare<source, kSouthEast>;

template <template <Bitboard> typename QueensideCapture,
          template <Bitboard>
          typename KingsideCapture,
          std::size_t index = 0>
constexpr std::array<std::array<Bitboard, 2>, 64> CalculatePawnCaptureLookUpTable()
{
    std::array<std::array<Bitboard, 2>, 64> lookup_table{};
    if constexpr (index < 63)  // terminate recursion at index 63
    {
        lookup_table = CalculatePawnCaptureLookUpTable<QueensideCapture, KingsideCapture, index + 1>();
    }
    std::get<index>(lookup_table) = {QueensideCapture<std::get<index>(kAllSquares)>::value,
                                     KingsideCapture<std::get<index>(kAllSquares)>::value};
    return lookup_table;
}

constexpr auto kPawnCaptureLookupTable =
    ConcatenateArrays(CalculatePawnCaptureLookUpTable<WhitePawnCaptureQueenside, WhitePawnCaptureKingside>(),
                      CalculatePawnCaptureLookUpTable<BlackPawnCaptureQueenside, BlackPawnCaptureKingside>());
constexpr std::size_t kPawnCapturesLookupTableOffsetForBlack = 64;

/// @brief Given an target bitboard the value of this struct represents a bitboard with the squares set that a black
/// pawn could attack from.
template <Bitboard target>
struct BlackPawnAttacks
{
    constexpr static Bitboard value = RayOneSquare<target, kNorthWest>::value | RayOneSquare<target, kNorthEast>::value;
};

/// @brief Given an target bitboard the value of this struct represents a bitboard with the squares set that a white
/// pawn could attack from.
template <Bitboard target>
struct WhitePawnAttacks
{
    constexpr static Bitboard value = RayOneSquare<target, kSouthWest>::value | RayOneSquare<target, kSouthEast>::value;
};

template <template <Bitboard> typename PawnAttacks, std::size_t index = 0>
constexpr std::array<Bitboard, 64> CalculatePawnAttackLookUpTable()
{
    std::array<Bitboard, 64> lookup_table{};
    if constexpr (index < 63)  // terminate recursion at index 63
    {
        lookup_table = CalculatePawnAttackLookUpTable<PawnAttacks, index + 1>();
    }
    std::get<index>(lookup_table) = PawnAttacks<std::get<index>(kAllSquares)>::value;
    return lookup_table;
}

constexpr std::array<Bitboard, 128> kPawnAttacks =
    ConcatenateArrays(CalculatePawnAttackLookUpTable<BlackPawnAttacks>(),
                      CalculatePawnAttackLookUpTable<WhitePawnAttacks>());
constexpr std::size_t kPawnAttacksLookupTableOffsetForWhite = 64;

}  // namespace Chess

#endif
