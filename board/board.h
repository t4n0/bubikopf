#ifndef BOARD_BOARD_H
#define BOARD_BOARD_H

#include "board/i_square.h"

#include <array>
#include <iostream>
#include <vector>

namespace Chess
{

struct Coordinate
{
    int8_t col{};
    int8_t row{};

    Coordinate& operator+=(const Coordinate& rhs);
    Coordinate& operator-=(const Coordinate& rhs);
};
using Coordinates = std::vector<Coordinate>;

bool operator==(const Coordinate& a, const Coordinate& b);
bool operator!=(const Coordinate& a, const Coordinate& b);

Coordinate operator+(Coordinate a, const Coordinate& b);
Coordinate operator-(Coordinate a, const Coordinate& b);

std::size_t ToIdx(const std::string& uci_square);
std::size_t ToIdx(const char file, const int8_t rank);
std::size_t ToIdx(const Coordinate coor);
Coordinate ToCoor(const std::size_t idx);
std::string ToUciSquare(const std::size_t idx);
bool IsOnTheBoard(const Coordinate coordinate);

class Board
{
  public:
    ISquarePtr Get(const Coordinate coor) const;
    ISquarePtr Get(const std::size_t idx) const;
    void Set(const Coordinate coor, const ISquarePtr square);
    void Set(const std::size_t idx, const ISquarePtr square);
    void SwapSquares(const std::size_t a, const std::size_t b);

    std::array<ISquarePtr, 64> squares_{};
};

std::ostream& operator<<(std::ostream& stream, const Board& piece);

}  // namespace Chess

#endif
