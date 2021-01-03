#include "bitboard/shift.h"

#include "bitboard/lookup_table/knight.h"
#include "bitboard/lookup_table/pawn.h"

#include <array>
#include <cmath>

namespace Chess
{

Bitboard SingleStep(const Bitboard value, const std::size_t direction)
{
    const int shift = kStepBits[direction];
    const Bitboard shifted_with_wrap = shift > 0 ? value << shift : value >> std::abs(shift);
    return shifted_with_wrap & kLegalAreasWithoutWrapping[direction];
}

Bitboard RuntimeKnightJump(const Bitboard value, const std::size_t direction)
{
    const int shift = kJumpBits[direction];
    const Bitboard shifted_with_wrap = shift > 0 ? value << shift : value >> std::abs(shift);
    return shifted_with_wrap & kLegalLandingAreasWithoutWrapping[direction];
}

}  // namespace Chess
