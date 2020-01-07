#include "pieces.h"

#include <array>

namespace Chess {

Piece::~Piece() {}

Square Empty::GetId() const { return Square::Empty; }
Square Pawn::GetId() const {
  return owner_ == AlphaBeta::Player::min ? Square::BlackPawn
                                          : Square::WhitePawn;
}
Square Knight::GetId() const {
  return owner_ == AlphaBeta::Player::min ? Square::BlackKnight
                                          : Square::WhiteKnight;
}
Square Bishop::GetId() const {
  return owner_ == AlphaBeta::Player::min ? Square::BlackBishop
                                          : Square::WhiteBishop;
}
Square Rook::GetId() const {
  return owner_ == AlphaBeta::Player::min ? Square::BlackRook
                                          : Square::WhiteRook;
}
Square Queen::GetId() const {
  return owner_ == AlphaBeta::Player::min ? Square::BlackQueen
                                          : Square::WhiteQueen;
}
Square King::GetId() const {
  return owner_ == AlphaBeta::Player::min ? Square::BlackKing
                                          : Square::WhiteKing;
}

float Empty::GetValue() const { return 0.0F; }
float Pawn::GetValue() const {
  return owner_ == AlphaBeta::Player::max ? 1.0F : -1.0F;
}
float Knight::GetValue() const {
  return owner_ == AlphaBeta::Player::max ? 3.0F : -3.0F;
}
float Bishop::GetValue() const {
  return owner_ == AlphaBeta::Player::max ? 3.0F : -3.0F;
}
float Rook::GetValue() const {
  return owner_ == AlphaBeta::Player::max ? 5.0F : -5.0F;
}
float Queen::GetValue() const {
  return owner_ == AlphaBeta::Player::max ? 9.0F : -9.0F;
}
float King::GetValue() const {
  return owner_ == AlphaBeta::Player::max ? 100.0F : -100.0F;
}

bool Empty::IsOfSide(const AlphaBeta::Player& /*unused*/) const {
  return false;
}
bool Pawn::IsOfSide(const AlphaBeta::Player& side) const {
  return owner_ == side;
}
bool Knight::IsOfSide(const AlphaBeta::Player& side) const {
  return owner_ == side;
}
bool Bishop::IsOfSide(const AlphaBeta::Player& side) const {
  return owner_ == side;
}
bool Rook::IsOfSide(const AlphaBeta::Player& side) const {
  return owner_ == side;
}
bool Queen::IsOfSide(const AlphaBeta::Player& side) const {
  return owner_ == side;
}
bool King::IsOfSide(const AlphaBeta::Player& side) const {
  return owner_ == side;
}

bool Empty::IsEmpty() const { return true; }
bool Pawn::IsEmpty() const { return false; }
bool Knight::IsEmpty() const { return false; }
bool Bishop::IsEmpty() const { return false; }
bool Rook::IsEmpty() const { return false; }
bool Queen::IsEmpty() const { return false; }
bool King::IsEmpty() const { return false; }

std::vector<State> Empty::FindMoves(const std::size_t /*unused*/,
                                    const State& /*unused*/) const {
  return {};
}

Coordinate GetSingleStepFor(const AlphaBeta::Player player) {
  constexpr Coordinate black_advance{0, 1};
  constexpr Coordinate white_advance{0, -1};
  return player == AlphaBeta::Player::max ? white_advance : black_advance;
}

int8_t GetDoubleStepStartRowFor(const AlphaBeta::Player player) {
  constexpr int8_t black_start_row = 1;
  constexpr int8_t white_start_row = 6;
  return player == AlphaBeta::Player::max ? white_start_row : black_start_row;
}

int8_t GetPromotionRowFor(const AlphaBeta::Player player) {
  constexpr int8_t black_start_row = 7;
  constexpr int8_t white_start_row = 0;
  return player == AlphaBeta::Player::max ? white_start_row : black_start_row;
}

std::array<ISquarePtr, 4> GetPromotionOptionsFor(
    const AlphaBeta::Player player) {
  std::array<ISquarePtr, 4> promotion_options{};
  promotion_options.at(0) = std::make_unique<Queen>(player);
  promotion_options.at(1) = std::make_unique<Rook>(player);
  promotion_options.at(2) = std::make_unique<Knight>(player);
  promotion_options.at(3) = std::make_unique<Bishop>(player);
  return promotion_options;
}

Coordinate GetDoubleStepFor(const AlphaBeta::Player player) {
  constexpr Coordinate black_advance{0, 2};
  constexpr Coordinate white_advance{0, -2};
  return player == AlphaBeta::Player::max ? white_advance : black_advance;
}

std::array<Coordinate, 2> GetCaptureBehaviourFor(
    const AlphaBeta::Player player) {
  constexpr std::array<Coordinate, 2> black_captures{Coordinate{1, 1},
                                                     Coordinate{-1, 1}};
  constexpr std::array<Coordinate, 2> white_captures{Coordinate{1, -1},
                                                     Coordinate{-1, -1}};
  return player == AlphaBeta::Player::max ? white_captures : black_captures;
}

State BaseStateAfterMove(const State& state) {
  State new_state = state;
  new_state.plies_++;
  new_state.turn_ = state.turn_ == AlphaBeta::Player::max
                        ? AlphaBeta::Player::min
                        : AlphaBeta::Player::max;
  new_state.en_passant_.reset();

  return new_state;
}

State BaseStateAfterCaptureOrPawnMove(const State& state) {
  State new_state = BaseStateAfterMove(state);
  new_state.static_plies_ = 0;

  return new_state;
}

State BaseStateAfterPieceMove(const State& state) {
  State new_state = BaseStateAfterMove(state);
  new_state.static_plies_++;

  return new_state;
}

AlphaBeta::Player GetOtherPlayer(const AlphaBeta::Player player) {
  return player == AlphaBeta::Player::max ? AlphaBeta::Player::min
                                          : AlphaBeta::Player::max;
}

std::vector<State> Pawn::FindMoves(const std::size_t idx,
                                   const State& state) const {
  std::vector<State> new_moves{};

  if (IsOfSide(state.turn_)) {
    const Coordinate location = ToCoor(idx);

    const Coordinate single_step_target = location + GetSingleStepFor(owner_);
    if (state.board_.Get(single_step_target)->IsEmpty()) {
      if (single_step_target.row != GetPromotionRowFor(owner_)) {
        State new_move = BaseStateAfterCaptureOrPawnMove(state);
        new_move.board_.SwapSquares(idx, ToIdx(single_step_target));
        new_moves.emplace_back(std::move(new_move));
      } else {
        std::array<ISquarePtr, 4> promotion_options =
            GetPromotionOptionsFor(owner_);
        for (auto& promotion_option : promotion_options) {
          State new_move = BaseStateAfterCaptureOrPawnMove(state);
          new_move.board_.Set(idx, std::make_unique<Empty>());
          new_move.board_.Set(single_step_target, std::move(promotion_option));
          new_moves.emplace_back(std::move(new_move));
        }
      }
    }

    const Coordinate double_step_target = location + GetDoubleStepFor(owner_);
    if (location.row == GetDoubleStepStartRowFor(owner_) &&
        state.board_.Get(single_step_target)->IsEmpty() &&
        state.board_.Get(double_step_target)->IsEmpty()) {
      State new_move = BaseStateAfterCaptureOrPawnMove(state);
      new_move.board_.SwapSquares(idx, ToIdx(double_step_target));
      new_move.en_passant_ = single_step_target;
      new_moves.emplace_back(std::move(new_move));
    }

    std::array<Coordinate, 2> capture_targets = GetCaptureBehaviourFor(owner_);
    for (auto& capture_target : capture_targets) {
      capture_target += location;
      if (IsOnTheBoard(capture_target)) {
        if (state.board_.Get(capture_target)
                ->IsOfSide(GetOtherPlayer(owner_))) {
          State new_move = BaseStateAfterCaptureOrPawnMove(state);
          new_move.board_.SwapSquares(idx, ToIdx(capture_target));
          new_move.board_.Set(idx, std::make_unique<Empty>());
          new_moves.emplace_back(std::move(new_move));
        } else if (state.en_passant_ &&
                   state.en_passant_.value() == capture_target) {
          State new_move = BaseStateAfterCaptureOrPawnMove(state);
          new_move.board_.SwapSquares(idx, ToIdx(capture_target));
          new_move.board_.Set(ToIdx(state.en_passant_.value() +
                                    GetSingleStepFor(GetOtherPlayer(owner_))),
                              std::make_unique<Empty>());
          new_moves.emplace_back(std::move(new_move));
        }
      }
    }
  }

  return new_moves;
}

State MakePieceAdvance(const State& state, const std::size_t location,
                       const std::size_t target) {
  State new_move = BaseStateAfterPieceMove(state);
  new_move.board_.SwapSquares(location, target);
  return new_move;
}

State MakePieceCapture(const State& state, const std::size_t location,
                       const std::size_t target) {
  State new_move = BaseStateAfterCaptureOrPawnMove(state);
  new_move.board_.SwapSquares(location, target);
  new_move.board_.Set(location, std::make_unique<Empty>());
  return new_move;
}

std::vector<State> FindStraightLineMoves(
    const State& state, const std::size_t location,
    const AlphaBeta::Player owner, const std::vector<Coordinate>& directions) {
  std::vector<State> new_moves{};

  if (owner == state.turn_) {
    for (const Coordinate& direction : directions) {
      Coordinate target = ToCoor(location);
      target += direction;
      while (IsOnTheBoard(target) &&
             !state.board_.Get(target)->IsOfSide(owner)) {
        if (state.board_.Get(target)->IsEmpty()) {
          new_moves.emplace_back(
              MakePieceAdvance(state, location, ToIdx(target)));
        } else if (!state.board_.Get(target)->IsOfSide(owner)) {
          new_moves.emplace_back(
              MakePieceCapture(state, location, ToIdx(target)));
          break;
        }
        target += direction;
      }
    }
  }

  return new_moves;
}

std::vector<State> Bishop::FindMoves(const std::size_t idx,
                                     const State& state) const {
  const std::vector<Coordinate> bishop_directions{
      {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}}};
  return FindStraightLineMoves(state, idx, owner_, bishop_directions);
}

std::vector<State> Rook::FindMoves(const std::size_t idx,
                                   const State& state) const {
  const std::vector<Coordinate> rook_directions{
      {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};
  return FindStraightLineMoves(state, idx, owner_, rook_directions);
}

std::vector<State> Queen::FindMoves(const std::size_t idx,
                                    const State& state) const {
  const std::vector<Coordinate> queen_directions{
      {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}, {1, 0}, {-1, 0}, {0, 1}, {0, -1}}};
  return FindStraightLineMoves(state, idx, owner_, queen_directions);
}

std::vector<State> FindJumpStyleMoves(const State& state,
                                      const std::size_t location,
                                      const AlphaBeta::Player owner,
                                      const std::vector<Coordinate>& jumps) {
  std::vector<State> new_moves{};

  if (owner == state.turn_) {
    for (const Coordinate& jump : jumps) {
      const Coordinate target = ToCoor(location) + jump;
      if (IsOnTheBoard(target)) {
        if (state.board_.Get(target)->IsEmpty()) {
          new_moves.emplace_back(
              MakePieceAdvance(state, location, ToIdx(target)));
        } else if (!state.board_.Get(target)->IsOfSide(owner)) {
          new_moves.emplace_back(
              MakePieceCapture(state, location, ToIdx(target)));
        }
      }
    }
  }

  return new_moves;
}

std::vector<State> Knight::FindMoves(const std::size_t idx,
                                     const State& state) const {
  const std::vector<Coordinate> knight_jumps{
      {{1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1}, {2, 1}}};
  return FindJumpStyleMoves(state, idx, owner_, knight_jumps);
}

std::vector<State> King::FindMoves(const std::size_t idx,
                                   const State& state) const {
  const std::vector<Coordinate> king_jumps{
      {{1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}}};
  return FindJumpStyleMoves(state, idx, owner_, king_jumps);
}

std::ostream& Empty::print(std::ostream& stream) const {
  stream << "_ ";
  return stream;
}

std::ostream& Pawn::print(std::ostream& stream) const {
  if (owner_ == AlphaBeta::Player::max) {
    stream << "P ";
  } else {
    stream << "p ";
  }
  return stream;
}

std::ostream& Knight::print(std::ostream& stream) const {
  if (owner_ == AlphaBeta::Player::max) {
    stream << "N ";
  } else {
    stream << "n ";
  }
  return stream;
}

std::ostream& Bishop::print(std::ostream& stream) const {
  if (owner_ == AlphaBeta::Player::max) {
    stream << "B ";
  } else {
    stream << "b ";
  }
  return stream;
}

std::ostream& Rook::print(std::ostream& stream) const {
  if (owner_ == AlphaBeta::Player::max) {
    stream << "R ";
  } else {
    stream << "r ";
  }
  return stream;
}

std::ostream& Queen::print(std::ostream& stream) const {
  if (owner_ == AlphaBeta::Player::max) {
    stream << "Q ";
  } else {
    stream << "q ";
  }
  return stream;
}

std::ostream& King::print(std::ostream& stream) const {
  if (owner_ == AlphaBeta::Player::max) {
    stream << "K ";
  } else {
    stream << "k ";
  }
  return stream;
}

}  // namespace Chess
