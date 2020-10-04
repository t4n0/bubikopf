#ifndef BOARD_POSITION_H
#define BOARD_POSITION_H

#include "board/board.h"
#include "board/evaluation.h"

#include <iostream>
#include <optional>

namespace Chess
{

struct Castling
{
    bool queenside{true};
    bool kingside{true};
};

class Position
{
  public:
    Position();

    Board board_{};
    int static_plies_{0};
    int plies_{0};
    std::optional<Coordinate> en_passant_{};
    std::string previous_move_{};
    std::optional<Evaluation> evaluation_{};

    std::vector<Position> FindPlies() const;
    void LogMove(const std::size_t source, const std::size_t target, const std::optional<SquareId> promotion = {});
    Player GetTurn() const;
    Castling GetCastling(const Player player) const;
    void SetCastling(const Player player, const Castling castling);

  private:
    Castling castling_black_{true, true};
    Castling castling_white_{true, true};
};

std::ostream& operator<<(std::ostream& stream, const Position& position);

}  // namespace Chess

#endif
