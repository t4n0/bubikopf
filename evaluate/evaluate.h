#ifndef EVALUATE_EVALUATE_H
#define EVALUATE_EVALUATE_H

#include "bitboard/position.h"
#include "board/evaluation.h"
#include "board/position.h"
#include "hardware/population_count.h"

#include <map>
#include <type_traits>

namespace Chess {

struct Production {
  static constexpr bool is_concrete{true};
};

template <typename Behaviour = Production>
std::enable_if_t<Behaviour::is_concrete, Evaluation> evaluate(
    const Position& position) {
  const std::map<SquareId, float> piece_values{
      {SquareId::WhitePawn, 1.0F},    {SquareId::WhiteKnight, 3.0F},
      {SquareId::WhiteBishop, 3.0F},  {SquareId::WhiteRook, 5.0F},
      {SquareId::WhiteQueen, 9.0F},   {SquareId::WhiteKing, 100.0F},
      {SquareId::BlackPawn, -1.0F},   {SquareId::BlackKnight, -3.0F},
      {SquareId::BlackBishop, -3.0F}, {SquareId::BlackRook, -5.0F},
      {SquareId::BlackQueen, -9.0F},  {SquareId::BlackKing, -100.0F},
      {SquareId::Empty, 0.0F},
  };
  float evaluation = 0.0F;
  for (const auto& square : position.board_.squares_) {
    evaluation += piece_values.at(square->GetId());
  }
  return evaluation;
}

struct EvaluteMaterial {
  static constexpr bool evaluate_material{true};
};

template <typename Behaviour = EvaluteMaterial>
std::enable_if_t<Behaviour::evaluate_material, Evaluation> evaluate(
    const PositionWithBitboards& position) {
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
