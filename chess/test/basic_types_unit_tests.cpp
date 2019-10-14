#include <chess/basic_types.h>

#include <gtest/gtest.h>

namespace Chess {

namespace {

TEST(Code, StreamOutputOperator_MustNotThrow) {
  const Chess::Code a{Piece::K};
  const Chess::Code b{Piece::e};
  const Chess::Code c{1};
  const Chess::Code d{8};

  EXPECT_NO_THROW(std::cout << a << '\n');
  EXPECT_NO_THROW(std::cout << b << '\n');
  EXPECT_NO_THROW(std::cout << c << '\n');
  EXPECT_NO_THROW(std::cout << d << '\n');
}

TEST(Piece, StreamOutputOperator_MustNotThrow) {
  EXPECT_NO_THROW(std::cout << Piece::B << '\n');
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
