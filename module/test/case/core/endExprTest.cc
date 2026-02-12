#include "test/byeolTest.hpp"
#include "core/ast/exprs/endExpr.hpp"
#include "core/builtin/scalar/nInt.hpp"
#include "core/builtin/scalar/nStr.hpp"
#include "core/type/ntype.hpp"

using namespace by;
using namespace std;

struct endExprTest: public byeolTest {};

TEST_F(endExprTest, testConstructorWithEmptyBlock) {
    blockExpr block;
    endExpr expr(block);

    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(endExprTest, testConstructorWithNonEmptyBlock) {
    nInt stmt(42);
    blockExpr block(stmt);
    endExpr expr(block);

    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(endExprTest, testInfer) {
    blockExpr block;
    endExpr expr(block);

    str inferredType = expr.infer();
    ASSERT_TRUE(inferredType);
    ASSERT_TRUE(inferredType->isSub<ntype>());
}

TEST_F(endExprTest, testTypeInheritance) {
    blockExpr block;
    endExpr expr(block);

    // Should inherit from blockExpr
    ASSERT_TRUE(expr.isSub<blockExpr>());
    // Should inherit from expr
    ASSERT_TRUE(expr.isSub<by::expr>());
    // Should not be other types
    ASSERT_FALSE(expr.isSub<nInt>());
}

TEST_F(endExprTest, testMultipleInstances) {
    blockExpr block1;
    blockExpr block2;

    endExpr expr1(block1);
    endExpr expr2(block2);

    ASSERT_TRUE(expr1.isSub<endExpr>());
    ASSERT_TRUE(expr2.isSub<endExpr>());
}

TEST_F(endExprTest, testInferConsistency) {
    blockExpr block;
    endExpr expr(block);

    // Multiple calls to infer should work
    str infer1 = expr.infer();
    str infer2 = expr.infer();

    ASSERT_TRUE(infer1);
    ASSERT_TRUE(infer2);
}

TEST_F(endExprTest, testExprTypeCheck) {
    blockExpr block;
    endExpr expr(block);

    // Verify it's recognized as an expr
    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(endExprTest, testBlockExprTypeCheck) {
    blockExpr block;
    endExpr expr(block);

    // Verify it's recognized as a blockExpr
    ASSERT_TRUE(expr.isSub<blockExpr>());
}

TEST_F(endExprTest, testGetStmtsInherited) {
    nInt stmt(100);
    blockExpr block(stmt);
    endExpr expr(block);

    // Should inherit getStmts from blockExpr
    const narr& stmts = expr.getStmts();
    ASSERT_EQ(stmts.len(), 1);
}

TEST_F(endExprTest, testEvalInherited) {
    blockExpr block;
    endExpr expr(block);
    args emptyArgs;

    // Should inherit eval from blockExpr
    str result = expr.eval(emptyArgs);
    ASSERT_TRUE(result);
}

TEST_F(endExprTest, testWithBlockContainingMultipleStatements) {
    nInt stmt1(10);
    nInt stmt2(20);
    nInt stmt3(30);
    blockExpr block(stmt1, stmt2, stmt3);
    endExpr expr(block);

    const narr& stmts = expr.getStmts();
    ASSERT_EQ(stmts.len(), 3);
}

TEST_F(endExprTest, testInferAfterConstruction) {
    blockExpr block;
    endExpr expr(block);

    str inferredType = expr.infer();
    ASSERT_TRUE(inferredType);
}

TEST_F(endExprTest, testWithBlockContainingStrings) {
    nStr stmt("cleanup");
    blockExpr block(stmt);
    endExpr expr(block);

    ASSERT_TRUE(expr.isSub<endExpr>());
    const narr& stmts = expr.getStmts();
    ASSERT_EQ(stmts.len(), 1);
}

TEST_F(endExprTest, testSetEvalInherited) {
    blockExpr block;
    endExpr expr(block);

    nInt value(42);
    expr.setEval(value);

    ASSERT_TRUE(expr.isSub<endExpr>());
}

TEST_F(endExprTest, testIsAbstractInherited) {
    blockExpr block;
    endExpr expr(block);

    // Should inherit isAbstract from blockExpr
    nbool abstract = expr.isAbstract();
    // Just verify it doesn't crash
}

TEST_F(endExprTest, testConstructorPreservesBlockContent) {
    nInt stmt1(5);
    nInt stmt2(10);
    blockExpr block(stmt1, stmt2);
    endExpr expr(block);

    const narr& stmts = expr.getStmts();
    ASSERT_EQ(stmts.len(), 2);
    ASSERT_TRUE(stmts[0].isSub<nInt>());
    ASSERT_TRUE(stmts[1].isSub<nInt>());
}

TEST_F(endExprTest, testEndExprTypeCheck) {
    blockExpr block;
    endExpr expr(block);

    // Verify it's specifically recognized as an endExpr
    ASSERT_TRUE(expr.isSub<endExpr>());
}

TEST_F(endExprTest, testMultipleEndExprsWithDifferentBlocks) {
    nInt stmt1(100);
    blockExpr block1(stmt1);

    nStr stmt2("test");
    blockExpr block2(stmt2);

    endExpr expr1(block1);
    endExpr expr2(block2);

    ASSERT_TRUE(expr1.isSub<endExpr>());
    ASSERT_TRUE(expr2.isSub<endExpr>());
    ASSERT_EQ(expr1.getStmts().len(), 1);
    ASSERT_EQ(expr2.getStmts().len(), 1);
}
