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

}  // namespace

}  // namespace Chess
