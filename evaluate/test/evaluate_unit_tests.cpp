#include "evaluate/evaluate.h"

#include <gtest/gtest.h>

namespace Chess
{
namespace
{

TEST(EvaluteMaterial, GivenSamples_ExpectAllHold)
{
    Position position{};

    position[kBlackBoard + kPawn] = 1;
    Evaluation returned_evaluation{evaluate(position)};
    EXPECT_FLOAT_EQ(returned_evaluation, -1.0F);

    position[kWhiteBoard + kPawn] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, 0.0F);

    position[kBlackBoard + kKnight] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, -3.0F);

    position[kWhiteBoard + kKnight] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, 0.0F);

    position[kBlackBoard + kBishop] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, -3.0F);

    position[kWhiteBoard + kBishop] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, 0.0F);

    position[kBlackBoard + kRook] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, -5.0F);

    position[kWhiteBoard + kRook] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, 0.0F);

    position[kBlackBoard + kQueen] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, -9.0F);

    position[kWhiteBoard + kQueen] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, 0.0F);

    position[kBlackBoard + kKing] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, -100.0F);

    position[kWhiteBoard + kKing] = 1;
    returned_evaluation = evaluate(position);
    EXPECT_FLOAT_EQ(returned_evaluation, 0.0F);
};

}  // namespace
}  // namespace Chess
