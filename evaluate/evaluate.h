#ifndef EVALUATE_EVALUATE_H
#define EVALUATE_EVALUATE_H

#include "board/position.h"
#include "evaluate/evaluation.h"

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

}  // namespace Chess

#endif
