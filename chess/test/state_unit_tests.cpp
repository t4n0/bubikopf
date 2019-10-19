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

/*class ChessStateTest : public ::testing::WithParamInterface<Coordinate> {};

TEST_P(ChessStateTest, GetPieceFrom_EmptyBoard) {
  EXPECT_EQ(state_.GetPieceFrom(GetParam().file_, GetParam().rank_), Piece::e);
}

TEST_P(ChessStateTest, GetPieceFrom_FileAWhiteRooksOnly) {
  const Rank file_of_rooks{'a'};

  for (Chess::CodedRank& rank : state_.board_) {
    rank.resize(2);
    rank.at(0) = Piece::R;
    rank.at(1) = 7;
  }

  if (GetParam().file_ == file_of_rooks) {
    EXPECT_EQ(state_.GetPieceFrom(GetParam().file_, GetParam().rank_),
              Piece::R);
  } else {
    EXPECT_EQ(state_.GetPieceFrom(GetParam().file_, GetParam().rank_),
              Piece::e);
  }
}

TEST_P(ChessStateTest, GetPieceFrom_Rank6BlackKnightsOnly) {
  const Rank rank_of_knights{6};

  state_.board_.at(MapToRow[rank_of_knights]).resize(8);
  for (std::size_t idx{0}; idx < 8; ++idx) {
    state_.board_.at(MapToRow[rank_of_knights]).at(idx) = Piece::n;
  }

  if (GetParam().rank_ == rank_of_knights) {
    EXPECT_EQ(state_.GetPieceFrom(GetParam().file_, GetParam().rank_),
              Piece::n);
  } else {
    EXPECT_EQ(state_.GetPieceFrom(GetParam().file_, GetParam().rank_),
              Piece::e);
  }
}

TEST_P(ChessStateTest, GetPieceFrom_BlackKingD2andWhiteKingE5) {
  const File file_black_king{'d'};
  const Rank rank_black_king{2};
  const File file_white_king{'e'};
  const Rank rank_white_king{5};

  state_.board_.at(MapToRow[rank_black_king]).at(0) = 3;
  state_.board_.at(MapToRow[rank_black_king]).emplace_back(Piece::k);
  state_.board_.at(MapToRow[rank_black_king]).emplace_back(4);
  state_.board_.at(MapToRow[rank_white_king]).at(0) = 4;
  state_.board_.at(MapToRow[rank_white_king]).emplace_back(Piece::K);
  state_.board_.at(MapToRow[rank_white_king]).emplace_back(3);

  if (GetParam().file_ == file_black_king &&
      GetParam().rank_ == rank_black_king) {
    EXPECT_EQ(state_.GetPieceFrom(GetParam().file_, GetParam().rank_),
              Piece::k);
  } else if (GetParam().file_ == file_white_king &&
             GetParam().rank_ == rank_white_king) {
    EXPECT_EQ(state_.GetPieceFrom(GetParam().file_, GetParam().rank_),
              Piece::K);
  } else {
    EXPECT_EQ(state_.GetPieceFrom(GetParam().file_, GetParam().rank_),
              Piece::e);
  }
}

TEST_F(ChessStateTest, GetPieceFrom_MustThrow) {
  EXPECT_THROW(state_.GetPieceFrom('A', 1), std::exception);
  EXPECT_THROW(state_.GetPieceFrom('a', 0), std::exception);
  EXPECT_THROW(state_.GetPieceFrom('a', 9), std::exception);

  state_.board_.at(7) =
      std::vector<Code>{1};  // file 1 is faulty (to few SquareCodes)
  EXPECT_THROW(state_.GetPieceFrom('h', 1), std::exception);

  state_.board_.at(7) =
      std::vector<Code>{8};  // reset so board is consistent at TearDown of test
}

TEST_P(ChessStateTest, SetSquareTo_BlackPawn) {
  const Coordinates coordinates{{'a', 1}, {'b', 2}, {'c', 3}, {'d', 4},
                                {'e', 5}, {'f', 6}, {'g', 7}, {'h', 8}};

  for (const Coordinate& coordinate : coordinates) {
    state_.SetSquareTo(coordinate.file_, coordinate.rank_, Piece::p);
  }

  if (std::any_of(
          coordinates.begin(), coordinates.end(),
          [this](const Coordinate& elem) { return elem == GetParam(); })) {
    EXPECT_EQ(state_.GetPieceFrom(GetParam().file_, GetParam().rank_),
              Piece::p);
  } else {
    EXPECT_EQ(state_.GetPieceFrom(GetParam().file_, GetParam().rank_),
              Piece::e);
  }
}

TEST_P(ChessStateTest, SetSquareTo_ExistingPieceIsSubstituted) {
  const Coordinates coordinates{{'a', 1}, {'b', 2}, {'c', 3}, {'d', 4},
                                {'e', 5}, {'f', 6}, {'g', 7}, {'h', 8}};

  for (const Coordinate& coordinate : coordinates) {
    state_.SetSquareTo(coordinate.file_, coordinate.rank_, Piece::p);
    state_.SetSquareTo(coordinate.file_, coordinate.rank_, Piece::N);
  }

  if (std::any_of(
          coordinates.begin(), coordinates.end(),
          [this](const Coordinate& elem) { return elem == GetParam(); })) {
    EXPECT_EQ(state_.GetPieceFrom(GetParam().file_, GetParam().rank_),
              Piece::N);
  } else {
    EXPECT_EQ(state_.GetPieceFrom(GetParam().file_, GetParam().rank_),
              Piece::e);
  }
}

TEST_P(ChessStateTest, SetSquareTo_OccupiedSquareBecomesEmpty) {
  const Coordinates coordinates{{'a', 1}, {'b', 2}, {'c', 3}, {'d', 4},
                                {'e', 5}, {'f', 6}, {'g', 7}, {'h', 8}};

  for (const Coordinate& coordinate : coordinates) {
    state_.SetSquareTo(coordinate.file_, coordinate.rank_, Piece::K);
    state_.SetSquareTo(coordinate.file_, coordinate.rank_, Piece::e);
  }

  EXPECT_EQ(state_.GetPieceFrom(GetParam().file_, GetParam().rank_), Piece::e);
}

TEST_P(ChessStateTest, SetSquareTo_TwoAdjacentPieces) {
  const Coordinates coordinates_black_pawn{
      {'a', 1}, {'b', 2}, {'c', 3}, {'d', 4}, {'e', 5}, {'f', 6}, {'g', 7}};
  const Coordinates coordinates_white_pawn{
      {'b', 1}, {'c', 2}, {'d', 3}, {'e', 4}, {'f', 5}, {'g', 6}, {'h', 7}};

  for (const Coordinate& coordinate : coordinates_black_pawn) {
    state_.SetSquareTo(coordinate.file_, coordinate.rank_, Piece::p);
  }
  for (const Coordinate& coordinate : coordinates_white_pawn) {
    state_.SetSquareTo(coordinate.file_, coordinate.rank_, Piece::P);
  }

  if (std::any_of(
          coordinates_black_pawn.begin(), coordinates_black_pawn.end(),
          [this](const Coordinate& elem) { return elem == GetParam(); })) {
    EXPECT_EQ(state_.GetPieceFrom(GetParam().file_, GetParam().rank_),
              Piece::p);
  } else if (std::any_of(coordinates_white_pawn.begin(),
                         coordinates_white_pawn.end(),
                         [this](const Coordinate& elem) {
                           return elem == GetParam();
                         })) {
    EXPECT_EQ(state_.GetPieceFrom(GetParam().file_, GetParam().rank_),
              Piece::P);
  } else {
    EXPECT_EQ(state_.GetPieceFrom(GetParam().file_, GetParam().rank_),
              Piece::e);
  }
}

TEST_P(ChessStateTest, SetSquareTo_TwoPiecesDifferentGaps) {
  const Coordinates coordinates_black_pawn{
      {'h', 1}, {'h', 2}, {'h', 3}, {'h', 4}, {'h', 5}, {'h', 6}, {'h', 7}};
  const Coordinates coordinates_white_pawn{
      {'a', 1}, {'b', 2}, {'c', 3}, {'d', 4}, {'e', 5}, {'f', 6}, {'g', 7}};

  for (const Coordinate& coordinate : coordinates_black_pawn) {
    state_.SetSquareTo(coordinate.file_, coordinate.rank_, Piece::p);
  }
  for (const Coordinate& coordinate : coordinates_white_pawn) {
    state_.SetSquareTo(coordinate.file_, coordinate.rank_, Piece::P);
  }

  if (std::any_of(
          coordinates_black_pawn.begin(), coordinates_black_pawn.end(),
          [this](const Coordinate& elem) { return elem == GetParam(); })) {
    EXPECT_EQ(state_.GetPieceFrom(GetParam().file_, GetParam().rank_),
              Piece::p);
  } else if (std::any_of(coordinates_white_pawn.begin(),
                         coordinates_white_pawn.end(),
                         [this](const Coordinate& elem) {
                           return elem == GetParam();
                         })) {
    EXPECT_EQ(state_.GetPieceFrom(GetParam().file_, GetParam().rank_),
              Piece::P);
  } else {
    EXPECT_EQ(state_.GetPieceFrom(GetParam().file_, GetParam().rank_),
              Piece::e);
  }
}

TEST_F(ChessStateTest, CodedRank_StreamOutputOperator_MustNotThrow) {
  EXPECT_NO_THROW(std::cout << state_.board_.at(0) << '\n');
}

TEST_F(ChessStateTest, BoardContainer_StreamOutputOperator_MustNotThrow) {
  state_.board_.at(0).resize(2);
  state_.board_.at(0).at(0) = 7;
  state_.board_.at(0).at(1) = Piece::K;

  EXPECT_NO_THROW(std::cout << state_ << '\n');
}

const Coordinates entire_board{
    {'a', 1}, {'a', 2}, {'a', 3}, {'a', 4}, {'a', 5}, {'a', 6}, {'a', 7},
    {'a', 8}, {'b', 1}, {'b', 2}, {'b', 3}, {'b', 4}, {'b', 5}, {'b', 6},
    {'b', 7}, {'b', 8}, {'c', 1}, {'c', 2}, {'c', 3}, {'c', 4}, {'c', 5},
    {'c', 6}, {'c', 7}, {'c', 8}, {'d', 1}, {'d', 2}, {'d', 3}, {'d', 4},
    {'d', 5}, {'d', 6}, {'d', 7}, {'d', 8}, {'e', 1}, {'e', 2}, {'e', 3},
    {'e', 4}, {'e', 5}, {'e', 6}, {'e', 7}, {'e', 8}, {'f', 1}, {'f', 2},
    {'f', 3}, {'f', 4}, {'f', 5}, {'f', 6}, {'f', 7}, {'f', 8}, {'g', 1},
    {'g', 2}, {'g', 3}, {'g', 4}, {'g', 5}, {'g', 6}, {'g', 7}, {'g', 8},
    {'h', 1}, {'h', 2}, {'h', 3}, {'h', 4}, {'h', 5}, {'h', 6}, {'h', 7},
    {'h', 8},
};

INSTANTIATE_TEST_CASE_P(EntireBoard, ChessStateTest,
                        ::testing::ValuesIn(entire_board));*/

}  // namespace
}  // namespace Chess
