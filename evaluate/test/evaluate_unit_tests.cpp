#include "evaluate/evaluate.h"

#include <gtest/gtest.h>

namespace Chess
{
namespace
{

TEST(EvaluteMaterial, GivenSamples_ExpectAllHold)
{
    Position position{};

    position[kBlackBoard + PAWN] = 1;
    Evaluation returned_evaluation{evaluate(position)};
    EXPECT_FLOAT_EQ(returned_evaluation, -1.0F);

    position[kWhiteBoard + PAWN] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, 0.0F);

    position[kBlackBoard + KNIGHT] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, -3.0F);

    position[kWhiteBoard + KNIGHT] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, 0.0F);

    position[kBlackBoard + BISHOP] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, -3.0F);

    position[kWhiteBoard + BISHOP] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, 0.0F);

    position[kBlackBoard + ROOK] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, -5.0F);

    position[kWhiteBoard + ROOK] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, 0.0F);

    position[kBlackBoard + QUEEN] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, -9.0F);

    position[kWhiteBoard + QUEEN] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, 0.0F);

    position[kBlackBoard + KING] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, -100.0F);

    position[kWhiteBoard + KING] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, 0.0F);
};

}  // namespace
}  // namespace Chess
