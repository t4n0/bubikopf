#include "pieces.h"

#include <array>

namespace Chess {

Piece::~Piece() {}

SquareId Empty::GetId() const { return SquareId::Empty; }
SquareId Pawn::GetId() const {
  return owner_ == Player::min ? SquareId::BlackPawn : SquareId::WhitePawn;
}
SquareId Knight::GetId() const {
  return owner_ == Player::min ? SquareId::BlackKnight : SquareId::WhiteKnight;
}
SquareId Bishop::GetId() const {
  return owner_ == Player::min ? SquareId::BlackBishop : SquareId::WhiteBishop;
}
SquareId Rook::GetId() const {
  return owner_ == Player::min ? SquareId::BlackRook : SquareId::WhiteRook;
}
SquareId Queen::GetId() const {
  return owner_ == Player::min ? SquareId::BlackQueen : SquareId::WhiteQueen;
}
SquareId King::GetId() const {
  return owner_ == Player::min ? SquareId::BlackKing : SquareId::WhiteKing;
}

float Empty::GetValue() const { return 0.0F; }
float Pawn::GetValue() const { return owner_ == Player::max ? 1.0F : -1.0F; }
float Knight::GetValue() const { return owner_ == Player::max ? 3.0F : -3.0F; }
float Bishop::GetValue() const { return owner_ == Player::max ? 3.0F : -3.0F; }
float Rook::GetValue() const { return owner_ == Player::max ? 5.0F : -5.0F; }
float Queen::GetValue() const { return owner_ == Player::max ? 9.0F : -9.0F; }
float King::GetValue() const {
  return owner_ == Player::max ? 100.0F : -100.0F;
}

bool Empty::IsOfSide(const Player& player) const {
  std::ignore = player;
  return false;
}
bool Pawn::IsOfSide(const Player& side) const { return owner_ == side; }
bool Knight::IsOfSide(const Player& side) const { return owner_ == side; }
bool Bishop::IsOfSide(const Player& side) const { return owner_ == side; }
bool Rook::IsOfSide(const Player& side) const { return owner_ == side; }
bool Queen::IsOfSide(const Player& side) const { return owner_ == side; }
bool King::IsOfSide(const Player& side) const { return owner_ == side; }

bool Empty::IsEmpty() const { return true; }
bool Pawn::IsEmpty() const { return false; }
bool Knight::IsEmpty() const { return false; }
bool Bishop::IsEmpty() const { return false; }
bool Rook::IsEmpty() const { return false; }
bool Queen::IsEmpty() const { return false; }
bool King::IsEmpty() const { return false; }

std::vector<Position> Empty::FindPlies(const std::size_t /*unused*/,
                                       const Position& /*unused*/) const {
  return {};
}

Coordinate GetSingleStepFor(const Player player) {
  constexpr Coordinate black_advance{0, 1};
  constexpr Coordinate white_advance{0, -1};
  return player == Player::max ? white_advance : black_advance;
}

int8_t GetDoubleStepStartRowFor(const Player player) {
  constexpr int8_t black_start_row = 1;
  constexpr int8_t white_start_row = 6;
  return player == Player::max ? white_start_row : black_start_row;
}

int8_t GetPromotionRowFor(const Player player) {
  constexpr int8_t black_start_row = 7;
  constexpr int8_t white_start_row = 0;
  return player == Player::max ? white_start_row : black_start_row;
}

std::array<ISquarePtr, 4> GetPromotionOptionsFor(const Player player) {
  std::array<ISquarePtr, 4> promotion_options{};
  promotion_options.at(0) = Queen::OfSide(player);
  promotion_options.at(1) = Rook::OfSide(player);
  promotion_options.at(2) = Knight::OfSide(player);
  promotion_options.at(3) = Bishop::OfSide(player);
  return promotion_options;
}

Coordinate GetDoubleStepFor(const Player player) {
  constexpr Coordinate black_advance{0, 2};
  constexpr Coordinate white_advance{0, -2};
  return player == Player::max ? white_advance : black_advance;
}

std::array<Coordinate, 2> GetCaptureBehaviourFor(const Player player) {
  constexpr std::array<Coordinate, 2> black_captures{Coordinate{1, 1},
                                                     Coordinate{-1, 1}};
  constexpr std::array<Coordinate, 2> white_captures{Coordinate{1, -1},
                                                     Coordinate{-1, -1}};
  return player == Player::max ? white_captures : black_captures;
}

Position BasePositionAfterPly(const Position& position) {
  Position new_position = position;
  new_position.plies_++;
  new_position.en_passant_.reset();

  return new_position;
}

Position BasePositionAfterCaptureOrPawnPly(const Position& position) {
  Position new_position = BasePositionAfterPly(position);
  new_position.static_plies_ = 0;

  return new_position;
}

Position BasePositionAfterPiecePly(const Position& position) {
  Position new_position = BasePositionAfterPly(position);
  new_position.static_plies_++;

  return new_position;
}

std::vector<Position> Pawn::FindPlies(const std::size_t idx,
                                      const Position& position) const {
  std::vector<Position> new_plies{};

  if (IsOfSide(position.GetTurn())) {
    const Coordinate location = ToCoor(idx);

    const Coordinate single_step_target = location + GetSingleStepFor(owner_);
    if (position.board_.Get(single_step_target)->IsEmpty()) {
      if (single_step_target.row != GetPromotionRowFor(owner_)) {
        Position new_ply = BasePositionAfterCaptureOrPawnPly(position);
        new_ply.board_.SwapSquares(idx, ToIdx(single_step_target));
        new_plies.emplace_back(std::move(new_ply));
      } else {
        std::array<ISquarePtr, 4> promotion_options =
            GetPromotionOptionsFor(owner_);
        for (auto& promotion_option : promotion_options) {
          Position new_ply = BasePositionAfterCaptureOrPawnPly(position);
          new_ply.board_.Set(idx, Empty::Make());
          new_ply.board_.Set(single_step_target, promotion_option);
          new_plies.emplace_back(std::move(new_ply));
        }
      }
    }

    const Coordinate double_step_target = location + GetDoubleStepFor(owner_);
    if (location.row == GetDoubleStepStartRowFor(owner_) &&
        position.board_.Get(single_step_target)->IsEmpty() &&
        position.board_.Get(double_step_target)->IsEmpty()) {
      Position new_ply = BasePositionAfterCaptureOrPawnPly(position);
      new_ply.board_.SwapSquares(idx, ToIdx(double_step_target));
      new_ply.en_passant_ = single_step_target;
      new_plies.emplace_back(std::move(new_ply));
    }

    std::array<Coordinate, 2> capture_targets = GetCaptureBehaviourFor(owner_);
    for (auto& capture_target : capture_targets) {
      capture_target += location;
      if (IsOnTheBoard(capture_target)) {
        if (position.board_.Get(capture_target)->IsOfSide(!owner_)) {
          Position new_ply = BasePositionAfterCaptureOrPawnPly(position);
          new_ply.board_.SwapSquares(idx, ToIdx(capture_target));
          new_ply.board_.Set(idx, Empty::Make());
          new_plies.emplace_back(std::move(new_ply));
        } else if (position.en_passant_ &&
                   position.en_passant_.value() == capture_target) {
          Position new_ply = BasePositionAfterCaptureOrPawnPly(position);
          new_ply.board_.SwapSquares(idx, ToIdx(capture_target));
          new_ply.board_.Set(
              ToIdx(position.en_passant_.value() + GetSingleStepFor(!owner_)),
              Empty::Make());
          new_plies.emplace_back(std::move(new_ply));
        }
      }
    }
  }

  return new_plies;
}

Position MakePieceAdvance(const Position& position, const std::size_t location,
                          const std::size_t target) {
  Position new_ply = BasePositionAfterPiecePly(position);
  new_ply.board_.SwapSquares(location, target);
  return new_ply;
}

Position MakePieceCapture(const Position& position, const std::size_t location,
                          const std::size_t target) {
  Position new_ply = BasePositionAfterCaptureOrPawnPly(position);
  new_ply.board_.SwapSquares(location, target);
  new_ply.board_.Set(location, Empty::Make());
  return new_ply;
}

std::vector<Position> FindStraightLinePlies(
    const Position& position, const std::size_t location, const Player owner,
    const std::vector<Coordinate>& directions) {
  std::vector<Position> new_plies{};

  if (owner == position.GetTurn()) {
    for (const Coordinate& direction : directions) {
      Coordinate target = ToCoor(location);
      target += direction;
      while (IsOnTheBoard(target) &&
             !position.board_.Get(target)->IsOfSide(owner)) {
        if (position.board_.Get(target)->IsEmpty()) {
          new_plies.emplace_back(
              MakePieceAdvance(position, location, ToIdx(target)));
        } else if (!position.board_.Get(target)->IsOfSide(owner)) {
          new_plies.emplace_back(
              MakePieceCapture(position, location, ToIdx(target)));
          break;
        }
        target += direction;
      }
    }
  }

  return new_plies;
}

std::vector<Position> Bishop::FindPlies(const std::size_t idx,
                                        const Position& position) const {
  const std::vector<Coordinate> bishop_directions{
      {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}}};
  return FindStraightLinePlies(position, idx, owner_, bishop_directions);
}

std::vector<Position> Rook::FindPlies(const std::size_t idx,
                                      const Position& position) const {
  const std::vector<Coordinate> rook_directions{
      {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};
  return FindStraightLinePlies(position, idx, owner_, rook_directions);
}

std::vector<Position> Queen::FindPlies(const std::size_t idx,
                                       const Position& position) const {
  const std::vector<Coordinate> queen_directions{
      {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}, {1, 0}, {-1, 0}, {0, 1}, {0, -1}}};
  return FindStraightLinePlies(position, idx, owner_, queen_directions);
}

std::vector<Position> FindJumpStylePlies(const Position& position,
                                         const std::size_t location,
                                         const Player owner,
                                         const std::vector<Coordinate>& jumps) {
  std::vector<Position> new_plies{};

  if (owner == position.GetTurn()) {
    for (const Coordinate& jump : jumps) {
      const Coordinate target = ToCoor(location) + jump;
      if (IsOnTheBoard(target)) {
        if (position.board_.Get(target)->IsEmpty()) {
          new_plies.emplace_back(
              MakePieceAdvance(position, location, ToIdx(target)));
        } else if (!position.board_.Get(target)->IsOfSide(owner)) {
          new_plies.emplace_back(
              MakePieceCapture(position, location, ToIdx(target)));
        }
      }
    }
  }

  return new_plies;
}

std::vector<Position> Knight::FindPlies(const std::size_t idx,
                                        const Position& position) const {
  const std::vector<Coordinate> knight_jumps{
      {{1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1}, {2, 1}}};
  return FindJumpStylePlies(position, idx, owner_, knight_jumps);
}

std::vector<Position> King::FindPlies(const std::size_t idx,
                                      const Position& position) const {
  const std::vector<Coordinate> king_jumps{
      {{1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}}};
  return FindJumpStylePlies(position, idx, owner_, king_jumps);
}

std::ostream& Empty::print(std::ostream& stream) const {
  stream << "_ ";
  return stream;
}

std::ostream& Pawn::print(std::ostream& stream) const {
  if (owner_ == Player::max) {
    stream << "P ";
  } else {
    stream << "p ";
  }
  return stream;
}

std::ostream& Knight::print(std::ostream& stream) const {
  if (owner_ == Player::max) {
    stream << "N ";
  } else {
    stream << "n ";
  }
  return stream;
}

std::ostream& Bishop::print(std::ostream& stream) const {
  if (owner_ == Player::max) {
    stream << "B ";
  } else {
    stream << "b ";
  }
  return stream;
}

std::ostream& Rook::print(std::ostream& stream) const {
  if (owner_ == Player::max) {
    stream << "R ";
  } else {
    stream << "r ";
  }
  return stream;
}

std::ostream& Queen::print(std::ostream& stream) const {
  if (owner_ == Player::max) {
    stream << "Q ";
  } else {
    stream << "q ";
  }
  return stream;
}

std::ostream& King::print(std::ostream& stream) const {
  if (owner_ == Player::max) {
    stream << "K ";
  } else {
    stream << "k ";
  }
  return stream;
}

ISquarePtr Empty::Make() {
  static const Empty empty_square_{};
  return &empty_square_;
}
ISquarePtr Pawn::OfSide(const Player player) {
  static const Pawn black_pawn_{Player::min};
  static const Pawn white_pawn_{Player::max};
  return player == Player::max ? &white_pawn_ : &black_pawn_;
}
ISquarePtr Knight::OfSide(const Player player) {
  static const Knight black_knight_{Player::min};
  static const Knight white_knight_{Player::max};
  return player == Player::max ? &white_knight_ : &black_knight_;
}
ISquarePtr Bishop::OfSide(const Player player) {
  static const Bishop black_bishop_{Player::min};
  static const Bishop white_bishop_{Player::max};
  return player == Player::max ? &white_bishop_ : &black_bishop_;
}
ISquarePtr Rook::OfSide(const Player player) {
  static const Rook black_rook_{Player::min};
  static const Rook white_rook_{Player::max};
  return player == Player::max ? &white_rook_ : &black_rook_;
}
ISquarePtr Queen::OfSide(const Player player) {
  static const Queen black_queen_{Player::min};
  static const Queen white_queen_{Player::max};
  return player == Player::max ? &white_queen_ : &black_queen_;
}
ISquarePtr King::OfSide(const Player player) {
  static const King black_king_{Player::min};
  static const King white_king_{Player::max};
  return player == Player::max ? &white_king_ : &black_king_;
}

}  // namespace Chess
