#include <chess/state.h>

#include <gtest/gtest.h>
#include <algorithm>
#include <iostream>

namespace Chess {
namespace {

TEST(Board, DefaultConstructedBoardIsFilledWithEmptyPieces) {
  const Board board{};
  for (const auto& row : board.data_) {
    for (const auto piece : row) {
      EXPECT_EQ(Piece::e, piece);
    }
  }
}

TEST(Piece, StreamOutputOperator_MustNotThrow) {
  EXPECT_NO_THROW(std::cout << Piece::B << '\n');
}

TEST(Board, StreamOutputOperator_MustNotThrow) {
  const Board board{};
  EXPECT_NO_THROW(std::cout << board << '\n');
}

TEST(IsAPieceOfSide, WhenBlacksTurnAndBlackPiece_ExpectTrue) {
  EXPECT_TRUE(IsAPieceOfSide(Piece::p, GameTree::Player::min));
  EXPECT_TRUE(IsAPieceOfSide(Piece::k, GameTree::Player::min));
  EXPECT_TRUE(IsAPieceOfSide(Piece::b, GameTree::Player::min));
  EXPECT_TRUE(IsAPieceOfSide(Piece::r, GameTree::Player::min));
  EXPECT_TRUE(IsAPieceOfSide(Piece::q, GameTree::Player::min));
  EXPECT_TRUE(IsAPieceOfSide(Piece::k, GameTree::Player::min));
}

TEST(IsAPieceOfSide, WhenBlacksTurnAndNotBlackPiece_ExpectFalse) {
  EXPECT_FALSE(IsAPieceOfSide(Piece::P, GameTree::Player::min));
  EXPECT_FALSE(IsAPieceOfSide(Piece::K, GameTree::Player::min));
  EXPECT_FALSE(IsAPieceOfSide(Piece::B, GameTree::Player::min));
  EXPECT_FALSE(IsAPieceOfSide(Piece::R, GameTree::Player::min));
  EXPECT_FALSE(IsAPieceOfSide(Piece::Q, GameTree::Player::min));
  EXPECT_FALSE(IsAPieceOfSide(Piece::K, GameTree::Player::min));
  EXPECT_FALSE(IsAPieceOfSide(Piece::e, GameTree::Player::min));
}

TEST(IsAPieceOfSide, WhenWhitesTurnAndWhitePiece_ExpectTrue) {
  EXPECT_TRUE(IsAPieceOfSide(Piece::P, GameTree::Player::max));
  EXPECT_TRUE(IsAPieceOfSide(Piece::K, GameTree::Player::max));
  EXPECT_TRUE(IsAPieceOfSide(Piece::B, GameTree::Player::max));
  EXPECT_TRUE(IsAPieceOfSide(Piece::R, GameTree::Player::max));
  EXPECT_TRUE(IsAPieceOfSide(Piece::Q, GameTree::Player::max));
  EXPECT_TRUE(IsAPieceOfSide(Piece::K, GameTree::Player::max));
}

TEST(IsAPieceOfSide, WhenWhitesTurnAndNotWhitePiece_ExpectFalse) {
  EXPECT_FALSE(IsAPieceOfSide(Piece::p, GameTree::Player::max));
  EXPECT_FALSE(IsAPieceOfSide(Piece::k, GameTree::Player::max));
  EXPECT_FALSE(IsAPieceOfSide(Piece::b, GameTree::Player::max));
  EXPECT_FALSE(IsAPieceOfSide(Piece::r, GameTree::Player::max));
  EXPECT_FALSE(IsAPieceOfSide(Piece::q, GameTree::Player::max));
  EXPECT_FALSE(IsAPieceOfSide(Piece::k, GameTree::Player::max));
  EXPECT_FALSE(IsAPieceOfSide(Piece::e, GameTree::Player::max));
}

}  // namespace
}  // namespace Chess
