#include <chess/evaluate.h>

#include <gtest/gtest.h>
#include <utility>
#include <vector>

namespace Chess {
namespace {

class EvaluateTest : public detail::EmptyBoardFixture,
                     public ::testing::WithParamInterface<
                         std::tuple<PlacedPieces, GameTree::Evaluation>> {};

TEST_P(EvaluateTest, AllMustHold) {
  const PlacedPieces& placed_pieces{std::get<PlacedPieces>(GetParam())};
  std::for_each(begin(placed_pieces), end(placed_pieces),
                [this](const PlacedPiece& placed_piece) {
                  state_.SetSquareTo(placed_piece.coordinate_.file_,
                                     placed_piece.coordinate_.rank_,
                                     placed_piece.piece_);
                });

  const GameTree::Evaluation returned_evaluation{GameTree::evaluate(state_)};

  const GameTree::Evaluation& expected_evaluation{
      std::get<GameTree::Evaluation>(GetParam())};
  EXPECT_TRUE(std::holds_alternative<float>(returned_evaluation));
  EXPECT_FLOAT_EQ(std::get<float>(returned_evaluation),
                  std::get<float>(expected_evaluation));
}

const std::vector<std::tuple<PlacedPieces, GameTree::Evaluation>> SAMPLE_POINTS{
    {{{{'a', 1}, Piece::p}, {{'a', 2}, Piece::P}, {{'a', 3}, Piece::P}}, 1.0F},
    {{{{'a', 1}, Piece::p}, {{'a', 2}, Piece::p}, {{'a', 3}, Piece::P}}, -1.0F},
    {{{{'a', 1}, Piece::p}, {{'a', 2}, Piece::P}}, 0.0F},
    {{{{'a', 1}, Piece::k}, {{'a', 2}, Piece::K}}, 0.0F},
    {{{{'a', 1}, Piece::q}, {{'a', 2}, Piece::B}}, -6.0F},
    {{{{'a', 1}, Piece::n}, {{'a', 2}, Piece::R}}, 2.0F},
};

INSTANTIATE_TEST_CASE_P(SamplePoints, EvaluateTest,
                        ::testing::ValuesIn(SAMPLE_POINTS));

}  // namespace
}  // namespace Chess
