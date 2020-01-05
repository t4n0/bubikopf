#include "chess/pieces.h"

#include <gtest/gtest.h>
#include <vector>

namespace Chess {
namespace {

TEST(Clone, ExpectEquality) {
  // Setup
  const std::unique_ptr<ISquare> original =
      std::make_unique<King>(AlphaBeta::Player::max);

  // Call
  const std::unique_ptr<ISquare> clone = original->clone();

  // Expect
  EXPECT_FLOAT_EQ(original->GetValue(), clone->GetValue());
  EXPECT_TRUE(clone->IsOfSide(AlphaBeta::Player::max));
}

class BlackPawnFindMoves_Fixture : public testing::Test {
 public:
  void SetUp() override { state_ = State{}; }
  void TearDown() override {}

  State state_;
};

TEST_F(BlackPawnFindMoves_Fixture, GivenNoFreeSquareInfront_ExpectNoMoves) {
  // Setup
  const Coordinate black_pawn_location{3, 1};
  const Coordinate blocking_piece_location{3, 2};
  state_.board_.Set(black_pawn_location,
                    std::make_unique<Pawn>(AlphaBeta::Player::min));
  state_.board_.Set(blocking_piece_location,
                    std::make_unique<King>(AlphaBeta::Player::max));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(black_pawn_location)
          .FindMoves(ToIdx(black_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 0);
}

TEST_F(BlackPawnFindMoves_Fixture, GivenWhitePawn_ExpectNoMoves) {
  // Setup
  const Coordinate white_pawn_location{3, 3};
  state_.board_.Set(white_pawn_location,
                    std::make_unique<Pawn>(AlphaBeta::Player::max));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(white_pawn_location)
          .FindMoves(ToIdx(white_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 0);
}

TEST_F(BlackPawnFindMoves_Fixture,
       GivenLocationNotOnStartRankAndFreeSquareInfront_ExpectSingleStepOnly) {
  // Setup
  const Coordinate location_behind_black_pawn{3, 3};
  const Coordinate black_pawn_location{3, 4};
  const Coordinate free_square{3, 5};
  state_.board_.Set(black_pawn_location,
                    std::make_unique<Pawn>(AlphaBeta::Player::min));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(black_pawn_location)
          .FindMoves(ToIdx(black_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 1);
  const State& returned_state = returned_states.front();
  EXPECT_TRUE(returned_state.board_.Get(location_behind_black_pawn).IsEmpty());
  EXPECT_TRUE(returned_state.board_.Get(black_pawn_location).IsEmpty());
  EXPECT_FALSE(returned_state.board_.Get(free_square).IsEmpty());
  EXPECT_FLOAT_EQ(returned_state.board_.Get(free_square).GetValue(),
                  Pawn{AlphaBeta::Player::min}.GetValue());
  EXPECT_FALSE(returned_state.en_passant_);
}

TEST_F(BlackPawnFindMoves_Fixture,
       GivenFreeSquareOnPromotionRank_ExpectAllPromotions) {
  // Setup
  const Coordinate location_behind_black_pawn{3, 5};
  const Coordinate black_pawn_location{3, 6};
  const Coordinate promotion_square{3, 7};
  state_.board_.Set(black_pawn_location,
                    std::make_unique<Pawn>(AlphaBeta::Player::min));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(black_pawn_location)
          .FindMoves(ToIdx(black_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 4);
  for (const auto& returned_state : returned_states) {
    EXPECT_TRUE(
        returned_state.board_.Get(location_behind_black_pawn).IsEmpty());
    EXPECT_TRUE(returned_state.board_.Get(black_pawn_location).IsEmpty());
    EXPECT_FALSE(returned_state.board_.Get(promotion_square).IsEmpty());
  }
  EXPECT_FLOAT_EQ(returned_states.at(0).board_.Get(promotion_square).GetValue(),
                  Queen{AlphaBeta::Player::min}.GetValue());
  EXPECT_FLOAT_EQ(returned_states.at(1).board_.Get(promotion_square).GetValue(),
                  Rook{AlphaBeta::Player::min}.GetValue());
  EXPECT_FLOAT_EQ(returned_states.at(2).board_.Get(promotion_square).GetValue(),
                  Knight{AlphaBeta::Player::min}.GetValue());
  EXPECT_FLOAT_EQ(returned_states.at(3).board_.Get(promotion_square).GetValue(),
                  Bishop{AlphaBeta::Player::min}.GetValue());
}

TEST_F(BlackPawnFindMoves_Fixture,
       GivenLocationOnStartRankAndTwoFreeSquaresInfront_ExpectAlsoDoubleStep) {
  // Setup
  const Coordinate location_behind_black_pawn{4, 0};
  const Coordinate black_pawn_location{4, 1};
  const Coordinate single_step_target{4, 2};
  const Coordinate double_step_target{4, 3};
  state_.board_.Set(black_pawn_location,
                    std::make_unique<Pawn>(AlphaBeta::Player::min));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(black_pawn_location)
          .FindMoves(ToIdx(black_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 2);
  for (const auto& returned_state : returned_states) {
    EXPECT_TRUE(
        returned_state.board_.Get(location_behind_black_pawn).IsEmpty());
    EXPECT_TRUE(returned_state.board_.Get(black_pawn_location).IsEmpty());
  }
  EXPECT_TRUE(returned_states.at(0).board_.Get(double_step_target).IsEmpty());
  EXPECT_FALSE(returned_states.at(0).board_.Get(single_step_target).IsEmpty());
  EXPECT_FLOAT_EQ(
      returned_states.at(0).board_.Get(single_step_target).GetValue(),
      Pawn{AlphaBeta::Player::min}.GetValue());
  EXPECT_TRUE(returned_states.at(1).board_.Get(single_step_target).IsEmpty());
  EXPECT_FALSE(returned_states.at(1).board_.Get(double_step_target).IsEmpty());
  EXPECT_FLOAT_EQ(
      returned_states.at(1).board_.Get(double_step_target).GetValue(),
      Pawn{AlphaBeta::Player::min}.GetValue());
}

TEST_F(BlackPawnFindMoves_Fixture, GivenOneCapturePossible_ExpectOneMove) {
  // Setup
  const Coordinate black_pawn_location{3, 3};
  const Coordinate white_blocking_piece_location{3, 4};
  const Coordinate white_hanging_piece_location{4, 4};
  const Coordinate black_piece_on_caputre_location{2, 4};
  state_.board_.Set(black_pawn_location,
                    std::make_unique<Pawn>(AlphaBeta::Player::min));
  state_.board_.Set(black_piece_on_caputre_location,
                    std::make_unique<Knight>(AlphaBeta::Player::min));
  state_.board_.Set(white_blocking_piece_location,
                    std::make_unique<King>(AlphaBeta::Player::max));
  state_.board_.Set(white_hanging_piece_location,
                    std::make_unique<Rook>(AlphaBeta::Player::max));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(black_pawn_location)
          .FindMoves(ToIdx(black_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 1);
  const State& returned_state = returned_states.front();
  EXPECT_TRUE(returned_state.board_.Get(black_pawn_location).IsEmpty());
  EXPECT_FLOAT_EQ(
      returned_state.board_.Get(white_blocking_piece_location).GetValue(),
      100.0F);
  EXPECT_FLOAT_EQ(
      returned_state.board_.Get(white_hanging_piece_location).GetValue(),
      -1.0F);
  EXPECT_FLOAT_EQ(
      returned_state.board_.Get(black_piece_on_caputre_location).GetValue(),
      -3.0F);
}

TEST_F(BlackPawnFindMoves_Fixture, GivenEnPassant_ExpectCapture) {
  // Setup
  const Coordinate black_pawn{5, 4};
  const Coordinate white_pawn{4, 4};
  const Coordinate white_blocking_piece{5, 5};
  const Coordinate en_passant{4, 5};
  state_.board_.Set(black_pawn, std::make_unique<Pawn>(AlphaBeta::Player::min));
  state_.board_.Set(white_pawn, std::make_unique<Pawn>(AlphaBeta::Player::max));
  state_.board_.Set(white_blocking_piece,
                    std::make_unique<Knight>(AlphaBeta::Player::max));
  state_.en_passant_ = en_passant;

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(black_pawn).FindMoves(ToIdx(black_pawn), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 1);
  const State& returned_state = returned_states.front();
  EXPECT_TRUE(returned_state.board_.Get(white_pawn).IsEmpty());
  EXPECT_TRUE(returned_state.board_.Get(black_pawn).IsEmpty());
  EXPECT_FLOAT_EQ(returned_state.board_.Get(en_passant).GetValue(), -1.0F);
  EXPECT_FLOAT_EQ(returned_state.board_.Get(white_blocking_piece).GetValue(),
                  3.0F);
}

class WhitePawnFindMoves_Fixture : public testing::Test {
 public:
  void SetUp() override {
    state_ = State{};
    state_.turn_ = AlphaBeta::Player::max;
  }
  void TearDown() override {}

  State state_;
};

TEST_F(WhitePawnFindMoves_Fixture, GivenNoFreeSquareInfront_ExpectNoMoves) {
  // Setup
  const Coordinate white_pawn_location{3, 6};
  const Coordinate blocking_piece_location{3, 5};
  state_.board_.Set(white_pawn_location,
                    std::make_unique<Pawn>(AlphaBeta::Player::max));
  state_.board_.Set(blocking_piece_location,
                    std::make_unique<King>(AlphaBeta::Player::min));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(white_pawn_location)
          .FindMoves(ToIdx(white_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 0);
}

TEST_F(WhitePawnFindMoves_Fixture, GivenBlackPawn_ExpectNoMoves) {
  // Setup
  const Coordinate black_pawn_location{3, 3};
  state_.board_.Set(black_pawn_location,
                    std::make_unique<Pawn>(AlphaBeta::Player::min));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(black_pawn_location)
          .FindMoves(ToIdx(black_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 0);
}

TEST_F(WhitePawnFindMoves_Fixture,
       GivenLocationNotOnStartRankAndFreeSquareInfront_ExpectSingleStepOnly) {
  // Setup
  const Coordinate location_behind_white_pawn{3, 5};
  const Coordinate white_pawn_location{3, 4};
  const Coordinate free_square{3, 3};
  state_.board_.Set(white_pawn_location,
                    std::make_unique<Pawn>(AlphaBeta::Player::max));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(white_pawn_location)
          .FindMoves(ToIdx(white_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 1);
  const State& returned_state = returned_states.front();
  EXPECT_TRUE(returned_state.board_.Get(location_behind_white_pawn).IsEmpty());
  EXPECT_TRUE(returned_state.board_.Get(white_pawn_location).IsEmpty());
  EXPECT_FALSE(returned_state.board_.Get(free_square).IsEmpty());
  EXPECT_FLOAT_EQ(returned_state.board_.Get(free_square).GetValue(),
                  Pawn{AlphaBeta::Player::max}.GetValue());
  EXPECT_FALSE(returned_state.en_passant_);
}

TEST_F(WhitePawnFindMoves_Fixture,
       GivenFreeSquareOnPromotionRank_ExpectAllPromotions) {
  // Setup
  const Coordinate location_behind_white_pawn{3, 2};
  const Coordinate white_pawn_location{3, 1};
  const Coordinate promotion_square{3, 0};
  state_.board_.Set(white_pawn_location,
                    std::make_unique<Pawn>(AlphaBeta::Player::max));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(white_pawn_location)
          .FindMoves(ToIdx(white_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 4);
  for (const auto& returned_state : returned_states) {
    EXPECT_TRUE(
        returned_state.board_.Get(location_behind_white_pawn).IsEmpty());
    EXPECT_TRUE(returned_state.board_.Get(white_pawn_location).IsEmpty());
    EXPECT_FALSE(returned_state.board_.Get(promotion_square).IsEmpty());
  }
  EXPECT_FLOAT_EQ(returned_states.at(0).board_.Get(promotion_square).GetValue(),
                  Queen{AlphaBeta::Player::max}.GetValue());
  EXPECT_FLOAT_EQ(returned_states.at(1).board_.Get(promotion_square).GetValue(),
                  Rook{AlphaBeta::Player::max}.GetValue());
  EXPECT_FLOAT_EQ(returned_states.at(2).board_.Get(promotion_square).GetValue(),
                  Knight{AlphaBeta::Player::max}.GetValue());
  EXPECT_FLOAT_EQ(returned_states.at(3).board_.Get(promotion_square).GetValue(),
                  Bishop{AlphaBeta::Player::max}.GetValue());
}

TEST_F(WhitePawnFindMoves_Fixture,
       GivenLocationOnStartRankAndTwoFreeSquaresInfront_ExpectAlsoDoubleStep) {
  // Setup
  const Coordinate location_behind_white_pawn{4, 7};
  const Coordinate white_pawn_location{4, 6};
  const Coordinate single_step_target{4, 5};
  const Coordinate double_step_target{4, 4};
  state_.board_.Set(white_pawn_location,
                    std::make_unique<Pawn>(AlphaBeta::Player::max));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(white_pawn_location)
          .FindMoves(ToIdx(white_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 2);
  for (const auto& returned_state : returned_states) {
    EXPECT_TRUE(
        returned_state.board_.Get(location_behind_white_pawn).IsEmpty());
    EXPECT_TRUE(returned_state.board_.Get(white_pawn_location).IsEmpty());
  }
  EXPECT_TRUE(returned_states.at(0).board_.Get(double_step_target).IsEmpty());
  EXPECT_FALSE(returned_states.at(0).board_.Get(single_step_target).IsEmpty());
  EXPECT_FLOAT_EQ(
      returned_states.at(0).board_.Get(single_step_target).GetValue(),
      Pawn{AlphaBeta::Player::max}.GetValue());
  EXPECT_TRUE(returned_states.at(1).board_.Get(single_step_target).IsEmpty());
  EXPECT_FALSE(returned_states.at(1).board_.Get(double_step_target).IsEmpty());
  EXPECT_FLOAT_EQ(
      returned_states.at(1).board_.Get(double_step_target).GetValue(),
      Pawn{AlphaBeta::Player::max}.GetValue());
}

TEST_F(WhitePawnFindMoves_Fixture,
       GivenDoubleStep_ExpectEnPassantOnlyForDoubleStep) {
  // Setup
  const Coordinate white_pawn_location{4, 6};
  const Coordinate single_step_target{4, 5};
  state_.board_.Set(white_pawn_location,
                    std::make_unique<Pawn>(AlphaBeta::Player::max));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(white_pawn_location)
          .FindMoves(ToIdx(white_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.at(1).en_passant_ == single_step_target);
}

TEST_F(WhitePawnFindMoves_Fixture, GivenOneCapturePossible_ExpectOneMove) {
  // Setup
  const Coordinate white_pawn_location{5, 5};
  const Coordinate black_blocking_piece_location{5, 4};
  const Coordinate black_hanging_piece_location{6, 4};
  const Coordinate white_piece_on_caputre_location{4, 4};
  state_.board_.Set(white_pawn_location,
                    std::make_unique<Pawn>(AlphaBeta::Player::max));
  state_.board_.Set(white_piece_on_caputre_location,
                    std::make_unique<Knight>(AlphaBeta::Player::max));
  state_.board_.Set(black_blocking_piece_location,
                    std::make_unique<King>(AlphaBeta::Player::min));
  state_.board_.Set(black_hanging_piece_location,
                    std::make_unique<Rook>(AlphaBeta::Player::min));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(white_pawn_location)
          .FindMoves(ToIdx(white_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 1);
  const State& returned_state = returned_states.front();
  EXPECT_TRUE(returned_state.board_.Get(white_pawn_location).IsEmpty());
  EXPECT_FLOAT_EQ(
      returned_state.board_.Get(black_blocking_piece_location).GetValue(),
      -100.0F);
  EXPECT_FLOAT_EQ(
      returned_state.board_.Get(black_hanging_piece_location).GetValue(), 1.0F);
  EXPECT_FLOAT_EQ(
      returned_state.board_.Get(white_piece_on_caputre_location).GetValue(),
      3.0F);
}

TEST_F(WhitePawnFindMoves_Fixture, GivenEnPassant_ExpectCapture) {
  // Setup
  const Coordinate white_pawn{4, 3};
  const Coordinate black_pawn{5, 3};
  const Coordinate black_blocking_piece{4, 2};
  const Coordinate en_passant{5, 2};
  state_.board_.Set(white_pawn, std::make_unique<Pawn>(AlphaBeta::Player::max));
  state_.board_.Set(black_pawn, std::make_unique<Pawn>(AlphaBeta::Player::min));
  state_.board_.Set(black_blocking_piece,
                    std::make_unique<Knight>(AlphaBeta::Player::min));
  state_.en_passant_ = en_passant;

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(white_pawn).FindMoves(ToIdx(white_pawn), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 1);
  const State& returned_state = returned_states.front();
  EXPECT_TRUE(returned_state.board_.Get(white_pawn).IsEmpty());
  EXPECT_TRUE(returned_state.board_.Get(black_pawn).IsEmpty());
  EXPECT_FLOAT_EQ(returned_state.board_.Get(en_passant).GetValue(), 1.0F);
  EXPECT_FLOAT_EQ(returned_state.board_.Get(black_blocking_piece).GetValue(),
                  -3.0F);
}

}  // namespace
}  // namespace Chess
