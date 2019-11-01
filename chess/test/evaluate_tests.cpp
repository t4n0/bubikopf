#include "chess/evaluate.h"

#include <gtest/gtest.h>

#include <utility>
#include <vector>

namespace Chess {
namespace {

class EvaluateTest : public testing::TestWithParam<
                         std::tuple<PlacedPieces, GameTree::Evaluation>> {};

TEST_P(EvaluateTest, AllSamplesMustHold) {
  State state{};
  const PlacedPieces& placed_pieces{std::get<PlacedPieces>(GetParam())};
  std::for_each(begin(placed_pieces), end(placed_pieces),
                [&state](const PlacedPiece& placed_piece) {
                  state.board_.Set(placed_piece.coordinate, placed_piece.piece);
                });

  const GameTree::Evaluation returned_evaluation{GameTree::evaluate(state)};

  const GameTree::Evaluation& expected_evaluation{
      std::get<GameTree::Evaluation>(GetParam())};
  EXPECT_TRUE(std::holds_alternative<float>(returned_evaluation));
  EXPECT_FLOAT_EQ(std::get<float>(returned_evaluation),
                  std::get<float>(expected_evaluation));
}

const std::vector<std::tuple<PlacedPieces, GameTree::Evaluation>> SAMPLES{
    {{{{0, 0}, Piece::p}, {{0, 1}, Piece::P}, {{0, 2}, Piece::P}}, 1.0F},
    {{{{0, 0}, Piece::p}, {{0, 1}, Piece::p}, {{0, 2}, Piece::P}}, -1.0F},
    {{{{0, 0}, Piece::p}, {{0, 1}, Piece::P}}, 0.0F},
    {{{{0, 0}, Piece::k}, {{0, 1}, Piece::K}}, 0.0F},
    {{{{0, 0}, Piece::q}, {{0, 1}, Piece::B}}, -6.0F},
    {{{{0, 0}, Piece::n}, {{0, 1}, Piece::R}}, 2.0F},
};

INSTANTIATE_TEST_CASE_P(SamplePoints, EvaluateTest, testing::ValuesIn(SAMPLES));

}  // namespace
}  // namespace Chess
