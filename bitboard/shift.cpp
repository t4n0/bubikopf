#include "bitboard/shift.h"

#include "bitboard/lookup_table/knight.h"

#include <array>
#include <cmath>

namespace Chess
{
namespace
{

// Values correspond to necessary bitshifts for single steps. Usage as step_bits.at(west).
constexpr std::array<int, 8> step_bits{1, 9, 8, 7, -1, -9, -8, -7};
// "wrapping" refers to e.g. a step from A1 to H2 when illegally going "west" of square A1
constexpr std::array<Bitboard, 8> legal_step_areas_without_wrapping{~(FILE_H),
                                                                    ~(FILE_H | RANK_1),
                                                                    ~(RANK_1),
                                                                    ~(RANK_1 | FILE_A),
                                                                    ~(FILE_A),
                                                                    ~(FILE_A | RANK_8),
                                                                    ~(RANK_8),
                                                                    ~(RANK_8 | FILE_H)};

}  // namespace

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
