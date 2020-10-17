#include "bitboard/shift.h"

#include "bitboard/squares.h"

#include <array>
#include <cmath>

namespace Chess
{
namespace
{

// bitshifts resulting in directions (e.g. "west", "north-west", etc.) in clockwise order
constexpr std::array<int, 8> direction_bits{1, 9, 8, 7, -1, -9, -8, -7};

// "wrapping" refers to e.g. a jump from A1 to H2 when illegally going "west" of square A1
constexpr std::array<Bitboard, 8> legal_areas_without_wrapping{
    ~(H1 | H2 | H3 | H4 | H5 | H6 | H7 | H8),
    ~(H1 | H2 | H3 | H4 | H5 | H6 | H7 | H8 | A1 | B1 | C1 | D1 | E1 | F1 | G1),
    ~(A1 | B1 | C1 | D1 | E1 | F1 | G1 | H1),
    ~(A1 | B1 | C1 | D1 | E1 | F1 | G1 | H1 | A2 | A3 | A4 | A5 | A6 | A7 | A8),
    ~(A1 | A2 | A3 | A4 | A5 | A6 | A7 | A8),
    ~(A1 | A2 | A3 | A4 | A5 | A6 | A7 | A8 | B8 | C8 | D8 | E8 | F8 | G8 | H8),
    ~(A8 | B8 | C8 | D8 | E8 | F8 | G8 | H8),
    ~(A8 | B8 | C8 | D8 | E8 | F8 | G8 | H8 | H1 | H2 | H3 | H4 | H5 | H6 | H7),
};

}  // namespace

Bitboard Shift(const Bitboard value, const std::size_t direction)
{
    const int shift = direction_bits[direction];
    const Bitboard shifted_with_wrap = shift > 0 ? value << shift : value >> std::abs(shift);
    return shifted_with_wrap & legal_areas_without_wrapping[direction];
}

}  // namespace Chess
