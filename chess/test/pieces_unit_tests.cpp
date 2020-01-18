#include "chess/pieces.h"

#include "chess/game.h"

#include <gtest/gtest.h>
#include <vector>

namespace Chess {
namespace {

class BlackPawnFindPlies_Fixture : public testing::Test {
 public:
  void SetUp() override { state_.turn_ = AlphaBeta::Player::min; }
  void TearDown() override {}

  State state_{SetUpEmptyBoard()};
};

TEST_F(BlackPawnFindPlies_Fixture, GivenNoFreeSquareInfront_ExpectNoPlies) {
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
          ->FindPlies(ToIdx(black_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 0);
}

TEST_F(BlackPawnFindPlies_Fixture, GivenWhitePawn_ExpectNoPlies) {
  // Setup
  const Coordinate white_pawn_location{3, 3};
  state_.board_.Set(white_pawn_location,
                    std::make_unique<Pawn>(AlphaBeta::Player::max));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(white_pawn_location)
          ->FindPlies(ToIdx(white_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 0);
}

TEST_F(BlackPawnFindPlies_Fixture,
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
          ->FindPlies(ToIdx(black_pawn_location), state_)};

  // Expect
  const State& returned_state = returned_states.front();
  EXPECT_TRUE(returned_states.size() == 1);
  EXPECT_TRUE(returned_state.board_.Get(location_behind_black_pawn)->IsEmpty());
  EXPECT_TRUE(returned_state.board_.Get(black_pawn_location)->IsEmpty());
  EXPECT_TRUE(returned_state.board_.Get(free_square)->GetId() ==
              SquareId::BlackPawn);
  EXPECT_FALSE(returned_state.en_passant_);
}

TEST_F(BlackPawnFindPlies_Fixture,
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
          ->FindPlies(ToIdx(black_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 4);
  for (const auto& returned_state : returned_states) {
    EXPECT_TRUE(
        returned_state.board_.Get(location_behind_black_pawn)->IsEmpty());
    EXPECT_TRUE(returned_state.board_.Get(black_pawn_location)->IsEmpty());
  }
  EXPECT_TRUE(returned_states.at(0).board_.Get(promotion_square)->GetId() ==
              SquareId::BlackQueen);
  EXPECT_TRUE(returned_states.at(1).board_.Get(promotion_square)->GetId() ==
              SquareId::BlackRook);
  EXPECT_TRUE(returned_states.at(2).board_.Get(promotion_square)->GetId() ==
              SquareId::BlackKnight);
  EXPECT_TRUE(returned_states.at(3).board_.Get(promotion_square)->GetId() ==
              SquareId::BlackBishop);
}

TEST_F(BlackPawnFindPlies_Fixture,
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
          ->FindPlies(ToIdx(black_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 2);
  for (const auto& returned_state : returned_states) {
    EXPECT_TRUE(
        returned_state.board_.Get(location_behind_black_pawn)->IsEmpty());
    EXPECT_TRUE(returned_state.board_.Get(black_pawn_location)->IsEmpty());
  }
  EXPECT_TRUE(returned_states.at(0).board_.Get(double_step_target)->IsEmpty());
  EXPECT_TRUE(returned_states.at(0).board_.Get(single_step_target)->GetId() ==
              SquareId::BlackPawn);
  EXPECT_TRUE(returned_states.at(1).board_.Get(single_step_target)->IsEmpty());
  EXPECT_TRUE(returned_states.at(1).board_.Get(double_step_target)->GetId() ==
              SquareId::BlackPawn);
}

TEST_F(BlackPawnFindPlies_Fixture, GivenOneCapturePossible_ExpectOnePly) {
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
          ->FindPlies(ToIdx(black_pawn_location), state_)};

  // Expect
  const State& returned_state = returned_states.front();
  EXPECT_TRUE(returned_states.size() == 1);
  EXPECT_TRUE(returned_state.board_.Get(black_pawn_location)->IsEmpty());
  EXPECT_TRUE(
      returned_state.board_.Get(white_blocking_piece_location)->GetId() ==
      SquareId::WhiteKing);
  EXPECT_TRUE(
      returned_state.board_.Get(white_hanging_piece_location)->GetId() ==
      SquareId::BlackPawn);
  EXPECT_TRUE(
      returned_state.board_.Get(black_piece_on_caputre_location)->GetId() ==
      SquareId::BlackKnight);
}

TEST_F(BlackPawnFindPlies_Fixture, GivenEnPassant_ExpectCapture) {
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
      state_.board_.Get(black_pawn)->FindPlies(ToIdx(black_pawn), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 1);
  const State& returned_state = returned_states.front();
  EXPECT_TRUE(returned_state.board_.Get(white_pawn)->IsEmpty());
  EXPECT_TRUE(returned_state.board_.Get(black_pawn)->IsEmpty());
  EXPECT_TRUE(returned_state.board_.Get(en_passant)->GetId() ==
              SquareId::BlackPawn);
  EXPECT_TRUE(returned_state.board_.Get(white_blocking_piece)->GetId() ==
              SquareId::WhiteKnight);
}

class WhitePawnFindPlies_Fixture : public testing::Test {
 public:
  void SetUp() override { state_.turn_ = AlphaBeta::Player::max; }
  void TearDown() override {}

  State state_{SetUpEmptyBoard()};
};

TEST_F(WhitePawnFindPlies_Fixture, GivenNoFreeSquareInfront_ExpectNoPlies) {
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
          ->FindPlies(ToIdx(white_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 0);
}

TEST_F(WhitePawnFindPlies_Fixture, GivenBlackPawn_ExpectNoPlies) {
  // Setup
  const Coordinate black_pawn_location{3, 3};
  state_.board_.Set(black_pawn_location,
                    std::make_unique<Pawn>(AlphaBeta::Player::min));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(black_pawn_location)
          ->FindPlies(ToIdx(black_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 0);
}

TEST_F(WhitePawnFindPlies_Fixture,
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
          ->FindPlies(ToIdx(white_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 1);
  const State& returned_state = returned_states.front();
  EXPECT_TRUE(returned_state.board_.Get(location_behind_white_pawn)->IsEmpty());
  EXPECT_TRUE(returned_state.board_.Get(white_pawn_location)->IsEmpty());
  EXPECT_TRUE(returned_state.board_.Get(free_square)->GetId() ==
              SquareId::WhitePawn);
  EXPECT_FALSE(returned_state.en_passant_);
}

TEST_F(WhitePawnFindPlies_Fixture,
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
          ->FindPlies(ToIdx(white_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 4);
  for (const auto& returned_state : returned_states) {
    EXPECT_TRUE(
        returned_state.board_.Get(location_behind_white_pawn)->IsEmpty());
    EXPECT_TRUE(returned_state.board_.Get(white_pawn_location)->IsEmpty());
  }
  EXPECT_TRUE(returned_states.at(0).board_.Get(promotion_square)->GetId() ==
              SquareId::WhiteQueen);
  EXPECT_TRUE(returned_states.at(1).board_.Get(promotion_square)->GetId() ==
              SquareId::WhiteRook);
  EXPECT_TRUE(returned_states.at(2).board_.Get(promotion_square)->GetId() ==
              SquareId::WhiteKnight);
  EXPECT_TRUE(returned_states.at(3).board_.Get(promotion_square)->GetId() ==
              SquareId::WhiteBishop);
}

TEST_F(WhitePawnFindPlies_Fixture,
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
          ->FindPlies(ToIdx(white_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 2);
  for (const auto& returned_state : returned_states) {
    EXPECT_TRUE(
        returned_state.board_.Get(location_behind_white_pawn)->IsEmpty());
    EXPECT_TRUE(returned_state.board_.Get(white_pawn_location)->IsEmpty());
  }
  EXPECT_TRUE(returned_states.at(0).board_.Get(double_step_target)->IsEmpty());
  EXPECT_TRUE(returned_states.at(0).board_.Get(single_step_target)->GetId() ==
              SquareId::WhitePawn);
  EXPECT_TRUE(returned_states.at(1).board_.Get(single_step_target)->IsEmpty());
  EXPECT_TRUE(returned_states.at(1).board_.Get(double_step_target)->GetId() ==
              SquareId::WhitePawn);
}

TEST_F(WhitePawnFindPlies_Fixture,
       GivenDoubleStep_ExpectEnPassantOnlyForDoubleStep) {
  // Setup
  const Coordinate white_pawn_location{4, 6};
  const Coordinate single_step_target{4, 5};
  state_.board_.Set(white_pawn_location,
                    std::make_unique<Pawn>(AlphaBeta::Player::max));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(white_pawn_location)
          ->FindPlies(ToIdx(white_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.at(1).en_passant_ == single_step_target);
}

TEST_F(WhitePawnFindPlies_Fixture, GivenOneCapturePossible_ExpectOnePly) {
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
          ->FindPlies(ToIdx(white_pawn_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 1);
  const State& returned_state = returned_states.front();
  EXPECT_TRUE(returned_state.board_.Get(white_pawn_location)->IsEmpty());
  EXPECT_TRUE(
      returned_state.board_.Get(black_blocking_piece_location)->GetId() ==
      SquareId::BlackKing);
  EXPECT_TRUE(
      returned_state.board_.Get(black_hanging_piece_location)->GetId() ==
      SquareId::WhitePawn);
  EXPECT_TRUE(
      returned_state.board_.Get(white_piece_on_caputre_location)->GetId() ==
      SquareId::WhiteKnight);
}

TEST_F(WhitePawnFindPlies_Fixture, GivenEnPassant_ExpectCapture) {
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
      state_.board_.Get(white_pawn)->FindPlies(ToIdx(white_pawn), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 1);
  const State& returned_state = returned_states.front();
  EXPECT_TRUE(returned_state.board_.Get(white_pawn)->IsEmpty());
  EXPECT_TRUE(returned_state.board_.Get(black_pawn)->IsEmpty());
  EXPECT_TRUE(returned_state.board_.Get(en_passant)->GetId() ==
              SquareId::WhitePawn);
  EXPECT_TRUE(returned_state.board_.Get(black_blocking_piece)->GetId() ==
              SquareId::BlackKnight);
}

class PiecePlies_Fixture : public testing::Test {
 public:
  void SetUp() override { state_.turn_ = AlphaBeta::Player::min; }
  void TearDown() override {}

  State state_{SetUpEmptyBoard()};
};

TEST_F(PiecePlies_Fixture, GivenKnightOfPlayerWhoIsNotOnTurn_ExpectNoPlies) {
  // Setup
  const Coordinate knight_location{3, 4};
  state_.board_.Set(knight_location,
                    std::make_unique<Knight>(AlphaBeta::Player::max));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(knight_location)
          ->FindPlies(ToIdx(knight_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 0);
}

TEST_F(PiecePlies_Fixture, GivenKnightInCenter_ExpectEightPlies) {
  // Setup
  const Coordinate knight_location{3, 4};
  state_.board_.Set(knight_location,
                    std::make_unique<Knight>(AlphaBeta::Player::min));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(knight_location)
          ->FindPlies(ToIdx(knight_location), state_)};

  // Expect
  const int expected_plies = 1;
  const int expected_static_plies = 1;
  EXPECT_TRUE(returned_states.size() == 8);
  EXPECT_TRUE(returned_states.front().plies_ == expected_plies);
  EXPECT_TRUE(returned_states.front().static_plies_ == expected_static_plies);
}

TEST_F(PiecePlies_Fixture, GivenKnightInCorner_ExpectTwoPlies) {
  // Setup
  const Coordinate knight_location{0, 0};
  state_.board_.Set(knight_location,
                    std::make_unique<Knight>(AlphaBeta::Player::min));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(knight_location)
          ->FindPlies(ToIdx(knight_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 2);
}

TEST_F(PiecePlies_Fixture,
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
          ->FindPlies(ToIdx(knight_location), state_)};

  // Expect
  const int expected_plies = 1;
  const int expected_static_plies = 0;
  EXPECT_TRUE(returned_states.size() == 1);
  EXPECT_TRUE(returned_states.front().plies_ == expected_plies);
  EXPECT_TRUE(returned_states.front().static_plies_ == expected_static_plies);
}

TEST_F(
    PiecePlies_Fixture,
    GivenBishopInCenterWithOneCapturePossible_ExpectAppropriateNumberOfPlies) {
  // Setup
  const Coordinate bishop_location{3, 4};
  const Coordinate own_piece_location{1, 2};
  const Coordinate opponent_piece_location{5, 2};
  state_.board_.Set(bishop_location,
                    std::make_unique<Bishop>(AlphaBeta::Player::min));
  state_.board_.Set(own_piece_location,
                    std::make_unique<Rook>(AlphaBeta::Player::min));
  state_.board_.Set(opponent_piece_location,
                    std::make_unique<Rook>(AlphaBeta::Player::max));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(bishop_location)
          ->FindPlies(ToIdx(bishop_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 9);
}

TEST_F(PiecePlies_Fixture,
       GivenRookInCenterWithOneCapturePossible_ExpectAppropriateNumberOfPlies) {
  // Setup
  const Coordinate rook_location{3, 4};
  const Coordinate own_piece_location{1, 4};
  const Coordinate opponent_piece_location{3, 2};
  state_.board_.Set(rook_location,
                    std::make_unique<Rook>(AlphaBeta::Player::min));
  state_.board_.Set(own_piece_location,
                    std::make_unique<Rook>(AlphaBeta::Player::min));
  state_.board_.Set(opponent_piece_location,
                    std::make_unique<Rook>(AlphaBeta::Player::max));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(rook_location)
          ->FindPlies(ToIdx(rook_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 10);
}

TEST_F(
    PiecePlies_Fixture,
    GivenQueenInCenterWithOneCapturePossible_ExpectAppropriateNumberOfPlies) {
  // Setup
  const Coordinate queen_location{3, 4};
  const Coordinate own_piece_location{1, 4};
  const Coordinate opponent_piece_location{3, 2};
  state_.board_.Set(queen_location,
                    std::make_unique<Queen>(AlphaBeta::Player::min));
  state_.board_.Set(own_piece_location,
                    std::make_unique<Rook>(AlphaBeta::Player::min));
  state_.board_.Set(opponent_piece_location,
                    std::make_unique<Rook>(AlphaBeta::Player::max));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(queen_location)
          ->FindPlies(ToIdx(queen_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 23);
}

TEST_F(PiecePlies_Fixture,
       GivenKingOnEdgeWithOneCapturePossible_ExpectAppropriateNumberOfPlies) {
  // Setup
  const Coordinate king_location{0, 3};
  const Coordinate own_piece_location{1, 3};
  const Coordinate opponent_piece_location{0, 4};
  state_.board_.Set(king_location,
                    std::make_unique<King>(AlphaBeta::Player::min));
  state_.board_.Set(own_piece_location,
                    std::make_unique<Rook>(AlphaBeta::Player::min));
  state_.board_.Set(opponent_piece_location,
                    std::make_unique<Rook>(AlphaBeta::Player::max));

  // Call
  const std::vector<State> returned_states{
      state_.board_.Get(king_location)
          ->FindPlies(ToIdx(king_location), state_)};

  // Expect
  EXPECT_TRUE(returned_states.size() == 4);
}

}  // namespace
}  // namespace Chess
