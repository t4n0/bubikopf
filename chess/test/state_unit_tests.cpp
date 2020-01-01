#include "chess/pieces.h"
#include "chess/state.h"

#include <gtest/gtest.h>
#include <algorithm>
#include <iostream>
#include <typeinfo>

namespace Chess {
namespace {

TEST(Board, GivenDefaultConstructed_ExpectBoardIsFilledWithEmptySquares) {
  const Board board{};
  for (const auto& square : board.squares_) {
    EXPECT_TRUE(square->IsEmpty());
  }
}

TEST(Board, Set_ExpectBoundsCheck) {
  Board board{};
  EXPECT_NO_THROW(board.Get({3, 3}));
  EXPECT_THROW(board.Get({-1, 3}), std::out_of_range);
  EXPECT_THROW(board.Get({8, 3}), std::out_of_range);
  EXPECT_THROW(board.Get({3, -1}), std::out_of_range);
  EXPECT_THROW(board.Get({3, 8}), std::out_of_range);

  EXPECT_NO_THROW(board.Set({3, 3}, KnightPtr{}));
  EXPECT_THROW(board.Set({-1, 3}, KnightPtr{}), std::out_of_range);
  EXPECT_THROW(board.Set({8, 3}, KnightPtr{}), std::out_of_range);
  EXPECT_THROW(board.Set({3, -1}, KnightPtr{}), std::out_of_range);
  EXPECT_THROW(board.Set({3, 8}, KnightPtr{}), std::out_of_range);
}

TEST(Piece, StreamOutputOperator_MustNotThrow) {
  EXPECT_NO_THROW(std::cout << Bishop{} << '\n');
}

TEST(Board, StreamOutputOperator_MustNotThrow) {
  const Board board{};
  EXPECT_NO_THROW(std::cout << board << '\n');
}

TEST(IsAPieceOfSide, GivenBlacksTurn_ExpectBlackPiecesTrueRestFalse) {
  EXPECT_TRUE(King{AlphaBeta::Player::min}.IsOfSide(AlphaBeta::Player::min));
  EXPECT_TRUE(Queen{AlphaBeta::Player::min}.IsOfSide(AlphaBeta::Player::min));
  EXPECT_TRUE(Rook{AlphaBeta::Player::min}.IsOfSide(AlphaBeta::Player::min));
  EXPECT_TRUE(Bishop{AlphaBeta::Player::min}.IsOfSide(AlphaBeta::Player::min));
  EXPECT_TRUE(Knight{AlphaBeta::Player::min}.IsOfSide(AlphaBeta::Player::min));
  EXPECT_TRUE(Pawn{AlphaBeta::Player::min}.IsOfSide(AlphaBeta::Player::min));
  EXPECT_FALSE(Empty{}.IsOfSide(AlphaBeta::Player::min));
  EXPECT_FALSE(King{AlphaBeta::Player::max}.IsOfSide(AlphaBeta::Player::min));
  EXPECT_FALSE(Queen{AlphaBeta::Player::max}.IsOfSide(AlphaBeta::Player::min));
  EXPECT_FALSE(Rook{AlphaBeta::Player::max}.IsOfSide(AlphaBeta::Player::min));
  EXPECT_FALSE(Bishop{AlphaBeta::Player::max}.IsOfSide(AlphaBeta::Player::min));
  EXPECT_FALSE(Knight{AlphaBeta::Player::max}.IsOfSide(AlphaBeta::Player::min));
  EXPECT_FALSE(Pawn{AlphaBeta::Player::max}.IsOfSide(AlphaBeta::Player::min));
}

TEST(IsAPieceOfSide, GivenWhitesTurn_ExpectWhitePiecesTrueRestFalse) {
  EXPECT_FALSE(King{AlphaBeta::Player::min}.IsOfSide(AlphaBeta::Player::max));
  EXPECT_FALSE(Queen{AlphaBeta::Player::min}.IsOfSide(AlphaBeta::Player::max));
  EXPECT_FALSE(Rook{AlphaBeta::Player::min}.IsOfSide(AlphaBeta::Player::max));
  EXPECT_FALSE(Bishop{AlphaBeta::Player::min}.IsOfSide(AlphaBeta::Player::max));
  EXPECT_FALSE(Knight{AlphaBeta::Player::min}.IsOfSide(AlphaBeta::Player::max));
  EXPECT_FALSE(Pawn{AlphaBeta::Player::min}.IsOfSide(AlphaBeta::Player::max));
  EXPECT_FALSE(Empty{}.IsOfSide(AlphaBeta::Player::max));
  EXPECT_TRUE(King{AlphaBeta::Player::max}.IsOfSide(AlphaBeta::Player::max));
  EXPECT_TRUE(Queen{AlphaBeta::Player::max}.IsOfSide(AlphaBeta::Player::max));
  EXPECT_TRUE(Rook{AlphaBeta::Player::max}.IsOfSide(AlphaBeta::Player::max));
  EXPECT_TRUE(Bishop{AlphaBeta::Player::max}.IsOfSide(AlphaBeta::Player::max));
  EXPECT_TRUE(Knight{AlphaBeta::Player::max}.IsOfSide(AlphaBeta::Player::max));
  EXPECT_TRUE(Pawn{AlphaBeta::Player::max}.IsOfSide(AlphaBeta::Player::max));
}

}  // namespace
}  // namespace Chess
