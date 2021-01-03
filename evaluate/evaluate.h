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
  evaluation += popcnt(position[kWhiteBoard + kPawn])     * 1.0F;
  evaluation += popcnt(position[kWhiteBoard + kKnight])   * 3.0F;
  evaluation += popcnt(position[kWhiteBoard + kBishop])   * 3.0F;
  evaluation += popcnt(position[kWhiteBoard + kRook])     * 5.0F;
  evaluation += popcnt(position[kWhiteBoard + kQueen])    * 9.0F;
  evaluation += popcnt(position[kWhiteBoard + kKing])   * 100.0F;
  evaluation += popcnt(position[kBlackBoard + kPawn])    * -1.0F;
  evaluation += popcnt(position[kBlackBoard + kKnight])  * -3.0F;
  evaluation += popcnt(position[kBlackBoard + kBishop])  * -3.0F;
  evaluation += popcnt(position[kBlackBoard + kRook])    * -5.0F;
  evaluation += popcnt(position[kBlackBoard + kQueen])   * -9.0F;
  evaluation += popcnt(position[kBlackBoard + kKing])  * -100.0F;
    // clang-format on

    return Evaluation{evaluation};
}

}  // namespace Chess

#endif
