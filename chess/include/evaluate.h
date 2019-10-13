#ifndef CHESS_EVALUATE_H
#define CHESS_EVALUATE_H

#include <chess/state.h>
#include <alpha_beta/minimax.h>

namespace Chess {
const std::map<Piece, float> PIECE_VALUES{
    {Piece::p, -1.0F}, {Piece::r, -5.0F}, {Piece::n, -3.0F},
    {Piece::b, -3.0F}, {Piece::q, -9.0F}, {Piece::k, -100.0F},
    {Piece::P, 1.0F},  {Piece::R, 5.0F},  {Piece::N, 3.0F},
    {Piece::B, 3.0F},  {Piece::Q, 9.0F},  {Piece::K, 100.0F},
};
}

namespace GameTree {

template <>
Evaluation evaluate(const Chess::State& state) {
  float evaluation = 0.0F;

  for (const Chess::CodedRank& codes : state.board_) {
    for (const Chess::Code& code : codes) {
      if (Chess::IsAPiece(code)) {
        evaluation += Chess::PIECE_VALUES.at(std::get<Chess::Piece>(code));
      }
    }
  }
  return evaluation;
}

}  // namespace GameTree

// todo: Test for checkmate

#endif
