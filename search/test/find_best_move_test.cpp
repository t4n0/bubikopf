#include "search/find_best_move.h"

#include "bitboard/fen_conversion.h"
#include "bitboard/generate_moves.h"
#include "bitboard/uci_conversion.h"
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
std::enable_if_t<Behaviour::evaluate_according_to_encoded_unique_id, Evaluation> Evaluate(const Position& position)
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
std::enable_if_t<Behavior::generate_two_moves_that_encode_unique_id, MoveStack::iterator> GenerateMoves(
    const Position& position,
    MoveStack::iterator move_generation_insertion_iterator)
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

constexpr std::size_t kPrincipalVariationLength{2};
struct CheckIfPrincipalVariationGetsEvaluatedFirst
{
    static constexpr bool check_if_principal_variation_gets_evaluated_first{true};
    static std::string fen;
    static std::array<Bitmove, kPrincipalVariationLength> principal_variation_to_check;
    static bool evaluation_called;
    static bool principal_variation_was_evaluated_fist;
};
std::array<Bitmove, kPrincipalVariationLength>
    CheckIfPrincipalVariationGetsEvaluatedFirst::principal_variation_to_check{};
std::string CheckIfPrincipalVariationGetsEvaluatedFirst::fen{};
bool CheckIfPrincipalVariationGetsEvaluatedFirst::evaluation_called{false};
bool CheckIfPrincipalVariationGetsEvaluatedFirst::principal_variation_was_evaluated_fist{false};

template <typename Behavior>
std::enable_if_t<Behavior::check_if_principal_variation_gets_evaluated_first, Evaluation> Evaluate(
    const Position& position)
{
    if (!Behavior::evaluation_called)
    {
        Position expected_position{PositionFromFen(Behavior::fen)};
        for (const auto move : Behavior::principal_variation_to_check)
        {
            expected_position.MakeMove(move);
        }
        Behavior::principal_variation_was_evaluated_fist = (position == expected_position);
        std::cout << "Expected:\n";
        PrettyPrintFen(FenFromPosition(expected_position));
        std::cout << "Encountered:\n";
        PrettyPrintFen(FenFromPosition(position));
    }
    Behavior::evaluation_called = true;
    return Evaluation{0};
}

struct CountEvaluations
{
    static constexpr bool count_evaluations{true};
    static long number_of_evaluations;
};
long CountEvaluations::number_of_evaluations{};

template <typename Behaviour>
std::enable_if_t<Behaviour::count_evaluations, Evaluation> Evaluate(const Position& position)
{
    std::ignore = position;
    CountEvaluations::number_of_evaluations++;
    return 0;
}

namespace
{

TEST(FindBestMovePruningTest, GivenDepth3_ExpectEvaluationOrderFromExample)
{
    // Setup
    constexpr std::size_t full_search_depth = 3;
    constexpr Chess::AbortCondition abort_condition{full_search_depth};
    MoveStack move_stack{};
    PrincipalVariation principal_variation{};
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
        position, principal_variation, move_stack.begin(), negamax_sign_for_white, abort_condition);

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

constexpr std::size_t kPliesForCheckmateInThree{6};  // one additional ply to test for nullmove as last move
using ForcedCheckmatePossibleParameter = std::tuple<std::string, std::array<std::string, kPliesForCheckmateInThree>>;
class FindBestMoveWhenForcedCheckmatePossible : public testing::TestWithParam<ForcedCheckmatePossibleParameter>
{
  public:
    std::string GetFen() { return std::get<0>(GetParam()); }
    std::array<std::string, kPliesForCheckmateInThree> GetWinningLine() { return std::get<1>(GetParam()); }
    Evaluation GetNegaMaxSign()
    {
        const auto side = TokenizeFen(GetFen()).at(kFenTokenSide);
        return side == "w" ? Evaluation{1} : Evaluation{-1};
    }
};

TEST_P(FindBestMoveWhenForcedCheckmatePossible, GivenCheckmateIn3_ExpectCorrectContinuation)
{
    // Setup
    constexpr std::size_t full_search_depth = 6;
    constexpr Chess::AbortCondition abort_condition{full_search_depth};
    Position position{PositionFromFen(GetFen())};
    MoveStack move_stack{};
    PrincipalVariation principal_variation{};

    // Call
    FindBestMove<GenerateAllPseudoLegalMoves, EvaluateMaterial, DebuggingDisabled>(
        position, principal_variation, move_stack.begin(), GetNegaMaxSign(), abort_condition);

    // Expect
    for (std::size_t index{0}; index < kPliesForCheckmateInThree; index++)
    {
        EXPECT_EQ(ToUciString(principal_variation.at(index)), GetWinningLine().at(index))
            << "lines differ at index: " << index;
    }
}

const std::array<ForcedCheckmatePossibleParameter, 4> kVariousCheckmateIn3Positions{{
    {"r2q1rk1/pb3p1p/1pn3p1/2p1R2Q/2P5/2BB4/P4PPP/R5K1 w - - 0 21", {"h5h7", "g8h7", "e5h5", "h7g8", "h5h8", "0000"}},
    {"r2q2kr/ppp1b1pp/2n5/4B3/3Pn1b1/2P5/PP4PP/RN1Q1RK1 w - - 1 12", {"d1b3", "d8d5", "b3d5", "g4e6", "d5e6", "0000"}},
    {"7r/Q1p2ppp/1p3k2/1Bb5/5q2/2N5/PPPrR1KP/R7 b - - 2 21", {"f4g4", "g2h1", "g4f3", "e2g2", "f3g2", "0000"}},
    {"7r/6k1/1p2p1p1/p2pP1b1/P1NP1qpr/2P3N1/1R2QPP1/4R1K1 b - - 0 37",
     {"h4h1", "g3h1", "f4h2", "g1f1", "h2h1", "0000"}},
}};

INSTANTIATE_TEST_SUITE_P(VariousCheckmateInThreePositions,
                         FindBestMoveWhenForcedCheckmatePossible,
                         testing::ValuesIn(kVariousCheckmateIn3Positions));

class FindBestMoveDetermineGameResult : public testing::TestWithParam<std::tuple<std::string, Evaluation>>
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

TEST_P(FindBestMoveDetermineGameResult, GivenDecidedPosition_ExpectCorrectEvaluation)
{
    // Setup
    constexpr std::size_t full_search_depth = 6;
    constexpr Chess::AbortCondition abort_condition{full_search_depth};
    Position position{PositionFromFen(GetFen())};
    MoveStack move_stack{};
    PrincipalVariation principal_variation{};

    // Call
    const auto evaluation = FindBestMove<GenerateAllPseudoLegalMoves, EvaluateMaterial, DebuggingDisabled>(
        position, principal_variation, move_stack.begin(), GetNegaMaxSign(), abort_condition);

    // Expect
    EXPECT_FLOAT_EQ(evaluation, GetExpectedEvaluation());
}

const std::array<std::tuple<std::string, Evaluation>, 10> kFinalPositions{{
    {"8/8/8/8/8/2K1Q3/8/3k4 b - - 0 1", Evaluation{0.0}},                              // stalemate on blacks turn
    {"8/8/8/8/8/2k1q3/8/3K4 w - - 0 1", Evaluation{0.0}},                              // stalemate on whites turn
    {"8/8/8/8/8/2k5/3q4/3K4 w - - 0 1", Evaluation{-1000.0}},                          // white checkmate
    {"8/8/8/8/8/2K5/3Q4/3k4 b - - 0 1", Evaluation{-1000.0}},                          // black checkmate
    {"6rk/5prp/1p3Q2/4N3/1P4b1/P3q1P1/7P/R1R2K2 b - - 1 30", Evaluation{999.0}},       // white checkmate in one ply
    {"5b1r/R2R1p2/1pk1p2p/8/2P5/1PnK2P1/7P/8 w - - 3 33", Evaluation{999.0}},          // black checkmate in one ply
    {"r6k/pp3Bpp/2p2B2/8/4N2Q/P4PP1/2P3K1/1R1r1q2 w - - 1 27", Evaluation{-998.0}},    // white checkmate in three plies
    {"5r1k/4b1p1/p6R/1p6/1P1p1QP1/P2P4/B1r2RK1/3q4 b - - 0 36", Evaluation{-998.0}},   // black checkmate in three plies
    {"r6k/pp3Bpp/2p2B2/1q6/4N2Q/P4PP1/2P3K1/1R1r4 b - - 0 26", Evaluation{997.0}},     // white checkmate in three plies
    {"5r1k/4b1p1/p3R2p/1p6/1P1p1QP1/P2P4/B1r2RK1/3q4 w - - 1 36", Evaluation{997.0}},  // black checkmate in three plies
}};

INSTANTIATE_TEST_SUITE_P(VariousFinalPositions, FindBestMoveDetermineGameResult, testing::ValuesIn(kFinalPositions));

class FindBestMoveInvestigatesPrincipalVariationFirst : public testing::TestWithParam<std::array<Bitmove, 2>>
{
  public:
    void SetUp() final
    {
        CheckIfPrincipalVariationGetsEvaluatedFirst::principal_variation_to_check.fill(kBitNullMove);
        CheckIfPrincipalVariationGetsEvaluatedFirst::fen = "";
        CheckIfPrincipalVariationGetsEvaluatedFirst::evaluation_called = false;
        CheckIfPrincipalVariationGetsEvaluatedFirst::principal_variation_was_evaluated_fist = false;
    }
    void TearDown() final {}

    std::array<Bitmove, kPrincipalVariationLength> GetPrincipalVariation() { return GetParam(); }
};

const std::string kSimpleArbitraryPosition{"k7/p7/8/8/8/8/P7/K7 w - - 0 1"};

TEST_P(FindBestMoveInvestigatesPrincipalVariationFirst,
       GivenStandardPosition_ExpectMovesFromPrincipalVariationSortedFirst)
{
    // Setup
    constexpr std::size_t full_search_depth = kPrincipalVariationLength;
    constexpr Chess::AbortCondition abort_condition{full_search_depth};
    constexpr Evaluation negamax_sign_for_starting_position{1};
    Position position{PositionFromFen(kSimpleArbitraryPosition)};
    MoveStack move_stack{};
    PrincipalVariation principal_variation{};
    const auto expected_principal_variation{GetPrincipalVariation()};

    std::copy(std::begin(expected_principal_variation),  // provide input for search
              std::end(expected_principal_variation),
              std::begin(principal_variation));

    std::copy(std::begin(expected_principal_variation),  // set expectation in static spy class
              std::end(expected_principal_variation),
              std::begin(CheckIfPrincipalVariationGetsEvaluatedFirst::principal_variation_to_check));
    CheckIfPrincipalVariationGetsEvaluatedFirst::fen = kSimpleArbitraryPosition;

    // Call
    std::ignore =
        FindBestMove<GenerateAllPseudoLegalMoves, CheckIfPrincipalVariationGetsEvaluatedFirst, DebuggingDisabled>(
            position, principal_variation, move_stack.begin(), negamax_sign_for_starting_position, abort_condition);

    // Expect
    EXPECT_TRUE(CheckIfPrincipalVariationGetsEvaluatedFirst::principal_variation_was_evaluated_fist);
}

const std::array<std::array<Bitmove, kPrincipalVariationLength>, 2> kArbitraryPrincipalVariations{{
    {ComposeMove(tzcnt(A2), tzcnt(A3), kPawn, kNoCapture, kNoPromotion, kMoveTypePawnSinglePush),
     ComposeMove(tzcnt(A7), tzcnt(A6), kPawn, kNoCapture, kNoPromotion, kMoveTypePawnSinglePush)},
    {ComposeMove(tzcnt(A1), tzcnt(B1), kKing, kNoCapture, kNoPromotion, kMoveTypeQuietNonPawn),
     ComposeMove(tzcnt(A8), tzcnt(B8), kKing, kNoCapture, kNoPromotion, kMoveTypeQuietNonPawn)},
}};

INSTANTIATE_TEST_SUITE_P(ArbitraryPrincipalVariations,
                         FindBestMoveInvestigatesPrincipalVariationFirst,
                         testing::ValuesIn(kArbitraryPrincipalVariations));

TEST(PrincipalVariationSmokeTest, GivenCheckmateInThree_ExpectPrincipalVariationNeedsLessEvaluations)
{
    // Setup
    constexpr const char* const mate_in_three = "7r/Q1p2ppp/1p3k2/1Bb5/5q2/2N5/PPPrR1KP/R7 b - - 2 21";
    Position position{PositionFromFen(mate_in_three)};
    PrincipalVariation principal_variation{};
    MoveStack move_stack{};
    constexpr Evaluation negamax_sign{-1};
    constexpr std::size_t full_search_depth = 6;
    constexpr Chess::AbortCondition abort_condition{full_search_depth};

    // Call
    CountEvaluations::number_of_evaluations = 0;
    std::ignore = FindBestMove<GenerateAllPseudoLegalMoves, CountEvaluations, DebuggingDisabled>(
        position, principal_variation, move_stack.begin(), negamax_sign, abort_condition);
    const auto number_of_evaluations_without_principal_variation = CountEvaluations::number_of_evaluations;

    ClearSublines(principal_variation);

    CountEvaluations::number_of_evaluations = 0;
    std::ignore = FindBestMove<GenerateAllPseudoLegalMoves, CountEvaluations, DebuggingDisabled>(
        position, principal_variation, move_stack.begin(), negamax_sign, abort_condition);
    const auto number_of_evaluations_with_principal_variation = CountEvaluations::number_of_evaluations;

    // Expect
    EXPECT_LT(number_of_evaluations_with_principal_variation, number_of_evaluations_without_principal_variation);
}

TEST(FindBestMoveTest, GivenTimeForCalculationIsOver_ExpectThrowsCalculationIsDue)
{
    // Setup
    Position position{PositionFromFen(kStandardStartingPosition)};
    PrincipalVariation principal_variation{};
    MoveStack move_stack{};
    constexpr Evaluation negamax_sign_for_starting_position{1};
    constexpr std::size_t full_search_depth = 8;
    constexpr auto beginning_of_time = std::chrono::steady_clock::time_point::min();
    constexpr Chess::AbortCondition abort_condition{full_search_depth, beginning_of_time};

    // Call& Expect
    EXPECT_THROW(
        (FindBestMove<GenerateAllPseudoLegalMoves, EvaluateMaterial, DebuggingDisabled>(
            position, principal_variation, move_stack.begin(), negamax_sign_for_starting_position, abort_condition)),
        CalculationWasDue);
}

}  // namespace
}  // namespace Chess
