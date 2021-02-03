#include "bitboard/fen_conversion.h"
#include "bitboard/generate_moves.h"
#include "search/traverse_all_leaves.h"

#include <chrono>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "You must provide depth and FEN as arguments." << std::endl;
    }

    const std::size_t depth = std::stoi(std::string{argv[1]});
    const std::string fen{argv[2]};
    std::cout << "FEN " << fen << std::endl;
    std::cout << "Depth " << depth << std::endl;

    Chess::Position position = Chess::PositionFromFen(fen);
    Chess::MoveStack move_stack{};
    Chess::Statistic stats{};
    const Chess::AbortCondition abort_condition{depth};

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    Chess::TraverseAllLeaves<Chess::GenerateAllPseudoLegalMoves>(position, move_stack.begin(), stats, abort_condition);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Number of static evaluations " << stats.number_of_evaluations << std::endl;
    std::cout << "Time spent = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]"
              << std::endl;

    return 0;
}