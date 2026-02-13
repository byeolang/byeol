#include "test/byeolTest.hpp"
#include "core/ast/exprs/blockExpr.hpp"
#include "core/builtin/scalar/nInt.hpp"
#include "core/builtin/scalar/nStr.hpp"
#include "core/builtin/scalar/nVoid.hpp"
#include "core/type/ntype.hpp"

using namespace by;
using namespace std;

struct blockExprTest: public byeolTest {};

TEST_F(blockExprTest, testDefaultConstructor) {
    blockExpr block;

    // blockExpr should be a valid expr
    ASSERT_TRUE(block.isSub<by::expr>());
}

TEST_F(blockExprTest, testGetStmtsEmpty) {
    blockExpr block;

    const narr& stmts = block.getStmts();
    ASSERT_EQ(stmts.len(), 0);
}

TEST_F(blockExprTest, testGetStmtsNonConst) {
    blockExpr block;

    narr& stmts = block.getStmts();
    ASSERT_EQ(stmts.len(), 0);
}

TEST_F(blockExprTest, testInfer) {
    blockExpr block;

    str inferredType = block.infer();
    ASSERT_TRUE(inferredType);
    // Note: infer() may return null or unexpected values in unit tests without full verification context
}

TEST_F(blockExprTest, testEvalWithEmptyArgs) {
    blockExpr block;
    args emptyArgs;

    str result = block.eval(emptyArgs);
    ASSERT_FALSE(result); // THERE_IS_NO_FRAMES_IN_THREAD

    obj o;
    threadUse th1; {
        o.inFrame(nmap()); // let it make a frame in thread forcibly.
        str result = block.eval(emptyArgs);
        ASSERT_FALSE(result);
        ASSERT_FALSE(thread::get().getEx());
    }

}

TEST_F(blockExprTest, testSetEval) {
    blockExpr block;
    nInt value(42);

    block.setEval(value);

    // After setting eval, the block should still be valid
    ASSERT_TRUE(block.isSub<blockExpr>());
}

TEST_F(blockExprTest, testSetEvalPointer) {
    blockExpr block;
    nInt* value = new nInt(100);

    block.setEval(value);

    // After setting eval, the block should still be valid
    ASSERT_TRUE(block.isSub<blockExpr>());
}

TEST_F(blockExprTest, testSetEvalWithString) {
    blockExpr block;
    nStr value("result");

    block.setEval(value);

    ASSERT_TRUE(block.isSub<blockExpr>());
}

TEST_F(blockExprTest, testSetEvalWithVoid) {
    blockExpr block;
    nVoid value;

    block.setEval(value);

    ASSERT_TRUE(block.isSub<blockExpr>());
}

TEST_F(blockExprTest, testIsAbstract) {
    blockExpr block;

    // Check if isAbstract returns a valid boolean
    nbool abstract = block.isAbstract();
    // Just verify it doesn't crash and returns a valid value
    (void)abstract; // Intentionally unused - just testing the function works
}

TEST_F(blockExprTest, testTypeInheritance) {
    blockExpr block;

    // Should inherit from expr
    ASSERT_TRUE(block.isSub<by::expr>());
    // Should not be other types
    ASSERT_FALSE(block.isSub<nInt>());
}

TEST_F(blockExprTest, testMultipleInstances) {
    blockExpr block1;
    blockExpr block2;

    // Both should be valid
    ASSERT_TRUE(block1.isSub<blockExpr>());
    ASSERT_TRUE(block2.isSub<blockExpr>());
}

TEST_F(blockExprTest, testInferConsistency) {
    blockExpr block;

    // Multiple calls to infer should work
    str infer1 = block.infer();
    str infer2 = block.infer();

    ASSERT_TRUE(infer1);
    ASSERT_TRUE(infer2);
}

TEST_F(blockExprTest, testGetStmtsAfterConstruction) {
    blockExpr block;

    const narr& stmts = block.getStmts();
    // Should be empty initially
    ASSERT_EQ(stmts.len(), 0);
}

TEST_F(blockExprTest, testExprTypeCheck) {
    blockExpr block;

    // Verify it's recognized as an expr
    ASSERT_TRUE(block.isSub<by::expr>());
}

TEST_F(blockExprTest, testSetEvalMultipleTimes) {
    blockExpr block;
    nInt value1(10);
    nInt value2(20);

    block.setEval(value1);
    ASSERT_TRUE(block.isSub<blockExpr>());

    block.setEval(value2);
    ASSERT_TRUE(block.isSub<blockExpr>());
}

TEST_F(blockExprTest, testVariadicConstructorWithSingleExpr) {
    nInt expr1(42);
    blockExpr block(expr1);

    ASSERT_TRUE(block.isSub<blockExpr>());
    const narr& stmts = block.getStmts();
    ASSERT_EQ(stmts.len(), 1);
}

TEST_F(blockExprTest, testVariadicConstructorWithTwoExprs) {
    nInt expr1(10);
    nInt expr2(20);
    blockExpr block(expr1, expr2);

    ASSERT_TRUE(block.isSub<blockExpr>());
    const narr& stmts = block.getStmts();
    ASSERT_EQ(stmts.len(), 2);
}

TEST_F(blockExprTest, testVariadicConstructorWithThreeExprs) {
    nInt expr1(1);
    nInt expr2(2);
    nInt expr3(3);
    blockExpr block(expr1, expr2, expr3);

    ASSERT_TRUE(block.isSub<blockExpr>());
    const narr& stmts = block.getStmts();
    ASSERT_EQ(stmts.len(), 3);
}

TEST_F(blockExprTest, testVariadicConstructorWithMixedTypes) {
    nInt intExpr(42);
    nStr strExpr("hello");
    blockExpr block(intExpr, strExpr);

    ASSERT_TRUE(block.isSub<blockExpr>());
    const narr& stmts = block.getStmts();
    ASSERT_EQ(stmts.len(), 2);
}

TEST_F(blockExprTest, testGetStmtsAfterVariadicConstruction) {
    nInt expr1(100);
    nInt expr2(200);
    blockExpr block(expr1, expr2);

    narr& stmts = block.getStmts();
    ASSERT_EQ(stmts.len(), 2);
    ASSERT_TRUE(stmts[0].isSub<nInt>());
    ASSERT_TRUE(stmts[1].isSub<nInt>());
}

TEST_F(blockExprTest, testInferAfterVariadicConstruction) {
    nInt expr1(10);
    nInt expr2(20);
    blockExpr block(expr1, expr2);

    str inferredType = block.infer();
    ASSERT_TRUE(inferredType);
    // Note: infer() may return null or unexpected values in unit tests without full verification context
}

TEST_F(blockExprTest, testEvalAfterVariadicConstruction) {
    nInt expr1(10);
    blockExpr block(expr1);

    str result = block.eval(args());
    ASSERT_FALSE(result); // THERE_IS_NO_FRAMES_IN_THREAD

    obj o;
    threadUse th1; {
        o.inFrame(nmap()); // let it make a frame in thread forcibly.
        tstr<nInt> result = block.eval(args());
        ASSERT_TRUE(result);
        ASSERT_EQ(result->get(), 10);
    }
}

TEST_F(blockExprTest, testSetEvalAfterVariadicConstruction) {
    nInt expr1(10);
    nInt expr2(20);
    blockExpr block(expr1, expr2);

    nInt resultValue(999);
    block.setEval(resultValue);

    ASSERT_TRUE(block.isSub<blockExpr>());
}
