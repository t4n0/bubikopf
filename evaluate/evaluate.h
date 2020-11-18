#ifndef EVALUATE_EVALUATE_H
#define EVALUATE_EVALUATE_H

#include "bitboard/position.h"
#include "hardware/population_count.h"

namespace Chess
{

struct EvaluteMaterial
{
    static constexpr bool evaluate_material{true};
};

template <typename Behaviour = EvaluteMaterial>
std::enable_if_t<Behaviour::evaluate_material, Evaluation> evaluate(const PositionWithBitboards& position)
{
    float evaluation = 0.0F;
    // clang-format off
  evaluation += popcnt(position[BOARD_IDX_WHITE + PAWN])     * 1.0F;
  evaluation += popcnt(position[BOARD_IDX_WHITE + KNIGHT])   * 3.0F;
  evaluation += popcnt(position[BOARD_IDX_WHITE + BISHOP])   * 3.0F;
  evaluation += popcnt(position[BOARD_IDX_WHITE + ROOK])     * 5.0F;
  evaluation += popcnt(position[BOARD_IDX_WHITE + QUEEN])    * 9.0F;
  evaluation += popcnt(position[BOARD_IDX_WHITE + KING])   * 100.0F;
  evaluation += popcnt(position[BOARD_IDX_BLACK + PAWN])    * -1.0F;
  evaluation += popcnt(position[BOARD_IDX_BLACK + KNIGHT])  * -3.0F;
  evaluation += popcnt(position[BOARD_IDX_BLACK + BISHOP])  * -3.0F;
  evaluation += popcnt(position[BOARD_IDX_BLACK + ROOK])    * -5.0F;
  evaluation += popcnt(position[BOARD_IDX_BLACK + QUEEN])   * -9.0F;
  evaluation += popcnt(position[BOARD_IDX_BLACK + KING])  * -100.0F;
    // clang-format on

    return Evaluation{evaluation};
}

}  // namespace Chess

#endif
