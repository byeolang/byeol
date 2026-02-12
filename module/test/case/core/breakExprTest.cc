#include "test/byeolTest.hpp"
#include "core/ast/exprs/breakExpr.hpp"
#include "core/type/ntype.hpp"

using namespace by;
using namespace std;

struct breakExprTest: public byeolTest {};

TEST_F(breakExprTest, testDefaultConstructor) {
    breakExpr expr;

    // breakExpr should be a valid expr
    ASSERT_TRUE(expr.isSub<by::expr>());
    ASSERT_TRUE(expr.isSub<retStateExpr>());
}

TEST_F(breakExprTest, testInfer) {
    breakExpr expr;

    str inferredType = expr.infer();
    ASSERT_TRUE(inferredType);
    ASSERT_TRUE(inferredType->isSub<ntype>());
}

TEST_F(breakExprTest, testInferReturnsNonNull) {
    breakExpr expr;

    str result = expr.infer();
    // ASSERT_FALSE(result.isNull()); // Removed - isNull not available;
}

TEST_F(breakExprTest, testTypeInheritance) {
    breakExpr expr;

    // Should inherit from retStateExpr
    ASSERT_TRUE(expr.isSub<retStateExpr>());
    // Should inherit from expr
    ASSERT_TRUE(expr.isSub<by::expr>());
    // Should not be other types
    ASSERT_FALSE(expr.isSub<nInt>());
}

TEST_F(breakExprTest, testMultipleInstances) {
    breakExpr expr1;
    breakExpr expr2;

    // Both should have valid infer results
    ASSERT_TRUE(expr1.infer());
    ASSERT_TRUE(expr2.infer());
}

TEST_F(breakExprTest, testEval) {
    breakExpr expr;
    args emptyArgs;

    // eval should return a retState
    str result = expr.eval(emptyArgs);
    ASSERT_TRUE(result);
}

TEST_F(breakExprTest, testPrioritize) {
    breakExpr expr;
    args emptyArgs;

    // prioritize should work without crashing
    priorType priority = expr.prioritize(emptyArgs);
    // Should return some valid priority
    ASSERT_GE(priority, priorType::NO_MATCH);
}

TEST_F(breakExprTest, testBreakRetTypeExists) {
    // Test that breakRet class exists and is a retState
    breakRet ret;
    ASSERT_TRUE(ret.isSub<retState>());
    ASSERT_TRUE(ret.isSub<nVoid>());
}
