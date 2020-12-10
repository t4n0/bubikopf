#include "search/minimax.h"

#include "hardware/trailing_zeros_count.h"

#include <gtest/gtest.h>

#include <iostream>

namespace Chess
{

Position EncodeUniqueIdToZero()
{
    Position position{};
    position[BOARD_IDX_WHITE + PAWN] = 1;
    position[BOARD_IDX_BLACK + PAWN] = 1;
    position[BOARD_IDX_EXTRAS] = BOARD_MASK_WHITE_TURN;
    return position;
}

int DecodeUniqueId(const Position& position)
{
    const int black_pawn_square = tzcnt(position[BOARD_IDX_BLACK + PAWN]);
    const int white_pawn_square = tzcnt(position[BOARD_IDX_WHITE + PAWN]);
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

    const std::size_t side = position.WhiteToMove() ? BOARD_IDX_WHITE : BOARD_IDX_BLACK;
    for (int i = 1; i < 3; i++)
    {
        *move_generation_insertion_iterator++ = ComposeMove(
            tzcnt(position[side + PAWN]), ++unique_id, PAWN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_PUSH);
        if (unique_id == 64)
        {
            throw std::runtime_error{"No more unique squares left."};
        }
    }
    return move_generation_insertion_iterator;
}

struct DebuggingEnabled
{
    static constexpr bool enabled = true;
};

template <typename DebugBehavior>
std::enable_if_t<DebugBehavior::enabled, void> PrintPruningInfo(const Evaluation alpha,
                                                                const Evaluation beta,
                                                                const Position& position,
                                                                const bool entering_node)
{
    const std::string identifier = entering_node ? "-> ID " + std::to_string(DecodeUniqueId(position))
                                                 : "ID " + std::to_string(DecodeUniqueId(position)) + " ->";
    std::cout << identifier << std::endl;
    std::cout << "alpha " << alpha << ", beta " << beta << std::endl;
    if (!entering_node)
    {
        std::cout << "Pruning: " << std::boolalpha << (alpha >= beta) << std::endl;
    }
    std::cout << std::endl;
}

template <typename DebugBehavior>
std::enable_if_t<DebugBehavior::enabled, void> PrintEvaluationInfo(const Evaluation evaluation,
                                                                   const Position& position)
{
    std::cout << "ID " << DecodeUniqueId(position) << std::endl;
    std::cout << "eval " << evaluation << std::endl << std::endl;
}

namespace
{

class MinimaxTestFixture : public testing::Test
{
    void SetUp() final
    {
        EvaluteAccordingToEncodedUniqueId::unique_id_evaluation_order = {};
        EvaluteAccordingToEncodedUniqueId::unique_id_evaluation = {};
    }
    void TearDown() final {}

  public:
    MoveList move_list{};
    Position position{EncodeUniqueIdToZero()};
};

TEST_F(MinimaxTestFixture, GivenDepth3_ExpectEvaluationOrderFromExample)
{
    // Setup
    const int DEPTH{3};

    // Example from https://youtu.be/l-hh51ncgDI?t=372
    EvaluteAccordingToEncodedUniqueId::unique_id_evaluation.at(5) = -1;
    EvaluteAccordingToEncodedUniqueId::unique_id_evaluation.at(6) = 3;
    EvaluteAccordingToEncodedUniqueId::unique_id_evaluation.at(7) = 5;
    EvaluteAccordingToEncodedUniqueId::unique_id_evaluation.at(11) = -6;
    EvaluteAccordingToEncodedUniqueId::unique_id_evaluation.at(12) = -4;

    // Call
    const Evaluation alpha_init = std::numeric_limits<Evaluation>::lowest();
    const Evaluation beta_init = std::numeric_limits<Evaluation>::max();
    minimax<SearchWithAlphaBetaPruning,
            GenerateTwoMovesThatEncodeUniqueId,
            EvaluteAccordingToEncodedUniqueId,
            DebuggingEnabled>(DEPTH, position, move_list.begin(), alpha_init, beta_init);

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

}  // namespace
}  // namespace Chess
