#include "bitboard/shift.h"

#include "bitboard/lookup_table/knight.h"
#include "bitboard/lookup_table/pawn.h"

#include <array>
#include <cmath>

namespace Chess
{

Bitboard SingleStep(const Bitboard value, const std::size_t direction)
{
    const int shift = step_bits[direction];
    const Bitboard shifted_with_wrap = shift > 0 ? value << shift : value >> std::abs(shift);
    return shifted_with_wrap & legal_step_areas_without_wrapping[direction];
}

Bitboard RuntimeKnightJump(const Bitboard value, const std::size_t direction)
{
    const int shift = jump_bits[direction];
    const Bitboard shifted_with_wrap = shift > 0 ? value << shift : value >> std::abs(shift);
    return shifted_with_wrap & legal_landing_areas_without_wrapping[direction];
}

}  // namespace Chess
