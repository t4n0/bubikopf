#ifndef BITBOARD_LOOKUP_TABLE_RAY_H
#define BITBOARD_LOOKUP_TABLE_RAY_H

#include "bitboard/basic_type_declarations.h"
#include "bitboard/lookup_table/utilities.h"
#include "bitboard/squares.h"

#include <array>

namespace Chess
{

/// @brief Indizes to access different aspects of a single piece step (except knights).
constexpr std::size_t west = 0;
constexpr std::size_t north_west = 1;
constexpr std::size_t north = 2;
constexpr std::size_t north_east = 3;
constexpr std::size_t east = 4;
constexpr std::size_t south_east = 5;
constexpr std::size_t south = 6;
constexpr std::size_t south_west = 7;

// Values correspond to necessary bitshifts for single steps. E.g. step_bits.at(west).
constexpr std::array<int, 8> step_bits{1, 9, 8, 7, -1, -9, -8, -7};

// Values correspond to legal landing areas without wrapping e.g. a step from A1 to H2 when illegally going "west" of
// square A1. E.g. legal_step_areas_without_wrapping.at(north).
constexpr std::array<Bitboard, 8> legal_step_areas_without_wrapping{~(FILE_H),
                                                                    ~(FILE_H | RANK_1),
                                                                    ~(RANK_1),
                                                                    ~(RANK_1 | FILE_A),
                                                                    ~(FILE_A),
                                                                    ~(FILE_A | RANK_8),
                                                                    ~(RANK_8),
                                                                    ~(RANK_8 | FILE_H)};

template <Bitboard source, std::size_t direction>
class RayOneSquare
{
    constexpr static int bitshift = std::get<direction>(step_bits);
    constexpr static Bitboard legal_step_area = std::get<direction>(legal_step_areas_without_wrapping);

  public:
    constexpr static Bitboard value =
        legal_step_area & (bitshift > 0 ? source << bitshift : source >> Absolute<bitshift>::value);
};

}  // namespace Chess

#endif
