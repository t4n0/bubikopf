#include "search/find_best_move.h"

#include "bitboard/fen_conversion.h"
#include "bitboard/generate_moves.h"
#include "evaluate/evaluate.h"
#include "hardware/trailing_zeros_count.h"

#include <gtest/gtest.h>

#include <iostream>

namespace Chess
{

Position EncodeUniqueIdToZero()
{
    Position position{};
    position[kWhiteBoard] = H1 | A8;
    position[kBlackBoard] = H1 | A8;
    position[kWhiteBoard + kPawn] = H1;
    position[kBlackBoard + kPawn] = H1;
    position[kWhiteBoard + kKing] = A8;
    position[kBlackBoard + kKing] = A8;
    position.white_to_move_ = true;
    return position;
}

int DecodeUniqueId(const Position& position)
{
    const int black_pawn_square = tzcnt(position[kBlackBoard + kPawn]);
    const int white_pawn_square = tzcnt(position[kWhiteBoard + kPawn]);
    const int unique_id = std::max(black_pawn_square, white_pawn_square);
    return unique_id;
}

struct EvaluteAccordingToEncodedUniqueId
{
    static constexpr bool evaluate_according_to_encoded_unique_id{true};
    static std::array<int, 64> unique_id_evaluation;
    static std::vector<int> unique_id_evaluation_order;
};
std::vector<int> EvaluteAccordingToEncodedUniqueId::unique_id_evaluation_order{};
std::array<int, 64> EvaluteAccordingToEncodedUniqueId::unique_id_evaluation{};

template <typename Behaviour>
std::enable_if_t<Behaviour::evaluate_according_to_encoded_unique_id, Evaluation> evaluate(const Position& position)
{
    Behaviour::unique_id_evaluation_order.push_back(DecodeUniqueId(position));
    const Evaluation unique_id_evaluation = Behaviour::unique_id_evaluation.at(DecodeUniqueId(position));
    return unique_id_evaluation;
}

struct GenerateTwoMovesThatEncodeUniqueId
{
    static constexpr bool generate_two_moves_that_encode_unique_id{true};
};

template <typename Behavior>
std::enable_if_t<Behavior::generate_two_moves_that_encode_unique_id, MoveList::iterator> GenerateMoves(
    const Position& position,
    MoveList::iterator move_generation_insertion_iterator)
{
    static int unique_id{0};

    const std::size_t side = position.white_to_move_ ? kWhiteBoard : kBlackBoard;
    for (int i = 1; i < 3; i++)
    {
        *move_generation_insertion_iterator++ = ComposeMove(
            tzcnt(position[side + kPawn]), ++unique_id, kPawn, kNoCapture, kNoPromotion, kMoveTypePawnSinglePush);
        if (unique_id == 64)
        {
            throw std::runtime_error{"No more unique squares left."};
        }
    }
    return move_generation_insertion_iterator;
}

namespace
{

TEST(FindBestMovePruningTest, GivenDepth3_ExpectEvaluationOrderFromExample)
{
    // Setup
    const int DEPTH{3};
    MoveList scratch_pad{};
    Position position{EncodeUniqueIdToZero()};
    const Evaluation negamax_sign_for_white{1};
    EvaluteAccordingToEncodedUniqueId::unique_id_evaluation_order = {};
    EvaluteAccordingToEncodedUniqueId::unique_id_evaluation = {};

    // Example from https://youtu.be/l-hh51ncgDI?t=372
    EvaluteAccordingToEncodedUniqueId::unique_id_evaluation.at(5) = -1;
    EvaluteAccordingToEncodedUniqueId::unique_id_evaluation.at(6) = 3;
    EvaluteAccordingToEncodedUniqueId::unique_id_evaluation.at(7) = 5;
    EvaluteAccordingToEncodedUniqueId::unique_id_evaluation.at(11) = -6;
    EvaluteAccordingToEncodedUniqueId::unique_id_evaluation.at(12) = -4;

    // Call
    FindBestMove<GenerateTwoMovesThatEncodeUniqueId, EvaluteAccordingToEncodedUniqueId, DebuggingDisabled>(
        DEPTH, position, scratch_pad.begin(), negamax_sign_for_white);

    // Expect
    std::cout << "Order of evaluation:" << std::endl;
    for (const auto unique_id : EvaluteAccordingToEncodedUniqueId::unique_id_evaluation_order)
    {
        std::cout << unique_id << " ";
    }
    std::cout << std::endl;
    const std::vector<int> expected_unique_id_evaluation_order{5, 6, 7, 11, 12};
    EXPECT_EQ(EvaluteAccordingToEncodedUniqueId::unique_id_evaluation_order, expected_unique_id_evaluation_order);
}

class FindBestMoveTestFixture : public testing::TestWithParam<std::tuple<std::string, std::string>>
{
  public:
    std::string GetFen() { return std::get<0>(GetParam()); }
    std::string GetExpectedBestMove() { return std::get<1>(GetParam()); }
    Evaluation GetNegaMaxSign()
    {
        const auto side = TokenizeFen(GetFen()).at(kFenTokenSide);
        return side == "w" ? Evaluation{1} : Evaluation{-1};
    }
};

TEST_P(FindBestMoveTestFixture, GivenCheckmateIn3_ExpectCorrectContinuation)
{
    // Setup
    const int depth{6};  // Allow king to be captured
    Position position{PositionFromFen(GetFen())};
    MoveList scratch_pad{};

    // Call
    Bitmove best_move;
    Evaluation evaluation;
    const auto side = TokenizeFen(GetFen()).at(kFenTokenSide);
    std::tie(best_move, evaluation) = FindBestMove<GenerateAllPseudoLegalMoves, EvaluteMaterial, DebuggingDisabled>(
        depth, position, scratch_pad.begin(), GetNegaMaxSign());

    // Expect
    EXPECT_EQ(ToUciString(best_move), GetExpectedBestMove());
}

const std::array<std::tuple<std::string, std::string>, 4> various_check_mate_in_three_positions{{
    {"r2q1rk1/pb3p1p/1pn3p1/2p1R2Q/2P5/2BB4/P4PPP/R5K1 w - - 0 21", "h5h7"},
    {"r2q2kr/ppp1b1pp/2n5/4B3/3Pn1b1/2P5/PP4PP/RN1Q1RK1 w - - 1 12", "d1b3"},
    {"7r/Q1p2ppp/1p3k2/1Bb5/5q2/2N5/PPPrR1KP/R7 b - - 2 21", "f4g4"},
    {"7r/6k1/1p2p1p1/p2pP1b1/P1NP1qpr/2P3N1/1R2QPP1/4R1K1 b - - 0 37", "h4h1"},
}};

INSTANTIATE_TEST_SUITE_P(VariousCheckmateInThreePositions,
                         FindBestMoveTestFixture,
                         testing::ValuesIn(various_check_mate_in_three_positions));

class FindBestMoveInFinalPosition : public testing::TestWithParam<std::tuple<std::string, Evaluation>>
{
  public:
    std::string GetFen() { return std::get<0>(GetParam()); }
    Evaluation GetExpectedEvaluation() { return std::get<1>(GetParam()); }
    Evaluation GetNegaMaxSign()
    {
        const auto side = TokenizeFen(GetFen()).at(kFenTokenSide);
        return side == "w" ? Evaluation{1} : Evaluation{-1};
    }
};

TEST_P(FindBestMoveInFinalPosition, GivenEndgamePositions_ExpectCorrectMoveAndEvaluation)
{
    // Setup
    const int depth{6};
    Position position{PositionFromFen(GetFen())};
    MoveList scratch_pad{};

    // Call
    const auto [best_move, evaluation] = FindBestMove<GenerateAllPseudoLegalMoves, EvaluteMaterial, DebuggingDisabled>(
        depth, position, scratch_pad.begin(), GetNegaMaxSign());

    // Expect
    EXPECT_EQ(ToUciString(best_move), kUciNullMove);
    EXPECT_FLOAT_EQ(evaluation, GetExpectedEvaluation());
}

const std::array<std::tuple<std::string, Evaluation>, 4> kFinalPositions{{
    {"8/8/8/8/8/2K1Q3/8/3k4 b - - 0 1", Evaluation{0.0}},      // stalemate on blacks turn
    {"8/8/8/8/8/2k1q3/8/3K4 w - - 0 1", Evaluation{0.0}},      // stalemate on whites turn
    {"8/8/8/8/8/2k5/3q4/3K4 w - - 0 1", Evaluation{-1000.0}},  // white is checkmate, eval -> opponent is losing
    {"8/8/8/8/8/2K5/3Q4/3k4 b - - 0 1", Evaluation{-1000.0}},  // black is checkmate, eval -> opponent is losing
}};

INSTANTIATE_TEST_SUITE_P(VariousFinalPositions, FindBestMoveInFinalPosition, testing::ValuesIn(kFinalPositions));

}  // namespace
}  // namespace Chess
