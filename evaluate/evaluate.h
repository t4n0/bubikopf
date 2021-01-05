#ifndef EVALUATE_EVALUATE_H
#define EVALUATE_EVALUATE_H

#include "bitboard/position.h"
#include "hardware/population_count.h"

namespace Chess
{

constexpr Evaluation kPawnValue{1};
constexpr Evaluation kKnightValue{3};
constexpr Evaluation kBishopValue{3};
constexpr Evaluation kRookValue{5};
constexpr Evaluation kQueenValue{9};
constexpr Evaluation kKingValue{100};

struct EvaluateMaterial
{
    static constexpr bool evaluate_material{true};
};

template <typename Behaviour = EvaluateMaterial>
std::enable_if_t<Behaviour::evaluate_material, Evaluation> Evaluate(const Position& position)
{
    return (popcnt(position[kWhiteBoard + kPawn]) - popcnt(position[kBlackBoard + kPawn])) * kPawnValue +
           (popcnt(position[kWhiteBoard + kKnight]) - popcnt(position[kBlackBoard + kKnight])) * kKnightValue +
           (popcnt(position[kWhiteBoard + kBishop]) - popcnt(position[kBlackBoard + kBishop])) * kBishopValue +
           (popcnt(position[kWhiteBoard + kRook]) - popcnt(position[kBlackBoard + kRook])) * kRookValue +
           (popcnt(position[kWhiteBoard + kQueen]) - popcnt(position[kBlackBoard + kQueen])) * kQueenValue +
           (popcnt(position[kWhiteBoard + kKing]) - popcnt(position[kBlackBoard + kKing])) * kKingValue;
}

}  // namespace Chess

#endif
