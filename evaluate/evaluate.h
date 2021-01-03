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
std::enable_if_t<Behaviour::evaluate_material, Evaluation> evaluate(const Position& position)
{
    float evaluation = 0.0F;
    // clang-format off
  evaluation += popcnt(position[kWhiteBoard + PAWN])     * 1.0F;
  evaluation += popcnt(position[kWhiteBoard + KNIGHT])   * 3.0F;
  evaluation += popcnt(position[kWhiteBoard + BISHOP])   * 3.0F;
  evaluation += popcnt(position[kWhiteBoard + ROOK])     * 5.0F;
  evaluation += popcnt(position[kWhiteBoard + QUEEN])    * 9.0F;
  evaluation += popcnt(position[kWhiteBoard + KING])   * 100.0F;
  evaluation += popcnt(position[kBlackBoard + PAWN])    * -1.0F;
  evaluation += popcnt(position[kBlackBoard + KNIGHT])  * -3.0F;
  evaluation += popcnt(position[kBlackBoard + BISHOP])  * -3.0F;
  evaluation += popcnt(position[kBlackBoard + ROOK])    * -5.0F;
  evaluation += popcnt(position[kBlackBoard + QUEEN])   * -9.0F;
  evaluation += popcnt(position[kBlackBoard + KING])  * -100.0F;
    // clang-format on

    return Evaluation{evaluation};
}

}  // namespace Chess

#endif
