#include "board/pieces.h"

#include <gtest/gtest.h>
#include <vector>

namespace Chess {
namespace {

class BlackPawnFindPlies_Fixture : public testing::Test {
 public:
  void SetUp() override {
    position_.plies_ = 1;  // make it blacks turn
  }
  void TearDown() override {}

  Position position_{};
};

TEST(Piece, StreamOutputOperator_MustNotThrow) {
  EXPECT_NO_THROW(std::cout << Bishop::OfSide(Player::max) << '\n');
}

TEST(IsAPieceOfSide, GivenBlackPieces_ExpectBlackPiecesTrueRestFalse) {
  EXPECT_TRUE(King::OfSide(Player::min)->IsOfSide(Player::min));
  EXPECT_TRUE(Queen::OfSide(Player::min)->IsOfSide(Player::min));
  EXPECT_TRUE(Rook::OfSide(Player::min)->IsOfSide(Player::min));
  EXPECT_TRUE(Bishop::OfSide(Player::min)->IsOfSide(Player::min));
  EXPECT_TRUE(Knight::OfSide(Player::min)->IsOfSide(Player::min));
  EXPECT_TRUE(Pawn::OfSide(Player::min)->IsOfSide(Player::min));
  EXPECT_FALSE(Empty::Make()->IsOfSide(Player::min));
  EXPECT_FALSE(King::OfSide(Player::max)->IsOfSide(Player::min));
  EXPECT_FALSE(Queen::OfSide(Player::max)->IsOfSide(Player::min));
  EXPECT_FALSE(Rook::OfSide(Player::max)->IsOfSide(Player::min));
  EXPECT_FALSE(Bishop::OfSide(Player::max)->IsOfSide(Player::min));
  EXPECT_FALSE(Knight::OfSide(Player::max)->IsOfSide(Player::min));
  EXPECT_FALSE(Pawn::OfSide(Player::max)->IsOfSide(Player::min));
}

TEST(IsAPieceOfSide, GivenWhitePieces_ExpectWhitePiecesTrueRestFalse) {
  EXPECT_FALSE(King::OfSide(Player::min)->IsOfSide(Player::max));
  EXPECT_FALSE(Queen::OfSide(Player::min)->IsOfSide(Player::max));
  EXPECT_FALSE(Rook::OfSide(Player::min)->IsOfSide(Player::max));
  EXPECT_FALSE(Bishop::OfSide(Player::min)->IsOfSide(Player::max));
  EXPECT_FALSE(Knight::OfSide(Player::min)->IsOfSide(Player::max));
  EXPECT_FALSE(Pawn::OfSide(Player::min)->IsOfSide(Player::max));
  EXPECT_FALSE(Empty::Make()->IsOfSide(Player::max));
  EXPECT_TRUE(King::OfSide(Player::max)->IsOfSide(Player::max));
  EXPECT_TRUE(Queen::OfSide(Player::max)->IsOfSide(Player::max));
  EXPECT_TRUE(Rook::OfSide(Player::max)->IsOfSide(Player::max));
  EXPECT_TRUE(Bishop::OfSide(Player::max)->IsOfSide(Player::max));
  EXPECT_TRUE(Knight::OfSide(Player::max)->IsOfSide(Player::max));
  EXPECT_TRUE(Pawn::OfSide(Player::max)->IsOfSide(Player::max));
}

TEST_F(BlackPawnFindPlies_Fixture, GivenNoFreeSquareInfront_ExpectNoPlies) {
  // Setup
  const Coordinate black_pawn_location{3, 1};
  const Coordinate blocking_piece_location{3, 2};
  position_.board_.Set(black_pawn_location, Pawn::OfSide(Player::min));
  position_.board_.Set(blocking_piece_location, King::OfSide(Player::max));

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(black_pawn_location)
          ->FindPlies(ToIdx(black_pawn_location), position_)};

  // Expect
  EXPECT_TRUE(returned_positions.empty());
}

TEST_F(BlackPawnFindPlies_Fixture, GivenWhitePawn_ExpectNoPlies) {
  // Setup
  const Coordinate white_pawn_location{3, 3};
  position_.board_.Set(white_pawn_location, Pawn::OfSide(Player::max));

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(white_pawn_location)
          ->FindPlies(ToIdx(white_pawn_location), position_)};

  // Expect
  EXPECT_TRUE(returned_positions.empty());
}

TEST_F(BlackPawnFindPlies_Fixture,
       GivenLocationNotOnStartRankAndFreeSquareInfront_ExpectSingleStepOnly) {
  // Setup
  const Coordinate location_behind_black_pawn{3, 3};
  const Coordinate black_pawn_location{3, 4};
  const Coordinate free_square{3, 5};
  position_.board_.Set(black_pawn_location, Pawn::OfSide(Player::min));

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(black_pawn_location)
          ->FindPlies(ToIdx(black_pawn_location), position_)};

  // Expect
  const Position& returned_position = returned_positions.front();
  EXPECT_EQ(returned_positions.size(), 1);
  EXPECT_TRUE(
      returned_position.board_.Get(location_behind_black_pawn)->IsEmpty());
  EXPECT_TRUE(returned_position.board_.Get(black_pawn_location)->IsEmpty());
  EXPECT_EQ(returned_position.board_.Get(free_square)->GetId(),
            SquareId::BlackPawn);
  EXPECT_FALSE(returned_position.en_passant_);
}

TEST_F(BlackPawnFindPlies_Fixture,
       GivenFreeSquareOnPromotionRank_ExpectAllPromotions) {
  // Setup
  const Coordinate location_behind_black_pawn{3, 5};
  const Coordinate black_pawn_location{3, 6};
  const Coordinate promotion_square{3, 7};
  position_.board_.Set(black_pawn_location, Pawn::OfSide(Player::min));

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(black_pawn_location)
          ->FindPlies(ToIdx(black_pawn_location), position_)};

  // Expect
  EXPECT_EQ(returned_positions.size(), 4);
  for (const auto& returned_position : returned_positions) {
    EXPECT_TRUE(
        returned_position.board_.Get(location_behind_black_pawn)->IsEmpty());
    EXPECT_TRUE(returned_position.board_.Get(black_pawn_location)->IsEmpty());
  }
  EXPECT_EQ(returned_positions.at(0).board_.Get(promotion_square)->GetId(),
            SquareId::BlackQueen);
  EXPECT_EQ(returned_positions.at(1).board_.Get(promotion_square)->GetId(),
            SquareId::BlackRook);
  EXPECT_EQ(returned_positions.at(2).board_.Get(promotion_square)->GetId(),
            SquareId::BlackKnight);
  EXPECT_EQ(returned_positions.at(3).board_.Get(promotion_square)->GetId(),
            SquareId::BlackBishop);
}

TEST_F(BlackPawnFindPlies_Fixture,
       GivenLocationOnStartRankAndTwoFreeSquaresInfront_ExpectAlsoDoubleStep) {
  // Setup
  const Coordinate location_behind_black_pawn{4, 0};
  const Coordinate black_pawn_location{4, 1};
  const Coordinate single_step_target{4, 2};
  const Coordinate double_step_target{4, 3};
  position_.board_.Set(black_pawn_location, Pawn::OfSide(Player::min));

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(black_pawn_location)
          ->FindPlies(ToIdx(black_pawn_location), position_)};

  // Expect
  EXPECT_EQ(returned_positions.size(), 2);
  for (const auto& returned_position : returned_positions) {
    EXPECT_TRUE(
        returned_position.board_.Get(location_behind_black_pawn)->IsEmpty());
    EXPECT_TRUE(returned_position.board_.Get(black_pawn_location)->IsEmpty());
  }
  EXPECT_TRUE(
      returned_positions.at(0).board_.Get(double_step_target)->IsEmpty());
  EXPECT_EQ(returned_positions.at(0).board_.Get(single_step_target)->GetId(),
            SquareId::BlackPawn);
  EXPECT_TRUE(
      returned_positions.at(1).board_.Get(single_step_target)->IsEmpty());
  EXPECT_EQ(returned_positions.at(1).board_.Get(double_step_target)->GetId(),
            SquareId::BlackPawn);
}

TEST_F(BlackPawnFindPlies_Fixture, GivenOneCapturePossible_ExpectOnePly) {
  // Setup
  const Coordinate black_pawn_location{3, 3};
  const Coordinate white_blocking_piece_location{3, 4};
  const Coordinate white_hanging_piece_location{4, 4};
  const Coordinate black_piece_on_capture_location{2, 4};
  position_.board_.Set(black_pawn_location, Pawn::OfSide(Player::min));
  position_.board_.Set(black_piece_on_capture_location,
                       Knight::OfSide(Player::min));
  position_.board_.Set(white_blocking_piece_location,
                       King::OfSide(Player::max));
  position_.board_.Set(white_hanging_piece_location, Rook::OfSide(Player::max));

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(black_pawn_location)
          ->FindPlies(ToIdx(black_pawn_location), position_)};

  // Expect
  const Position& returned_position = returned_positions.front();
  EXPECT_EQ(returned_positions.size(), 1);
  EXPECT_TRUE(returned_position.board_.Get(black_pawn_location)->IsEmpty());
  EXPECT_EQ(
      returned_position.board_.Get(white_blocking_piece_location)->GetId(),
      SquareId::WhiteKing);
  EXPECT_EQ(returned_position.board_.Get(white_hanging_piece_location)->GetId(),
            SquareId::BlackPawn);
  EXPECT_EQ(
      returned_position.board_.Get(black_piece_on_capture_location)->GetId(),
      SquareId::BlackKnight);
}

TEST_F(BlackPawnFindPlies_Fixture, GivenEnPassant_ExpectCapture) {
  // Setup
  const Coordinate black_pawn{5, 4};
  const Coordinate white_pawn{4, 4};
  const Coordinate white_blocking_piece{5, 5};
  const Coordinate en_passant{4, 5};
  position_.board_.Set(black_pawn, Pawn::OfSide(Player::min));
  position_.board_.Set(white_pawn, Pawn::OfSide(Player::max));
  position_.board_.Set(white_blocking_piece, Knight::OfSide(Player::max));
  position_.en_passant_ = en_passant;

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(black_pawn)
          ->FindPlies(ToIdx(black_pawn), position_)};

  // Expect
  EXPECT_EQ(returned_positions.size(), 1);
  const Position& returned_position = returned_positions.front();
  EXPECT_TRUE(returned_position.board_.Get(white_pawn)->IsEmpty());
  EXPECT_TRUE(returned_position.board_.Get(black_pawn)->IsEmpty());
  EXPECT_EQ(returned_position.board_.Get(en_passant)->GetId(),
            SquareId::BlackPawn);
  EXPECT_EQ(returned_position.board_.Get(white_blocking_piece)->GetId(),
            SquareId::WhiteKnight);
}

class WhitePawnFindPlies_Fixture : public testing::Test {
 public:
  Position position_{};
};

TEST_F(WhitePawnFindPlies_Fixture, GivenNoFreeSquareInfront_ExpectNoPlies) {
  // Setup
  const Coordinate white_pawn_location{3, 6};
  const Coordinate blocking_piece_location{3, 5};
  position_.board_.Set(white_pawn_location, Pawn::OfSide(Player::max));
  position_.board_.Set(blocking_piece_location, King::OfSide(Player::min));

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(white_pawn_location)
          ->FindPlies(ToIdx(white_pawn_location), position_)};

  // Expect
  EXPECT_TRUE(returned_positions.empty());
}

TEST_F(WhitePawnFindPlies_Fixture, GivenBlackPawn_ExpectNoPlies) {
  // Setup
  const Coordinate black_pawn_location{3, 3};
  position_.board_.Set(black_pawn_location, Pawn::OfSide(Player::min));

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(black_pawn_location)
          ->FindPlies(ToIdx(black_pawn_location), position_)};

  // Expect
  EXPECT_TRUE(returned_positions.empty());
}

TEST_F(WhitePawnFindPlies_Fixture,
       GivenLocationNotOnStartRankAndFreeSquareInfront_ExpectSingleStepOnly) {
  // Setup
  const Coordinate location_behind_white_pawn{3, 5};
  const Coordinate white_pawn_location{3, 4};
  const Coordinate free_square{3, 3};
  position_.board_.Set(white_pawn_location, Pawn::OfSide(Player::max));

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(white_pawn_location)
          ->FindPlies(ToIdx(white_pawn_location), position_)};

  // Expect
  EXPECT_EQ(returned_positions.size(), 1);
  const Position& returned_position = returned_positions.front();
  EXPECT_TRUE(
      returned_position.board_.Get(location_behind_white_pawn)->IsEmpty());
  EXPECT_TRUE(returned_position.board_.Get(white_pawn_location)->IsEmpty());
  EXPECT_EQ(returned_position.board_.Get(free_square)->GetId(),
            SquareId::WhitePawn);
  EXPECT_FALSE(returned_position.en_passant_);
}

TEST_F(WhitePawnFindPlies_Fixture,
       GivenFreeSquareOnPromotionRank_ExpectAllPromotions) {
  // Setup
  const Coordinate location_behind_white_pawn{3, 2};
  const Coordinate white_pawn_location{3, 1};
  const Coordinate promotion_square{3, 0};
  position_.board_.Set(white_pawn_location, Pawn::OfSide(Player::max));

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(white_pawn_location)
          ->FindPlies(ToIdx(white_pawn_location), position_)};

  // Expect
  EXPECT_EQ(returned_positions.size(), 4);
  for (const auto& returned_position : returned_positions) {
    EXPECT_TRUE(
        returned_position.board_.Get(location_behind_white_pawn)->IsEmpty());
    EXPECT_TRUE(returned_position.board_.Get(white_pawn_location)->IsEmpty());
  }
  EXPECT_EQ(returned_positions.at(0).board_.Get(promotion_square)->GetId(),
            SquareId::WhiteQueen);
  EXPECT_EQ(returned_positions.at(1).board_.Get(promotion_square)->GetId(),
            SquareId::WhiteRook);
  EXPECT_EQ(returned_positions.at(2).board_.Get(promotion_square)->GetId(),
            SquareId::WhiteKnight);
  EXPECT_EQ(returned_positions.at(3).board_.Get(promotion_square)->GetId(),
            SquareId::WhiteBishop);
}

TEST_F(WhitePawnFindPlies_Fixture,
       GivenLocationOnStartRankAndTwoFreeSquaresInfront_ExpectAlsoDoubleStep) {
  // Setup
  const Coordinate location_behind_white_pawn{4, 7};
  const Coordinate white_pawn_location{4, 6};
  const Coordinate single_step_target{4, 5};
  const Coordinate double_step_target{4, 4};
  position_.board_.Set(white_pawn_location, Pawn::OfSide(Player::max));

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(white_pawn_location)
          ->FindPlies(ToIdx(white_pawn_location), position_)};

  // Expect
  EXPECT_EQ(returned_positions.size(), 2);
  for (const auto& returned_position : returned_positions) {
    EXPECT_TRUE(
        returned_position.board_.Get(location_behind_white_pawn)->IsEmpty());
    EXPECT_TRUE(returned_position.board_.Get(white_pawn_location)->IsEmpty());
  }
  EXPECT_TRUE(
      returned_positions.at(0).board_.Get(double_step_target)->IsEmpty());
  EXPECT_EQ(returned_positions.at(0).board_.Get(single_step_target)->GetId(),
            SquareId::WhitePawn);
  EXPECT_TRUE(
      returned_positions.at(1).board_.Get(single_step_target)->IsEmpty());
  EXPECT_EQ(returned_positions.at(1).board_.Get(double_step_target)->GetId(),
            SquareId::WhitePawn);
}

TEST_F(WhitePawnFindPlies_Fixture,
       GivenDoubleStep_ExpectEnPassantOnlyForDoubleStep) {
  // Setup
  const Coordinate white_pawn_location{4, 6};
  const Coordinate single_step_target{4, 5};
  position_.board_.Set(white_pawn_location, Pawn::OfSide(Player::max));

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(white_pawn_location)
          ->FindPlies(ToIdx(white_pawn_location), position_)};

  // Expect
  EXPECT_EQ(returned_positions.at(1).en_passant_, single_step_target);
}

TEST_F(WhitePawnFindPlies_Fixture, GivenOneCapturePossible_ExpectOnePly) {
  // Setup
  const Coordinate white_pawn_location{5, 5};
  const Coordinate black_blocking_piece_location{5, 4};
  const Coordinate black_hanging_piece_location{6, 4};
  const Coordinate white_piece_on_capture_location{4, 4};
  position_.board_.Set(white_pawn_location, Pawn::OfSide(Player::max));
  position_.board_.Set(white_piece_on_capture_location,
                       Knight::OfSide(Player::max));
  position_.board_.Set(black_blocking_piece_location,
                       King::OfSide(Player::min));
  position_.board_.Set(black_hanging_piece_location, Rook::OfSide(Player::min));

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(white_pawn_location)
          ->FindPlies(ToIdx(white_pawn_location), position_)};

  // Expect
  EXPECT_EQ(returned_positions.size(), 1);
  const Position& returned_position = returned_positions.front();
  EXPECT_TRUE(returned_position.board_.Get(white_pawn_location)->IsEmpty());
  EXPECT_EQ(
      returned_position.board_.Get(black_blocking_piece_location)->GetId(),
      SquareId::BlackKing);
  EXPECT_EQ(returned_position.board_.Get(black_hanging_piece_location)->GetId(),
            SquareId::WhitePawn);
  EXPECT_EQ(
      returned_position.board_.Get(white_piece_on_capture_location)->GetId(),
      SquareId::WhiteKnight);
}

TEST_F(WhitePawnFindPlies_Fixture, GivenEnPassant_ExpectCapture) {
  // Setup
  const Coordinate white_pawn{4, 3};
  const Coordinate black_pawn{5, 3};
  const Coordinate black_blocking_piece{4, 2};
  const Coordinate en_passant{5, 2};
  position_.board_.Set(white_pawn, Pawn::OfSide(Player::max));
  position_.board_.Set(black_pawn, Pawn::OfSide(Player::min));
  position_.board_.Set(black_blocking_piece, Knight::OfSide(Player::min));
  position_.en_passant_ = en_passant;

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(white_pawn)
          ->FindPlies(ToIdx(white_pawn), position_)};

  // Expect
  EXPECT_EQ(returned_positions.size(), 1);
  const Position& returned_position = returned_positions.front();
  EXPECT_TRUE(returned_position.board_.Get(white_pawn)->IsEmpty());
  EXPECT_TRUE(returned_position.board_.Get(black_pawn)->IsEmpty());
  EXPECT_EQ(returned_position.board_.Get(en_passant)->GetId(),
            SquareId::WhitePawn);
  EXPECT_EQ(returned_position.board_.Get(black_blocking_piece)->GetId(),
            SquareId::BlackKnight);
}

class PiecePlies_Fixture : public testing::Test {
 public:
  void SetUp() override {
    position_.plies_ = start_ply_;  // make it blacks turn
  }
  void TearDown() override {}

  Position position_{};
  const int start_ply_{1};
};

TEST_F(PiecePlies_Fixture, GivenKnightOfPlayerWhoIsNotOnTurn_ExpectNoPlies) {
  // Setup
  const Coordinate knight_location{3, 4};
  position_.board_.Set(knight_location, Knight::OfSide(Player::max));

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(knight_location)
          ->FindPlies(ToIdx(knight_location), position_)};

  // Expect
  EXPECT_TRUE(returned_positions.empty());
}

TEST_F(PiecePlies_Fixture, GivenKnightInCenter_ExpectEightPlies) {
  // Setup
  const Coordinate knight_location{3, 4};
  position_.board_.Set(knight_location, Knight::OfSide(Player::min));

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(knight_location)
          ->FindPlies(ToIdx(knight_location), position_)};

  // Expect
  const int expected_plies = start_ply_ + 1;
  const int expected_static_plies = 1;
  EXPECT_EQ(returned_positions.size(), 8);
  EXPECT_EQ(returned_positions.front().plies_, expected_plies);
  EXPECT_EQ(returned_positions.front().static_plies_, expected_static_plies);
}

TEST_F(PiecePlies_Fixture, GivenKnightInCorner_ExpectTwoPlies) {
  // Setup
  const Coordinate knight_location{0, 0};
  position_.board_.Set(knight_location, Knight::OfSide(Player::min));

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(knight_location)
          ->FindPlies(ToIdx(knight_location), position_)};

  // Expect
  EXPECT_EQ(returned_positions.size(), 2);
}

TEST_F(PiecePlies_Fixture,
       GivenKnightInCornerAndPiecesOnTargetSquares_ExpectCapture) {
  // Setup
  const Coordinate knight_location{0, 0};
  const Coordinate own_piece_location{1, 2};
  const Coordinate opponent_piece_location{2, 1};
  position_.board_.Set(knight_location, Knight::OfSide(Player::min));
  position_.board_.Set(own_piece_location, Rook::OfSide(Player::min));
  position_.board_.Set(opponent_piece_location, Rook::OfSide(Player::max));

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(knight_location)
          ->FindPlies(ToIdx(knight_location), position_)};

  // Expect
  const int expected_plies = start_ply_ + 1;
  const int expected_static_plies = 0;
  EXPECT_EQ(returned_positions.size(), 1);
  EXPECT_EQ(returned_positions.front().plies_, expected_plies);
  EXPECT_EQ(returned_positions.front().static_plies_, expected_static_plies);
}

TEST_F(
    PiecePlies_Fixture,
    GivenBishopInCenterWithOneCapturePossible_ExpectAppropriateNumberOfPlies) {
  // Setup
  const Coordinate bishop_location{3, 4};
  const Coordinate own_piece_location{1, 2};
  const Coordinate opponent_piece_location{5, 2};
  position_.board_.Set(bishop_location, Bishop::OfSide(Player::min));
  position_.board_.Set(own_piece_location, Rook::OfSide(Player::min));
  position_.board_.Set(opponent_piece_location, Rook::OfSide(Player::max));

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(bishop_location)
          ->FindPlies(ToIdx(bishop_location), position_)};

  // Expect
  EXPECT_EQ(returned_positions.size(), 9);
}

TEST_F(PiecePlies_Fixture,
       GivenRookInCenterWithOneCapturePossible_ExpectAppropriateNumberOfPlies) {
  // Setup
  const Coordinate rook_location{3, 4};
  const Coordinate own_piece_location{1, 4};
  const Coordinate opponent_piece_location{3, 2};
  position_.board_.Set(rook_location, Rook::OfSide(Player::min));
  position_.board_.Set(own_piece_location, Rook::OfSide(Player::min));
  position_.board_.Set(opponent_piece_location, Rook::OfSide(Player::max));

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(rook_location)
          ->FindPlies(ToIdx(rook_location), position_)};

  // Expect
  EXPECT_EQ(returned_positions.size(), 10);
}

TEST_F(
    PiecePlies_Fixture,
    GivenQueenInCenterWithOneCapturePossible_ExpectAppropriateNumberOfPlies) {
  // Setup
  const Coordinate queen_location{3, 4};
  const Coordinate own_piece_location{1, 4};
  const Coordinate opponent_piece_location{3, 2};
  position_.board_.Set(queen_location, Queen::OfSide(Player::min));
  position_.board_.Set(own_piece_location, Rook::OfSide(Player::min));
  position_.board_.Set(opponent_piece_location, Rook::OfSide(Player::max));

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(queen_location)
          ->FindPlies(ToIdx(queen_location), position_)};

  // Expect
  EXPECT_EQ(returned_positions.size(), 23);
}

TEST_F(PiecePlies_Fixture,
       GivenKingOnEdgeWithOneCapturePossible_ExpectAppropriateNumberOfPlies) {
  // Setup
  const Coordinate king_location{0, 3};
  const Coordinate own_piece_location{1, 3};
  const Coordinate opponent_piece_location{0, 4};
  position_.board_.Set(king_location, King::OfSide(Player::min));
  position_.board_.Set(own_piece_location, Rook::OfSide(Player::min));
  position_.board_.Set(opponent_piece_location, Rook::OfSide(Player::max));

  // Call
  const std::vector<Position> returned_positions{
      position_.board_.Get(king_location)
          ->FindPlies(ToIdx(king_location), position_)};

  // Expect
  EXPECT_EQ(returned_positions.size(), 4);
}

}  // namespace
}  // namespace Chess
