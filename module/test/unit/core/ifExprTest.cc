#include "test/byeolTest.hpp"
#include "core/ast/exprs/ifExpr.hpp"
#include "core/ast/exprs/blockExpr.hpp"
#include "core/builtin/scalar/nInt.hpp"
#include "core/builtin/scalar/nBool.hpp"
#include "core/type/ntype.hpp"

using namespace by;
using namespace std;

struct ifExprTest: public byeolTest {};

TEST_F(ifExprTest, testConstructorWithoutElse) {
    nBool condition(true);
    blockExpr thenBlock;
    ifExpr expr(condition, thenBlock);

    // Should have valid condition and then block
    ASSERT_TRUE(expr.getCondition().isSub<nBool>());
    ASSERT_TRUE(expr.getThen().isSub<blockExpr>());
    ASSERT_EQ(expr.getElse(), nullptr);
}

TEST_F(ifExprTest, testConstructorWithElse) {
    nBool condition(false);
    blockExpr thenBlock;
    blockExpr elseBlock;
    ifExpr expr(condition, thenBlock, elseBlock);

    // Should have all parts
    ASSERT_TRUE(expr.getCondition().isSub<nBool>());
    ASSERT_TRUE(expr.getThen().isSub<blockExpr>());
    ASSERT_NE(expr.getElse(), nullptr);
}

TEST_F(ifExprTest, testGetThen) {
    nBool condition(true);
    blockExpr thenBlock;
    ifExpr expr(condition, thenBlock);

    blockExpr& then = expr.getThen();
    ASSERT_TRUE(then.isSub<blockExpr>());
}

TEST_F(ifExprTest, testGetThenConst) {
    nBool condition(true);
    blockExpr thenBlock;
    const ifExpr expr(condition, thenBlock);

    const blockExpr& then = expr.getThen();
    ASSERT_TRUE(then.isSub<blockExpr>());
}

TEST_F(ifExprTest, testSetThen) {
    nBool condition(true);
    blockExpr thenBlock1;
    ifExpr expr(condition, thenBlock1);

    blockExpr thenBlock2;
    expr.setThen(thenBlock2);

    ASSERT_TRUE(expr.getThen().isSub<blockExpr>());
}

TEST_F(ifExprTest, testSetThenPointer) {
    nBool condition(true);
    blockExpr thenBlock1;
    ifExpr expr(condition, thenBlock1);

    blockExpr* thenBlock2 = new blockExpr();
    expr.setThen(thenBlock2);

    ASSERT_TRUE(expr.getThen().isSub<blockExpr>());
}

TEST_F(ifExprTest, testGetElse) {
    nBool condition(true);
    blockExpr thenBlock;
    blockExpr elseBlock;
    ifExpr expr(condition, thenBlock, elseBlock);

    blockExpr* elsePtr = expr.getElse();
    ASSERT_NE(elsePtr, nullptr);
    ASSERT_TRUE(elsePtr->isSub<blockExpr>());
}

TEST_F(ifExprTest, testGetElseConst) {
    nBool condition(true);
    blockExpr thenBlock;
    blockExpr elseBlock;
    const ifExpr expr(condition, thenBlock, elseBlock);

    const blockExpr* elsePtr = expr.getElse();
    ASSERT_NE(elsePtr, nullptr);
    ASSERT_TRUE(elsePtr->isSub<blockExpr>());
}

TEST_F(ifExprTest, testSetElse) {
    nBool condition(true);
    blockExpr thenBlock;
    ifExpr expr(condition, thenBlock);

    // Initially no else
    ASSERT_EQ(expr.getElse(), nullptr);

    // Set else block
    blockExpr elseBlock;
    expr.setElse(elseBlock);

    // Now should have else
    ASSERT_NE(expr.getElse(), nullptr);
}

TEST_F(ifExprTest, testSetElsePointer) {
    nBool condition(true);
    blockExpr thenBlock;
    ifExpr expr(condition, thenBlock);

    blockExpr* elseBlock = new blockExpr();
    expr.setElse(elseBlock);

    ASSERT_NE(expr.getElse(), nullptr);
}

TEST_F(ifExprTest, testGetCondition) {
    nBool condition(true);
    blockExpr thenBlock;
    ifExpr expr(condition, thenBlock);

    node& cond = expr.getCondition();
    ASSERT_TRUE(cond.isSub<nBool>());
}

TEST_F(ifExprTest, testGetConditionConst) {
    nBool condition(true);
    blockExpr thenBlock;
    const ifExpr expr(condition, thenBlock);

    const node& cond = expr.getCondition();
    ASSERT_TRUE(cond.isSub<nBool>());
}

TEST_F(ifExprTest, testSetCondition) {
    nBool condition1(true);
    blockExpr thenBlock;
    ifExpr expr(condition1, thenBlock);

    nBool condition2(false);
    expr.setCondition(condition2);

    ASSERT_TRUE(expr.getCondition().isSub<nBool>());
}

TEST_F(ifExprTest, testSetConditionPointer) {
    nBool condition1(true);
    blockExpr thenBlock;
    ifExpr expr(condition1, thenBlock);

    nBool* condition2 = new nBool(false);
    expr.setCondition(condition2);

    ASSERT_TRUE(expr.getCondition().isSub<nBool>());
}

TEST_F(ifExprTest, testInferNegative) {
    nBool condition(true);
    blockExpr thenBlock;
    thenBlock.getStmts().add(*new nInt(22));

    threadUse th1;
    {
        obj o;
        o.inFrame();
        ifExpr expr(condition, thenBlock);

        str inferredType = expr.infer();
        ASSERT_FALSE(inferredType); // there is no `else` block.
    }
}

TEST_F(ifExprTest, testInfer) {
    nBool condition(true);
    blockExpr thenBlock;
    thenBlock.getStmts().add(*new nInt(22));
    blockExpr elseBlock;
    elseBlock.getStmts().add(*new nInt(11));

    threadUse th1;
    {
        obj o;
        o.inFrame();
        ifExpr expr(condition, thenBlock, elseBlock);

        str inferredType = expr.infer();
        ASSERT_TRUE(inferredType);
        ASSERT_TRUE(inferredType->isSub<nInt>());
    }
}

TEST_F(ifExprTest, testInferWithElse) {
    nBool condition(true);
    blockExpr thenBlock;
    blockExpr elseBlock;
    ifExpr expr(condition, thenBlock, elseBlock);

    str inferredType = expr.infer();
    ASSERT_TRUE(inferredType);
}

TEST_F(ifExprTest, testEvalWithEmptyArgsNoThrow) {
    nBool condition(true);
    blockExpr thenBlock;
    ifExpr expr(condition, thenBlock);
    args emptyArgs;

    setNoisyEx(false);
    expr.eval(emptyArgs);
    setNoisyEx(true);
    // eval might return an err or void depending on implementation
    // Just ensure it doesn't crash
}

TEST_F(ifExprTest, testTypeInheritance) {
    nBool condition(true);
    blockExpr thenBlock;
    ifExpr expr(condition, thenBlock);

    // Should inherit from expr
    ASSERT_TRUE(expr.isSub<by::expr>());
    // Should not be other types
    ASSERT_FALSE(expr.isSub<nInt>());
}

TEST_F(ifExprTest, testConditionWithIntegerExpression) {
    nInt condition(1); // Non-zero should be truthy
    blockExpr thenBlock;
    ifExpr expr(condition, thenBlock);

    ASSERT_TRUE(expr.getCondition().isSub<nInt>());
}

TEST_F(ifExprTest, testMultipleInstances) {
    nBool cond1(true);
    nBool cond2(false);
    blockExpr then1;
    blockExpr then2;

    ifExpr expr1(cond1, then1);
    ifExpr expr2(cond2, then2);

    ASSERT_TRUE(expr1.getCondition().isSub<nBool>());
    ASSERT_TRUE(expr2.getCondition().isSub<nBool>());
}

TEST_F(ifExprTest, testInferConsistency) {
    nBool condition(true);
    blockExpr thenBlock;
    thenBlock.getStmts().add(*new nInt(22));
    blockExpr elseBlock;
    elseBlock.getStmts().add(*new nInt(2));
    ifExpr expr(condition, thenBlock, elseBlock);

    threadUse th1;
    {
        obj o;
        o.inFrame();

        str infer1 = expr.infer();
        str infer2 = expr.infer();

        ASSERT_TRUE(infer1);
        ASSERT_TRUE(infer2);
    }
}
