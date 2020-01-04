#include "pieces.h"

namespace Chess {

float Empty::GetValue() const { return 0.0F; }
float Pawn::GetValue() const {
  return side_ == AlphaBeta::Player::max ? 1.0F : -1.0F;
}
float Knight::GetValue() const {
  return side_ == AlphaBeta::Player::max ? 3.0F : -3.0F;
}
float Bishop::GetValue() const {
  return side_ == AlphaBeta::Player::max ? 3.0F : -3.0F;
}
float Rook::GetValue() const {
  return side_ == AlphaBeta::Player::max ? 5.0F : -5.0F;
}
float Queen::GetValue() const {
  return side_ == AlphaBeta::Player::max ? 9.0F : -9.0F;
}
float King::GetValue() const {
  return side_ == AlphaBeta::Player::max ? 100.0F : -100.0F;
}

bool Empty::IsOfSide(const AlphaBeta::Player& /*unused*/) { return false; }
bool Pawn::IsOfSide(const AlphaBeta::Player& side) { return side_ == side; }
bool Knight::IsOfSide(const AlphaBeta::Player& side) { return side_ == side; }
bool Bishop::IsOfSide(const AlphaBeta::Player& side) { return side_ == side; }
bool Rook::IsOfSide(const AlphaBeta::Player& side) { return side_ == side; }
bool Queen::IsOfSide(const AlphaBeta::Player& side) { return side_ == side; }
bool King::IsOfSide(const AlphaBeta::Player& side) { return side_ == side; }

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
std::vector<State> Pawn::FindMoves(const std::size_t idx,
                                   const State& state) const {
  return {};
}
std::vector<State> Knight::FindMoves(const std::size_t idx,
                                     const State& state) const {
  return {};
}
std::vector<State> Bishop::FindMoves(const std::size_t idx,
                                     const State& state) const {
  return {};
}
std::vector<State> Rook::FindMoves(const std::size_t idx,
                                   const State& state) const {
  return {};
}
std::vector<State> Queen::FindMoves(const std::size_t idx,
                                    const State& state) const {
  return {};
}
std::vector<State> King::FindMoves(const std::size_t idx,
                                   const State& state) const {
  return {};
}

std::ostream& Empty::print(std::ostream& stream) const {
  stream << "_ ";
  return stream;
}

std::ostream& Pawn::print(std::ostream& stream) const {
  if (side_ == AlphaBeta::Player::max) {
    stream << "P ";
  } else {
    stream << "p ";
  }
  return stream;
}

std::ostream& Knight::print(std::ostream& stream) const {
  if (side_ == AlphaBeta::Player::max) {
    stream << "N ";
  } else {
    stream << "n ";
  }
  return stream;
}

std::ostream& Bishop::print(std::ostream& stream) const {
  if (side_ == AlphaBeta::Player::max) {
    stream << "B ";
  } else {
    stream << "b ";
  }
  return stream;
}

std::ostream& Rook::print(std::ostream& stream) const {
  if (side_ == AlphaBeta::Player::max) {
    stream << "R ";
  } else {
    stream << "r ";
  }
  return stream;
}

std::ostream& Queen::print(std::ostream& stream) const {
  if (side_ == AlphaBeta::Player::max) {
    stream << "Q ";
  } else {
    stream << "q ";
  }
  return stream;
}

std::ostream& King::print(std::ostream& stream) const {
  if (side_ == AlphaBeta::Player::max) {
    stream << "K ";
  } else {
    stream << "k ";
  }
  return stream;
}

std::unique_ptr<ISquare> Empty::clone() const {
  return std::make_unique<Empty>(*this);
}
std::unique_ptr<ISquare> Pawn::clone() const {
  return std::make_unique<Pawn>(*this);
}
std::unique_ptr<ISquare> Knight::clone() const {
  return std::make_unique<Knight>(*this);
}
std::unique_ptr<ISquare> Bishop::clone() const {
  return std::make_unique<Bishop>(*this);
}
std::unique_ptr<ISquare> Rook::clone() const {
  return std::make_unique<Rook>(*this);
}
std::unique_ptr<ISquare> Queen::clone() const {
  return std::make_unique<Queen>(*this);
}
std::unique_ptr<ISquare> King::clone() const {
  return std::make_unique<King>(*this);
}

}  // namespace Chess
