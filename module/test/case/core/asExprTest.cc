#include "test/byeolTest.hpp"
#include "core/ast/exprs/asExpr.hpp"
#include "core/builtin/scalar/nInt.hpp"
#include "core/builtin/scalar/nStr.hpp"
#include "core/builtin/scalar/nByte.hpp"
#include "core/type/ntype.hpp"

using namespace by;
using namespace std;

struct asExprTest: public byeolTest {};

TEST_F(asExprTest, testConstructor) {
    nInt value(42);
    nInt targetType;
    asExpr expr(value, targetType);

    ASSERT_TRUE(expr.getMe().isSub<nInt>());
}

TEST_F(asExprTest, testGetMe) {
    nInt value(100);
    nInt targetType;
    asExpr expr(value, targetType);

    const node& me = expr.getMe();
    ASSERT_TRUE(me.isSub<nInt>());
    ASSERT_EQ(me.cast<nInt>()->get(), 100);
}

TEST_F(asExprTest, testGetMeWithString) {
    nStr value("hello");
    nInt targetType;
    asExpr expr(value, targetType);

    const node& me = expr.getMe();
    ASSERT_TRUE(me.isSub<nStr>());
}

TEST_F(asExprTest, testGetAs) {
    nInt value(42);
    nInt targetType;
    asExpr expr(value, targetType);

    const node& as = expr.getAs();
}

TEST_F(asExprTest, testSetAs) {
    nInt value(42);
    nInt targetType1;
    asExpr expr(value, targetType1);

    nInt targetType2;
    expr.setAs(targetType2);

    const node& as = expr.getAs();
}

TEST_F(asExprTest, testSetAsPointer) {
    nInt value(42);
    nInt targetType1;
    asExpr expr(value, targetType1);

    nInt* targetType2 = new nInt();
    expr.setAs(*targetType2);
    delete targetType2;

    const node& as = expr.getAs();
}

TEST_F(asExprTest, testInfer) {
    nInt value(42);
    nInt targetType;
    asExpr expr(value, targetType);

    str inferredType = expr.infer();
    ASSERT_TRUE(inferredType);
    ASSERT_TRUE(inferredType->isSub<ntype>());
}

TEST_F(asExprTest, testEvalWithEmptyArgs) {
    nInt value(42);
    nInt targetType;
    asExpr expr(value, targetType);
    args emptyArgs;

    str result = expr.eval(emptyArgs);
    // eval should return a result
    ASSERT_TRUE(result);
}

TEST_F(asExprTest, testTypeInheritance) {
    nInt value(42);
    nInt targetType;
    asExpr expr(value, targetType);

    // Should inherit from expr
    ASSERT_TRUE(expr.isSub<by::expr>());
    // Should not be other types
    ASSERT_FALSE(expr.isSub<nInt>());
}

TEST_F(asExprTest, testMultipleInstances) {
    nInt value1(10);
    nInt value2(20);
    nInt targetType1;
    nInt targetType2;

    asExpr expr1(value1, targetType1);
    asExpr expr2(value2, targetType2);

    ASSERT_TRUE(expr1.getMe().isSub<nInt>());
    ASSERT_TRUE(expr2.getMe().isSub<nInt>());
    ASSERT_EQ(expr1.getMe().cast<nInt>()->get(), 10);
    ASSERT_EQ(expr2.getMe().cast<nInt>()->get(), 20);
}

TEST_F(asExprTest, testDifferentValueTypes) {
    nInt intValue(42);
    nInt targetType1;
    asExpr intExpr(intValue, targetType1);
    ASSERT_TRUE(intExpr.getMe().isSub<scalar>());

    nStr strValue("test");
    nInt targetType2;
    asExpr strExpr(strValue, targetType2);
    ASSERT_TRUE(strExpr.getMe().isSub<scalar>());

    nByte byteValue(255);
    nInt targetType3;
    asExpr byteExpr(byteValue, targetType3);
    ASSERT_TRUE(byteExpr.getMe().isSub<scalar>());
}

TEST_F(asExprTest, testInferConsistency) {
    nInt value(42);
    nInt targetType;
    asExpr expr(value, targetType);

    // Multiple calls to infer should work
    str infer1 = expr.infer();
    str infer2 = expr.infer();

    ASSERT_TRUE(infer1);
    ASSERT_TRUE(infer2);
}

TEST_F(asExprTest, testGetMeAfterConstruction) {
    nInt value(123);
    nInt targetType;
    asExpr expr(value, targetType);

    // Verify that getMe returns the correct value immediately after construction
    const node& me = expr.getMe();
    ASSERT_TRUE(me.isSub<nInt>());
    const nInt* intVal = me.cast<nInt>();
    ASSERT_TRUE(intVal != nullptr);
    ASSERT_EQ(intVal->get(), 123);
}

TEST_F(asExprTest, testGetAsAfterConstruction) {
    nInt value(42);
    nInt targetType;
    asExpr expr(value, targetType);

    // Verify that getAs returns a valid type immediately after construction
    const node& as = expr.getAs();
}

TEST_F(asExprTest, testSetAsMultipleTimes) {
    nInt value(42);
    nInt targetType1;
    asExpr expr(value, targetType1);

    nInt targetType2;
    expr.setAs(targetType2);

    nInt targetType3;
    expr.setAs(targetType3);
}

TEST_F(asExprTest, testExprTypeCheck) {
    nInt value(42);
    nInt targetType;
    asExpr expr(value, targetType);

    // Verify it's recognized as an expr
    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(asExprTest, testWithZeroValue) {
    nInt value(0);
    nInt targetType;
    asExpr expr(value, targetType);

    const node& me = expr.getMe();
    ASSERT_TRUE(me.isSub<nInt>());
    ASSERT_EQ(me.cast<nInt>()->get(), 0);
}

TEST_F(asExprTest, testWithNegativeValue) {
    nInt value(-50);
    nInt targetType;
    asExpr expr(value, targetType);

    const node& me = expr.getMe();
    ASSERT_TRUE(me.isSub<nInt>());
    ASSERT_EQ(me.cast<nInt>()->get(), -50);
}

TEST_F(asExprTest, testWithEmptyString) {
    nStr value("");
    nInt targetType;
    asExpr expr(value, targetType);

    const node& me = expr.getMe();
    ASSERT_TRUE(me.isSub<nStr>());
}
