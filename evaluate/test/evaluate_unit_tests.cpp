#include "evaluate/evaluate.h"
#include "evaluate/evaluation.h"

#include <gtest/gtest.h>

namespace Chess
{
namespace
{

TEST(EvaluteMaterial, GivenSamples_ExpectAllHold)
{
    PositionWithBitboards position{};

    position[BOARD_IDX_BLACK + PAWN] = 1;
    Evaluation returned_evaluation{evaluate(position)};
    EXPECT_EQ(std::get<float>(returned_evaluation), -1.0F);

    position[BOARD_IDX_WHITE + PAWN] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

    position[BOARD_IDX_BLACK + KNIGHT] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_EQ(std::get<float>(returned_evaluation), -3.0F);

    position[BOARD_IDX_WHITE + KNIGHT] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

    position[BOARD_IDX_BLACK + BISHOP] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_EQ(std::get<float>(returned_evaluation), -3.0F);

    position[BOARD_IDX_WHITE + BISHOP] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

    position[BOARD_IDX_BLACK + ROOK] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_EQ(std::get<float>(returned_evaluation), -5.0F);

    position[BOARD_IDX_WHITE + ROOK] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

    position[BOARD_IDX_BLACK + QUEEN] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_EQ(std::get<float>(returned_evaluation), -9.0F);

    position[BOARD_IDX_WHITE + QUEEN] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

    position[BOARD_IDX_BLACK + KING] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_EQ(std::get<float>(returned_evaluation), -100.0F);

    position[BOARD_IDX_WHITE + KING] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);
};

}  // namespace
}  // namespace Chess
