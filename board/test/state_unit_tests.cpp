#include "board/state.h"

#include "board/piece.h"

#include <gtest/gtest.h>

namespace Chess {
namespace {

TEST(Board, Set_ExpectBoundsCheck) {
  Board board{};
  EXPECT_NO_THROW(board.Get({3, 3}));
  EXPECT_THROW(board.Get({-1, 3}), std::out_of_range);
  EXPECT_THROW(board.Get({8, 3}), std::out_of_range);
  EXPECT_THROW(board.Get({3, -1}), std::out_of_range);
  EXPECT_THROW(board.Get({3, 8}), std::out_of_range);

  constexpr Player player{Player::max};
  EXPECT_NO_THROW(board.Set({3, 3}, Knight::OfSide(player)));
  EXPECT_THROW(board.Set({-1, 3}, Knight::OfSide(player)), std::out_of_range);
  EXPECT_THROW(board.Set({8, 3}, Knight::OfSide(player)), std::out_of_range);
  EXPECT_THROW(board.Set({3, -1}, Knight::OfSide(player)), std::out_of_range);
  EXPECT_THROW(board.Set({3, 8}, Knight::OfSide(player)), std::out_of_range);
}

TEST(Piece, StreamOutputOperator_MustNotThrow) {
  EXPECT_NO_THROW(std::cout << Bishop::OfSide(Player::max) << '\n');
}

TEST(Board, StreamOutputOperator_MustNotThrow) {
  const State state{};
  EXPECT_NO_THROW(std::cout << state.board_ << '\n');
}

TEST(IsAPieceOfSide, GivenBlacksTurn_ExpectBlackPiecesTrueRestFalse) {
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

TEST(IsAPieceOfSide, GivenWhitesTurn_ExpectWhitePiecesTrueRestFalse) {
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

TEST(ToIdxAndToCoor, GivenAConversionThereAndBack_ExpectUnalteredValue) {
  const Coordinate coordinate{4, 3};
  const std::size_t index{28};

  EXPECT_TRUE(index == ToIdx(coordinate));
  EXPECT_TRUE(coordinate == ToCoor(index));
}

TEST(SetGetCastling, ExpectRespectiveCastlingRights) {
  // Setup
  State state{};
  const Castling expected_castling_white{true, false};
  const Castling expected_castling_black{false, true};

  // Call
  state.SetCastling(Player::max, expected_castling_white);
  state.SetCastling(Player::min, expected_castling_black);
  const Castling returned_castling_white = state.GetCastling(Player::max);
  const Castling returned_castling_black = state.GetCastling(Player::min);

  // Expect
  EXPECT_TRUE(expected_castling_white.queenside ==
              returned_castling_white.queenside);
  EXPECT_TRUE(expected_castling_white.kingside ==
              returned_castling_white.kingside);
  EXPECT_TRUE(expected_castling_black.queenside ==
              returned_castling_black.queenside);
  EXPECT_TRUE(expected_castling_black.kingside ==
              returned_castling_black.kingside);
}

TEST(State, MemoryFootprint) {
  EXPECT_EQ(sizeof(State), 536);
  // consisting of
  EXPECT_EQ(sizeof(Board), 512);
  EXPECT_EQ(sizeof(int), 4);  // x2 = 8
  EXPECT_EQ(sizeof(Player), 4);
  EXPECT_EQ(sizeof(std::optional<Coordinate>), 3);
  EXPECT_EQ(sizeof(Castling), 2);  // x2 = 4
  // + padding
}

TEST(ToIdx, GivenSquareNotation_ExpectIndex) {
  const std::size_t a8_idx = 0;
  const std::size_t h8_idx = 7;
  const std::size_t a1_idx = 56;
  const std::size_t h1_idx = 63;
  EXPECT_EQ(ToIdx('a', 1), a1_idx);
  EXPECT_EQ(ToIdx('a', 8), a8_idx);
  EXPECT_EQ(ToIdx('h', 1), h1_idx);
  EXPECT_EQ(ToIdx('h', 8), h8_idx);
}

TEST(ToIdx, GivenUciNotation_ExpectIndex) {
  const std::size_t a8_idx = 0;
  const std::size_t h8_idx = 7;
  const std::size_t a1_idx = 56;
  const std::size_t h1_idx = 63;
  EXPECT_EQ(ToIdx("a1"), a1_idx);
  EXPECT_EQ(ToIdx("a8"), a8_idx);
  EXPECT_EQ(ToIdx("h1"), h1_idx);
  EXPECT_EQ(ToIdx("h8"), h8_idx);
}

TEST(ToIdx, GivenIndex_ExpectUciNotation) {
  EXPECT_EQ(ToUciSquare(0), "a8");
  EXPECT_EQ(ToUciSquare(7), "h8");
  EXPECT_EQ(ToUciSquare(56), "a1");
  EXPECT_EQ(ToUciSquare(63), "h1");
}

TEST(State, GivenDefaultCtor_ExpectBoardIsFilledWithEmptySquares) {
  const State state{};
  for (const auto& square : state.board_.squares_) {
    EXPECT_TRUE(square->IsEmpty());
  }
}

}  // namespace
}  // namespace Chess
