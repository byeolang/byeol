#include "test/byeolTest.hpp"
#include "core/ast/exprs/assignExpr.hpp"
#include "core/builtin/scalar/nInt.hpp"
#include "core/builtin/scalar/nStr.hpp"
#include "core/builtin/scalar/nByte.hpp"
#include "core/builtin/scalar/nBool.hpp"
#include "core/type/ntype.hpp"

using namespace by;
using namespace std;

struct assignExprTest: public byeolTest {};

TEST_F(assignExprTest, testConstructor) {
    nInt lhs(10);
    nInt rhs(20);
    assignExpr expr(lhs, rhs);

    ASSERT_TRUE(expr.getLeft().isSub<nInt>());
    ASSERT_TRUE(expr.getRight().isSub<nInt>());
}

TEST_F(assignExprTest, testGetLeft) {
    nInt lhs(42);
    nInt rhs(100);
    assignExpr expr(lhs, rhs);

    const node& left = expr.getLeft();
    ASSERT_TRUE(left.isSub<nInt>());
    ASSERT_EQ(left.cast<nInt>()->get(), 42);
}

TEST_F(assignExprTest, testGetRight) {
    nInt lhs(10);
    nInt rhs(200);
    assignExpr expr(lhs, rhs);

    const node& right = expr.getRight();
    ASSERT_TRUE(right.isSub<nInt>());
    ASSERT_EQ(right.cast<nInt>()->get(), 200);
}

TEST_F(assignExprTest, testInfer) {
    nInt lhs(10);
    nInt rhs(20);
    assignExpr expr(lhs, rhs);

    str inferredType = expr.infer();
    ASSERT_TRUE(inferredType);
    // Note: infer() may return null or unexpected values in unit tests without full verification context
}

TEST_F(assignExprTest, testEvalWithEmptyArgs) {
    nInt lhs(10);
    nInt rhs(20);
    assignExpr expr(lhs, rhs);
    args emptyArgs;

    str result = expr.eval(emptyArgs);
    // eval should return a result
    ASSERT_TRUE(result);
}

TEST_F(assignExprTest, testTypeInheritance) {
    nInt lhs(10);
    nInt rhs(20);
    assignExpr expr(lhs, rhs);

    // Should inherit from expr
    ASSERT_TRUE(expr.isSub<by::expr>());
    // Should not be other types
    ASSERT_FALSE(expr.isSub<nInt>());
}

TEST_F(assignExprTest, testMultipleInstances) {
    nInt lhs1(10);
    nInt rhs1(20);
    nInt lhs2(30);
    nInt rhs2(40);

    assignExpr expr1(lhs1, rhs1);
    assignExpr expr2(lhs2, rhs2);

    ASSERT_TRUE(expr1.getLeft().isSub<nInt>());
    ASSERT_TRUE(expr2.getLeft().isSub<nInt>());
    ASSERT_EQ(expr1.getLeft().cast<nInt>()->get(), 10);
    ASSERT_EQ(expr2.getLeft().cast<nInt>()->get(), 30);
}

TEST_F(assignExprTest, testWithStringValues) {
    nStr lhs("variable");
    nStr rhs("value");
    assignExpr expr(lhs, rhs);

    const node& left = expr.getLeft();
    const node& right = expr.getRight();
    ASSERT_TRUE(left.isSub<nStr>());
    ASSERT_TRUE(right.isSub<nStr>());
}

TEST_F(assignExprTest, testWithBooleanValues) {
    nBool lhs(true);
    nBool rhs(false);
    assignExpr expr(lhs, rhs);

    const node& left = expr.getLeft();
    const node& right = expr.getRight();
    ASSERT_TRUE(left.isSub<nBool>());
    ASSERT_TRUE(right.isSub<nBool>());
}

TEST_F(assignExprTest, testWithByteValues) {
    nByte lhs(100);
    nByte rhs(200);
    assignExpr expr(lhs, rhs);

    const node& left = expr.getLeft();
    const node& right = expr.getRight();
    ASSERT_TRUE(left.isSub<nByte>());
    ASSERT_TRUE(right.isSub<nByte>());
}

TEST_F(assignExprTest, testInferConsistency) {
    nInt lhs(10);
    nInt rhs(20);
    assignExpr expr(lhs, rhs);

    // Multiple calls to infer should work
    str infer1 = expr.infer();
    str infer2 = expr.infer();

    ASSERT_TRUE(infer1);
    ASSERT_TRUE(infer2);
}

TEST_F(assignExprTest, testDifferentOperandTypes) {
    nInt intLhs(10);
    nInt intRhs(20);
    assignExpr intExpr(intLhs, intRhs);
    ASSERT_TRUE(intExpr.getLeft().isSub<scalar>());
    ASSERT_TRUE(intExpr.getRight().isSub<scalar>());

    nStr strLhs("x");
    nStr strRhs("y");
    assignExpr strExpr(strLhs, strRhs);
    ASSERT_TRUE(strExpr.getLeft().isSub<scalar>());
    ASSERT_TRUE(strExpr.getRight().isSub<scalar>());
}

TEST_F(assignExprTest, testWithZeroValue) {
    nInt lhs(0);
    nInt rhs(0);
    assignExpr expr(lhs, rhs);

    const node& left = expr.getLeft();
    const node& right = expr.getRight();
    ASSERT_TRUE(left.isSub<nInt>());
    ASSERT_TRUE(right.isSub<nInt>());
    ASSERT_EQ(left.cast<nInt>()->get(), 0);
    ASSERT_EQ(right.cast<nInt>()->get(), 0);
}

TEST_F(assignExprTest, testWithNegativeValues) {
    nInt lhs(-10);
    nInt rhs(-20);
    assignExpr expr(lhs, rhs);

    const node& left = expr.getLeft();
    const node& right = expr.getRight();
    ASSERT_EQ(left.cast<nInt>()->get(), -10);
    ASSERT_EQ(right.cast<nInt>()->get(), -20);
}

TEST_F(assignExprTest, testExprTypeCheck) {
    nInt lhs(10);
    nInt rhs(20);
    assignExpr expr(lhs, rhs);

    // Verify it's recognized as an expr
    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(assignExprTest, testGetLeftAfterConstruction) {
    nInt lhs(123);
    nInt rhs(456);
    assignExpr expr(lhs, rhs);

    // Verify that getLeft returns the correct value immediately after construction
    const node& left = expr.getLeft();
    ASSERT_TRUE(left.isSub<nInt>());
    const nInt* intVal = left.cast<nInt>();
    ASSERT_TRUE(intVal != nullptr);
    ASSERT_EQ(intVal->get(), 123);
}

TEST_F(assignExprTest, testGetRightAfterConstruction) {
    nInt lhs(123);
    nInt rhs(789);
    assignExpr expr(lhs, rhs);

    // Verify that getRight returns the correct value immediately after construction
    const node& right = expr.getRight();
    ASSERT_TRUE(right.isSub<nInt>());
    const nInt* intVal = right.cast<nInt>();
    ASSERT_TRUE(intVal != nullptr);
    ASSERT_EQ(intVal->get(), 789);
}

TEST_F(assignExprTest, testWithLargeValues) {
    nInt lhs(999999);
    nInt rhs(888888);
    assignExpr expr(lhs, rhs);

    const node& left = expr.getLeft();
    const node& right = expr.getRight();
    ASSERT_EQ(left.cast<nInt>()->get(), 999999);
    ASSERT_EQ(right.cast<nInt>()->get(), 888888);
}

TEST_F(assignExprTest, testWithEmptyString) {
    nStr lhs("");
    nStr rhs("");
    assignExpr expr(lhs, rhs);

    const node& left = expr.getLeft();
    const node& right = expr.getRight();
    ASSERT_TRUE(left.isSub<nStr>());
    ASSERT_TRUE(right.isSub<nStr>());
}
