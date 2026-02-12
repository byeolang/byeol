#include "test/byeolTest.hpp"
#include "core/ast/exprs/defSeqExpr.hpp"
#include "core/builtin/scalar/nInt.hpp"
#include "core/builtin/scalar/nByte.hpp"
#include "core/type/ntype.hpp"

using namespace by;
using namespace std;

struct defSeqExprTest: public byeolTest {};

TEST_F(defSeqExprTest, testConstructor) {
    nInt start(0);
    nInt end(10);
    defSeqExpr expr(start, end);

    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(defSeqExprTest, testGetStart) {
    nInt start(5);
    nInt end(15);
    defSeqExpr expr(start, end);

    const node& retrievedStart = expr.getStart();
    ASSERT_TRUE(retrievedStart.isSub<nInt>());
    ASSERT_EQ(retrievedStart.cast<nInt>()->get(), 5);
}

TEST_F(defSeqExprTest, testGetEnd) {
    nInt start(10);
    nInt end(100);
    defSeqExpr expr(start, end);

    const node& retrievedEnd = expr.getEnd();
    ASSERT_TRUE(retrievedEnd.isSub<nInt>());
    ASSERT_EQ(retrievedEnd.cast<nInt>()->get(), 100);
}

TEST_F(defSeqExprTest, testInfer) {
    nInt start(1);
    nInt end(10);
    defSeqExpr expr(start, end);

    str inferredType = expr.infer();
    ASSERT_TRUE(inferredType);
    ASSERT_TRUE(inferredType->isSub<ntype>());
}

TEST_F(defSeqExprTest, testEvalWithEmptyArgs) {
    nInt start(1);
    nInt end(10);
    defSeqExpr expr(start, end);
    args emptyArgs;

    str result = expr.eval(emptyArgs);
    ASSERT_TRUE(result);
}

TEST_F(defSeqExprTest, testTypeInheritance) {
    nInt start(0);
    nInt end(5);
    defSeqExpr expr(start, end);

    // Should inherit from expr
    ASSERT_TRUE(expr.isSub<by::expr>());
    // Should not be other types
    ASSERT_FALSE(expr.isSub<nInt>());
}

TEST_F(defSeqExprTest, testMultipleInstances) {
    nInt start1(0);
    nInt end1(10);
    nInt start2(20);
    nInt end2(30);

    defSeqExpr expr1(start1, end1);
    defSeqExpr expr2(start2, end2);

    ASSERT_TRUE(expr1.isSub<defSeqExpr>());
    ASSERT_TRUE(expr2.isSub<defSeqExpr>());
    ASSERT_EQ(expr1.getStart().cast<nInt>()->get(), 0);
    ASSERT_EQ(expr2.getStart().cast<nInt>()->get(), 20);
}

TEST_F(defSeqExprTest, testWithZeroRange) {
    nInt start(0);
    nInt end(0);
    defSeqExpr expr(start, end);

    const node& retrievedStart = expr.getStart();
    const node& retrievedEnd = expr.getEnd();
    ASSERT_EQ(retrievedStart.cast<nInt>()->get(), 0);
    ASSERT_EQ(retrievedEnd.cast<nInt>()->get(), 0);
}

TEST_F(defSeqExprTest, testWithNegativeRange) {
    nInt start(-10);
    nInt end(-5);
    defSeqExpr expr(start, end);

    const node& retrievedStart = expr.getStart();
    const node& retrievedEnd = expr.getEnd();
    ASSERT_EQ(retrievedStart.cast<nInt>()->get(), -10);
    ASSERT_EQ(retrievedEnd.cast<nInt>()->get(), -5);
}

TEST_F(defSeqExprTest, testWithReverseRange) {
    nInt start(10);
    nInt end(5);
    defSeqExpr expr(start, end);

    const node& retrievedStart = expr.getStart();
    const node& retrievedEnd = expr.getEnd();
    ASSERT_EQ(retrievedStart.cast<nInt>()->get(), 10);
    ASSERT_EQ(retrievedEnd.cast<nInt>()->get(), 5);
}

TEST_F(defSeqExprTest, testInferConsistency) {
    nInt start(1);
    nInt end(10);
    defSeqExpr expr(start, end);

    // Multiple calls to infer should work
    str infer1 = expr.infer();
    str infer2 = expr.infer();

    ASSERT_TRUE(infer1);
    ASSERT_TRUE(infer2);
}

TEST_F(defSeqExprTest, testWithByteRange) {
    nByte start(0);
    nByte end(255);
    defSeqExpr expr(start, end);

    const node& retrievedStart = expr.getStart();
    const node& retrievedEnd = expr.getEnd();
    ASSERT_TRUE(retrievedStart.isSub<nByte>());
    ASSERT_TRUE(retrievedEnd.isSub<nByte>());
}

TEST_F(defSeqExprTest, testExprTypeCheck) {
    nInt start(1);
    nInt end(10);
    defSeqExpr expr(start, end);

    // Verify it's recognized as an expr
    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(defSeqExprTest, testGetStartAfterConstruction) {
    nInt start(42);
    nInt end(100);
    defSeqExpr expr(start, end);

    // Verify that getStart returns the correct value immediately after construction
    const node& retrievedStart = expr.getStart();
    ASSERT_TRUE(retrievedStart.isSub<nInt>());
    const nInt* intVal = retrievedStart.cast<nInt>();
    ASSERT_TRUE(intVal != nullptr);
    ASSERT_EQ(intVal->get(), 42);
}

TEST_F(defSeqExprTest, testGetEndAfterConstruction) {
    nInt start(1);
    nInt end(999);
    defSeqExpr expr(start, end);

    // Verify that getEnd returns the correct value immediately after construction
    const node& retrievedEnd = expr.getEnd();
    ASSERT_TRUE(retrievedEnd.isSub<nInt>());
    const nInt* intVal = retrievedEnd.cast<nInt>();
    ASSERT_TRUE(intVal != nullptr);
    ASSERT_EQ(intVal->get(), 999);
}

TEST_F(defSeqExprTest, testWithLargeRange) {
    nInt start(0);
    nInt end(1000000);
    defSeqExpr expr(start, end);

    const node& retrievedStart = expr.getStart();
    const node& retrievedEnd = expr.getEnd();
    ASSERT_EQ(retrievedStart.cast<nInt>()->get(), 0);
    ASSERT_EQ(retrievedEnd.cast<nInt>()->get(), 1000000);
}

TEST_F(defSeqExprTest, testBothScalarTypes) {
    nInt start(1);
    nInt end(10);
    defSeqExpr expr(start, end);

    ASSERT_TRUE(expr.getStart().isSub<scalar>());
    ASSERT_TRUE(expr.getEnd().isSub<scalar>());
}

TEST_F(defSeqExprTest, testEvalAfterConstruction) {
    nInt start(5);
    nInt end(15);
    defSeqExpr expr(start, end);
    args emptyArgs;

    str result = expr.eval(emptyArgs);
    ASSERT_TRUE(result);
}

TEST_F(defSeqExprTest, testInferAfterConstruction) {
    nInt start(1);
    nInt end(100);
    defSeqExpr expr(start, end);

    str inferredType = expr.infer();
    ASSERT_TRUE(inferredType);
}

TEST_F(defSeqExprTest, testSingleElementRange) {
    nInt start(5);
    nInt end(6);
    defSeqExpr expr(start, end);

    const node& retrievedStart = expr.getStart();
    const node& retrievedEnd = expr.getEnd();
    ASSERT_EQ(retrievedStart.cast<nInt>()->get(), 5);
    ASSERT_EQ(retrievedEnd.cast<nInt>()->get(), 6);
}
