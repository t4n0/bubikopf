#include "bitboard/position.h"
#include "bitboard/position_from_fen.h"
#include "evaluate/test/evaluate_mock.h"
#include "search/statistic.h"
#include "search/test/minimax_mock.h"
#include "search/test/move_generation_mock.h"

#include <cassert>
#include <chrono>
#include <iostream>
#include <string>

long long Chess::Statistic::number_of_evaluations = 0;

int main(int argc, char** argv)
{
    const int depth = std::stoi(std::string{argv[1]});
    const std::string fen{argv[2]};
    std::cout << "FEN " << fen << std::endl;
    std::cout << "Depth " << depth << std::endl;

    assert(argc == 3);  // Must provide depth and fen.

    Chess::PositionWithBitboards position = Chess::PositionFromFen(fen);
    Chess::MoveList move_list{};

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    Chess::minimax<Chess::SearchAllBranchesWithoutPruning, Chess::GenerateAllPseudoLegalMoves, Chess::EvaluteToZero>(
        depth, position, move_list.begin());
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Number of static evaluations " << Chess::Statistic::number_of_evaluations << std::endl;
    std::cout << "Time spent = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]"
              << std::endl;

    return 0;
}