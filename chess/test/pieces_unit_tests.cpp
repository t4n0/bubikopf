#include "chess/pieces.h"

#include <gtest/gtest.h>
#include <vector>

namespace Chess {
namespace {

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
          ->FindMoves(ToIdx(black_pawn_location), state_)};

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
          ->FindMoves(ToIdx(white_pawn_location), state_)};

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
          ->FindMoves(ToIdx(black_pawn_location), state_)};

  // Expect
  const State& returned_state = returned_states.front();
  EXPECT_TRUE(returned_states.size() == 1);
  EXPECT_TRUE(returned_state.board_.Get(location_behind_black_pawn)->IsEmpty());
  EXPECT_TRUE(returned_state.board_.Get(black_pawn_location)->IsEmpty());
  EXPECT_TRUE(returned_state.board_.Get(free_square)->GetId() ==
              Square::BlackPawn);
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
          ->FindMoves(ToIdx(black_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 4);
  for (const auto& returned_state : returned_states) {
    EXPECT_TRUE(
        returned_state.board_.Get(location_behind_black_pawn)->IsEmpty());
    EXPECT_TRUE(returned_state.board_.Get(black_pawn_location)->IsEmpty());
  }
  EXPECT_TRUE(returned_states.at(0).board_.Get(promotion_square)->GetId() ==
              Square::BlackQueen);
  EXPECT_TRUE(returned_states.at(1).board_.Get(promotion_square)->GetId() ==
              Square::BlackRook);
  EXPECT_TRUE(returned_states.at(2).board_.Get(promotion_square)->GetId() ==
              Square::BlackKnight);
  EXPECT_TRUE(returned_states.at(3).board_.Get(promotion_square)->GetId() ==
              Square::BlackBishop);
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
          ->FindMoves(ToIdx(black_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 2);
  for (const auto& returned_state : returned_states) {
    EXPECT_TRUE(
        returned_state.board_.Get(location_behind_black_pawn)->IsEmpty());
    EXPECT_TRUE(returned_state.board_.Get(black_pawn_location)->IsEmpty());
  }
  EXPECT_TRUE(returned_states.at(0).board_.Get(double_step_target)->IsEmpty());
  EXPECT_TRUE(returned_states.at(0).board_.Get(single_step_target)->GetId() ==
              Square::BlackPawn);
  EXPECT_TRUE(returned_states.at(1).board_.Get(single_step_target)->IsEmpty());
  EXPECT_TRUE(returned_states.at(1).board_.Get(double_step_target)->GetId() ==
              Square::BlackPawn);
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
          ->FindMoves(ToIdx(black_pawn_location), state_)};

  // Expect
  const State& returned_state = returned_states.front();
  EXPECT_TRUE(returned_states.size() == 1);
  EXPECT_TRUE(returned_state.board_.Get(black_pawn_location)->IsEmpty());
  EXPECT_TRUE(
      returned_state.board_.Get(white_blocking_piece_location)->GetId() ==
      Square::WhiteKing);
  EXPECT_TRUE(
      returned_state.board_.Get(white_hanging_piece_location)->GetId() ==
      Square::BlackPawn);
  EXPECT_TRUE(
      returned_state.board_.Get(black_piece_on_caputre_location)->GetId() ==
      Square::BlackKnight);
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
      state_.board_.Get(black_pawn)->FindMoves(ToIdx(black_pawn), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 1);
  const State& returned_state = returned_states.front();
  EXPECT_TRUE(returned_state.board_.Get(white_pawn)->IsEmpty());
  EXPECT_TRUE(returned_state.board_.Get(black_pawn)->IsEmpty());
  EXPECT_TRUE(returned_state.board_.Get(en_passant)->GetId() ==
              Square::BlackPawn);
  EXPECT_TRUE(returned_state.board_.Get(white_blocking_piece)->GetId() ==
              Square::WhiteKnight);
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
          ->FindMoves(ToIdx(white_pawn_location), state_)};

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
          ->FindMoves(ToIdx(black_pawn_location), state_)};

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
          ->FindMoves(ToIdx(white_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 1);
  const State& returned_state = returned_states.front();
  EXPECT_TRUE(returned_state.board_.Get(location_behind_white_pawn)->IsEmpty());
  EXPECT_TRUE(returned_state.board_.Get(white_pawn_location)->IsEmpty());
  EXPECT_TRUE(returned_state.board_.Get(free_square)->GetId() ==
              Square::WhitePawn);
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
          ->FindMoves(ToIdx(white_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 4);
  for (const auto& returned_state : returned_states) {
    EXPECT_TRUE(
        returned_state.board_.Get(location_behind_white_pawn)->IsEmpty());
    EXPECT_TRUE(returned_state.board_.Get(white_pawn_location)->IsEmpty());
  }
  EXPECT_TRUE(returned_states.at(0).board_.Get(promotion_square)->GetId() ==
              Square::WhiteQueen);
  EXPECT_TRUE(returned_states.at(1).board_.Get(promotion_square)->GetId() ==
              Square::WhiteRook);
  EXPECT_TRUE(returned_states.at(2).board_.Get(promotion_square)->GetId() ==
              Square::WhiteKnight);
  EXPECT_TRUE(returned_states.at(3).board_.Get(promotion_square)->GetId() ==
              Square::WhiteBishop);
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
          ->FindMoves(ToIdx(white_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 2);
  for (const auto& returned_state : returned_states) {
    EXPECT_TRUE(
        returned_state.board_.Get(location_behind_white_pawn)->IsEmpty());
    EXPECT_TRUE(returned_state.board_.Get(white_pawn_location)->IsEmpty());
  }
  EXPECT_TRUE(returned_states.at(0).board_.Get(double_step_target)->IsEmpty());
  EXPECT_TRUE(returned_states.at(0).board_.Get(single_step_target)->GetId() ==
              Square::WhitePawn);
  EXPECT_TRUE(returned_states.at(1).board_.Get(single_step_target)->IsEmpty());
  EXPECT_TRUE(returned_states.at(1).board_.Get(double_step_target)->GetId() ==
              Square::WhitePawn);
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
          ->FindMoves(ToIdx(white_pawn_location), state_)};

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
          ->FindMoves(ToIdx(white_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 1);
  const State& returned_state = returned_states.front();
  EXPECT_TRUE(returned_state.board_.Get(white_pawn_location)->IsEmpty());
  EXPECT_TRUE(
      returned_state.board_.Get(black_blocking_piece_location)->GetId() ==
      Square::BlackKing);
  EXPECT_TRUE(
      returned_state.board_.Get(black_hanging_piece_location)->GetId() ==
      Square::WhitePawn);
  EXPECT_TRUE(
      returned_state.board_.Get(white_piece_on_caputre_location)->GetId() ==
      Square::WhiteKnight);
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
      state_.board_.Get(white_pawn)->FindMoves(ToIdx(white_pawn), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 1);
  const State& returned_state = returned_states.front();
  EXPECT_TRUE(returned_state.board_.Get(white_pawn)->IsEmpty());
  EXPECT_TRUE(returned_state.board_.Get(black_pawn)->IsEmpty());
  EXPECT_TRUE(returned_state.board_.Get(en_passant)->GetId() ==
              Square::WhitePawn);
  EXPECT_TRUE(returned_state.board_.Get(black_blocking_piece)->GetId() ==
              Square::BlackKnight);
}

class KnightFindMoves_Fixture : public testing::Test {
 public:
  void SetUp() override { state_ = State{}; }
  void TearDown() override {}

  State state_;
};

TEST_F(KnightFindMoves_Fixture,
       GivenKnightOfPlayerWhoIsNotOnTurn_ExpectNoMoves) {
  // Setup
  const Coordinate knight_location{3, 4};
  state_.board_.Set(knight_location,
                    std::make_unique<Knight>(AlphaBeta::Player::max));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(knight_location)
          ->FindMoves(ToIdx(knight_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 0);
}

TEST_F(KnightFindMoves_Fixture, GivenKnightInCenter_ExpectEightMoves) {
  // Setup
  const Coordinate knight_location{3, 4};
  state_.board_.Set(knight_location,
                    std::make_unique<Knight>(AlphaBeta::Player::min));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(knight_location)
          ->FindMoves(ToIdx(knight_location), state_)};

  // Expect
  const int expected_plies = 1;
  const int expected_static_plies = 1;
  EXPECT_TRUE(returned_states.size() == 8);
  EXPECT_TRUE(returned_states.front().plies_ == expected_plies);
  EXPECT_TRUE(returned_states.front().static_plies_ == expected_static_plies);
}

TEST_F(KnightFindMoves_Fixture, GivenKnightInCorner_ExpectTwoMoves) {
  // Setup
  const Coordinate knight_location{0, 0};
  state_.board_.Set(knight_location,
                    std::make_unique<Knight>(AlphaBeta::Player::min));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(knight_location)
          ->FindMoves(ToIdx(knight_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 2);
}

TEST_F(KnightFindMoves_Fixture,
       GivenKnightInCornerAndPiecesOnTargetSquares_ExpectCapture) {
  // Setup
  const Coordinate knight_location{0, 0};
  const Coordinate own_piece_location{1, 2};
  const Coordinate opponent_piece_location{2, 1};
  state_.board_.Set(knight_location,
                    std::make_unique<Knight>(AlphaBeta::Player::min));
  state_.board_.Set(own_piece_location,
                    std::make_unique<Rook>(AlphaBeta::Player::min));
  state_.board_.Set(opponent_piece_location,
                    std::make_unique<Rook>(AlphaBeta::Player::max));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(knight_location)
          ->FindMoves(ToIdx(knight_location), state_)};

  // Expect
  const int expected_plies = 1;
  const int expected_static_plies = 0;
  EXPECT_TRUE(returned_states.size() == 1);
  EXPECT_TRUE(returned_states.front().plies_ == expected_plies);
  EXPECT_TRUE(returned_states.front().static_plies_ == expected_static_plies);
}

}  // namespace
}  // namespace Chess
