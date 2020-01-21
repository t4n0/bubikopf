#include "board/state.h"

#include "board/utilities.h"

#include <gtest/gtest.h>

namespace Chess {
namespace {

TEST(Board, Set_ExpectBoundsCheck) {
  SquareBehaviourPool pool;
  Board board{};
  EXPECT_NO_THROW(board.Get({3, 3}));
  EXPECT_THROW(board.Get({-1, 3}), std::out_of_range);
  EXPECT_THROW(board.Get({8, 3}), std::out_of_range);
  EXPECT_THROW(board.Get({3, -1}), std::out_of_range);
  EXPECT_THROW(board.Get({3, 8}), std::out_of_range);

  constexpr Player player{Player::max};
  EXPECT_NO_THROW(board.Set({3, 3}, pool.GetKnight(player)));
  EXPECT_THROW(board.Set({-1, 3}, pool.GetKnight(player)), std::out_of_range);
  EXPECT_THROW(board.Set({8, 3}, pool.GetKnight(player)), std::out_of_range);
  EXPECT_THROW(board.Set({3, -1}, pool.GetKnight(player)), std::out_of_range);
  EXPECT_THROW(board.Set({3, 8}, pool.GetKnight(player)), std::out_of_range);
}

TEST(Piece, StreamOutputOperator_MustNotThrow) {
  EXPECT_NO_THROW(std::cout << Bishop{Player::max} << '\n');
}

TEST(Board, StreamOutputOperator_MustNotThrow) {
  const State state{SetUpEmptyBoard()};
  EXPECT_NO_THROW(std::cout << state.board_ << '\n');
}

TEST(IsAPieceOfSide, GivenBlacksTurn_ExpectBlackPiecesTrueRestFalse) {
  EXPECT_TRUE(King{Player::min}.IsOfSide(Player::min));
  EXPECT_TRUE(Queen{Player::min}.IsOfSide(Player::min));
  EXPECT_TRUE(Rook{Player::min}.IsOfSide(Player::min));
  EXPECT_TRUE(Bishop{Player::min}.IsOfSide(Player::min));
  EXPECT_TRUE(Knight{Player::min}.IsOfSide(Player::min));
  EXPECT_TRUE(Pawn{Player::min}.IsOfSide(Player::min));
  EXPECT_FALSE(Empty{}.IsOfSide(Player::min));
  EXPECT_FALSE(King{Player::max}.IsOfSide(Player::min));
  EXPECT_FALSE(Queen{Player::max}.IsOfSide(Player::min));
  EXPECT_FALSE(Rook{Player::max}.IsOfSide(Player::min));
  EXPECT_FALSE(Bishop{Player::max}.IsOfSide(Player::min));
  EXPECT_FALSE(Knight{Player::max}.IsOfSide(Player::min));
  EXPECT_FALSE(Pawn{Player::max}.IsOfSide(Player::min));
}

TEST(IsAPieceOfSide, GivenWhitesTurn_ExpectWhitePiecesTrueRestFalse) {
  EXPECT_FALSE(King{Player::min}.IsOfSide(Player::max));
  EXPECT_FALSE(Queen{Player::min}.IsOfSide(Player::max));
  EXPECT_FALSE(Rook{Player::min}.IsOfSide(Player::max));
  EXPECT_FALSE(Bishop{Player::min}.IsOfSide(Player::max));
  EXPECT_FALSE(Knight{Player::min}.IsOfSide(Player::max));
  EXPECT_FALSE(Pawn{Player::min}.IsOfSide(Player::max));
  EXPECT_FALSE(Empty{}.IsOfSide(Player::max));
  EXPECT_TRUE(King{Player::max}.IsOfSide(Player::max));
  EXPECT_TRUE(Queen{Player::max}.IsOfSide(Player::max));
  EXPECT_TRUE(Rook{Player::max}.IsOfSide(Player::max));
  EXPECT_TRUE(Bishop{Player::max}.IsOfSide(Player::max));
  EXPECT_TRUE(Knight{Player::max}.IsOfSide(Player::max));
  EXPECT_TRUE(Pawn{Player::max}.IsOfSide(Player::max));
}

TEST(ToIdxAndToCoor, GivenAConversionThereAndBack_ExpectUnalteredValue) {
  const Coordinate coordinate{4, 3};
  const std::size_t index{28};

  EXPECT_TRUE(index == ToIdx(coordinate));
  EXPECT_TRUE(coordinate == ToCoor(index));
}

TEST(SetGetCastling, ExpectRespectiveCastlingRights) {
  // Setup
  State state{SetUpEmptyBoard()};
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
  State state{SetUpEmptyBoard()};
  EXPECT_EQ(sizeof(State), 544);
  // consisting of
  EXPECT_EQ(sizeof(Board), 512);
  EXPECT_EQ(sizeof(SquareBehaviourPool*), 8);
  EXPECT_EQ(sizeof(int), 4);  // x2 = 8
  EXPECT_EQ(sizeof(Player), 4);
  EXPECT_EQ(sizeof(std::optional<Coordinate>), 3);
  EXPECT_EQ(sizeof(Castling), 2);  // x2 = 4
  // + 4 bytes of padding
}

}  // namespace
}  // namespace Chess
