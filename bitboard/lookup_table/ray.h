#ifndef BITBOARD_LOOKUP_TABLE_RAY_H
#define BITBOARD_LOOKUP_TABLE_RAY_H

#include "bitboard/basic_type_declarations.h"
#include "bitboard/lookup_table/utilities.h"
#include "bitboard/squares.h"

#include <array>

namespace Chess
{

/// @brief Indizes to access different aspects of a single piece step (except knights).
constexpr std::size_t kWest = 0;
constexpr std::size_t kNorthWest = 1;
constexpr std::size_t kNorth = 2;
constexpr std::size_t kNorthEast = 3;
constexpr std::size_t kEast = 4;
constexpr std::size_t kSouthEast = 5;
constexpr std::size_t kSouth = 6;
constexpr std::size_t kSouthWest = 7;

// Values correspond to necessary bitshifts for single steps. E.g. kStepBits[kWest].
constexpr std::array<int, 8> kStepBits{1, 9, 8, 7, -1, -9, -8, -7};

// Values correspond to legal landing areas without wrapping e.g. a step from A1 to H2 when illegally going "kWest" of
// square A1. E.g. kLegalAreasWithoutWrapping[kNorth].
constexpr std::array<Bitboard, 8> kLegalAreasWithoutWrapping{~(kFileH),
                                                             ~(kFileH | kRank1),
                                                             ~(kRank1),
                                                             ~(kRank1 | kFileA),
                                                             ~(kFileA),
                                                             ~(kFileA | kRank8),
                                                             ~(kRank8),
                                                             ~(kRank8 | kFileH)};

template <Bitboard source, std::size_t direction>
class RayOneSquare
{
    constexpr static int bitshift = std::get<direction>(kStepBits);
    constexpr static Bitboard legal_step_area = std::get<direction>(kLegalAreasWithoutWrapping);

  public:
    constexpr static Bitboard value =
        legal_step_area & (bitshift > 0 ? source << bitshift : source >> Absolute<bitshift>::value);
};

template <Bitboard source, std::size_t direction, int loop = 0>
class RaySevenSquares
{
    constexpr static Bitboard ray_one_square = RayOneSquare<source, direction>::value;

  public:
    constexpr static Bitboard value = ray_one_square | RaySevenSquares<ray_one_square, direction, loop + 1>::value;
};

template <Bitboard source, std::size_t direction>
struct RaySevenSquares<source, direction, 8>
{
    constexpr static Bitboard value = 0;
};

}  // namespace Chess

#endif
