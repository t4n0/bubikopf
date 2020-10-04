#ifndef BOARD_EVALUATION_H
#define BOARD_EVALUATION_H

#include "board/player.h"

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <variant>

namespace Chess
{

// The three variants mean respectively:
// Player: The game is over. The value indicates the winning side.
// int8_t: The game is decided. The value indicates the remaining moves the
// winning side must play before it can force a win.
// float: The game is undecided. The value indicates the current advantage.
using Evaluation = std::variant<Player, int8_t, float>;

const Evaluation MAX_EVAL{Player::max};
const Evaluation MIN_EVAL{Player::min};

inline bool operator<(const Evaluation lhs, const Evaluation rhs)
{
    if (std::holds_alternative<Player>(lhs) && std::holds_alternative<Player>(rhs))
    {
        return std::get<Player>(lhs) < std::get<Player>(rhs);
    }
    else if (std::holds_alternative<int8_t>(lhs) && std::holds_alternative<int8_t>(rhs))
    {
        return std::get<int8_t>(lhs) < std::get<int8_t>(rhs);
    }
    else if (std::holds_alternative<float>(lhs) && std::holds_alternative<float>(rhs))
    {
        return std::get<float>(lhs) < std::get<float>(rhs);
    }
    else if (std::holds_alternative<Player>(lhs) && std::holds_alternative<int8_t>(rhs))
    {
        return !bool(std::get<Player>(lhs));
    }
    else if (std::holds_alternative<Player>(lhs) && std::holds_alternative<float>(rhs))
    {
        return !bool(std::get<Player>(lhs));
    }
    else if (std::holds_alternative<int8_t>(lhs) && std::holds_alternative<Player>(rhs))
    {
        return bool(std::get<Player>(rhs));
    }
    else if (std::holds_alternative<int8_t>(lhs) && std::holds_alternative<float>(rhs))
    {
        return std::get<int8_t>(lhs) < 0;
    }
    else if (std::holds_alternative<float>(lhs) && std::holds_alternative<Player>(rhs))
    {
        return bool(std::get<Player>(rhs));
    }
    else if (std::holds_alternative<float>(lhs) && std::holds_alternative<int8_t>(rhs))
    {
        return std::get<int8_t>(rhs) > 0;
    }
    else
    {
        throw std::runtime_error("Evaluation operator< failed.");
    }
}

inline bool operator>(const Evaluation lhs, const Evaluation rhs)
{
    return rhs < lhs;
}

inline std::ostream& operator<<(std::ostream& stream, const Evaluation output)
{
    if (std::holds_alternative<Player>(output))
    {
        if (bool(std::get<Player>(output)))
        {
            stream << "+";
        }
        else
        {
            stream << "-";
        }
    }
    else if (std::holds_alternative<int8_t>(output))
    {
        int value{std::get<int8_t>(output)};
        if (value > 0)
        {
            stream << "+";
        }
        stream << value;
    }
    else if (std::holds_alternative<float>(output))
    {
        float value{std::get<float>(output)};
        if (value > 0)
        {
            stream << "+";
        }
        stream << value;
    }
    return stream;
}

}  // namespace Chess

#endif
