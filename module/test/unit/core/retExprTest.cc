#include "test/byeolTest.hpp"
#include "core/ast/exprs/retExpr.hpp"
#include "core/builtin/scalar/nInt.hpp"
#include "core/builtin/scalar/nStr.hpp"
#include "core/builtin/scalar/nVoid.hpp"
#include "core/type/ntype.hpp"

using namespace by;
using namespace std;

struct retExprTest: public byeolTest {};

TEST_F(retExprTest, testDefaultConstructor) {
    retExpr expr;

    // retExpr should be a valid expr
    ASSERT_TRUE(expr.isSub<by::expr>());
    ASSERT_TRUE(expr.isSub<retStateExpr>());
}

TEST_F(retExprTest, testConstructorWithNode) {
    nInt returnValue(42);
    retExpr expr(returnValue);

    // Should have valid return value
    const node& ret = expr.getRet();
    ASSERT_TRUE(ret.isSub<nInt>());
}

TEST_F(retExprTest, testGetRet) {
    nInt returnValue(100);
    retExpr expr(returnValue);

    const node& ret = expr.getRet();
    ASSERT_TRUE(ret.isSub<nInt>());

    const nInt* intVal = ret.cast<nInt>();
    ASSERT_TRUE(intVal != nullptr);
    ASSERT_EQ(intVal->get(), 100);
}

TEST_F(retExprTest, testGetRetNonConst) {
    nInt returnValue(50);
    retExpr expr(returnValue);

    node& ret = expr.getRet();
    ASSERT_TRUE(ret.isSub<nInt>());
}

TEST_F(retExprTest, testInfer) {
    nInt returnValue(42);
    retExpr expr(returnValue);

    threadUse th1;
    {
        obj o;
        o.inFrame();

        str inferredType = expr.infer();
        ASSERT_TRUE(inferredType);
    }
}

TEST_F(retExprTest, testInferWithDefaultConstructor) {
    retExpr expr;

    str inferredType = expr.infer();
    ASSERT_TRUE(inferredType);
}

TEST_F(retExprTest, testEvalWithEmptyArgsNegative) {
    nInt returnValue(42);
    retExpr expr(returnValue);
    args emptyArgs;

    setNoisyEx(false);
    str result = expr.eval(emptyArgs);
    setNoisyEx(true);
    ASSERT_FALSE(result);
}

TEST_F(retExprTest, testEvalWithEmptyArgs) {
    nInt returnValue(42);
    retExpr expr(returnValue);
    args emptyArgs;
    obj o;
    func foo(modifier(true, false), typeMaker::make<func>("foo", params(), new nVoid()));
    o.getShares().add("foo", foo);

    threadUse th1;
    {
        frameInteract i1(o);
        {
            frameInteract i2(foo);
            {
                str result = expr.eval(emptyArgs);
                ASSERT_TRUE(result);
            }
        }
    }
}

TEST_F(retExprTest, testPrioritize) {
    nInt returnValue(42);
    retExpr expr(returnValue);
    args emptyArgs;

    priorType priority = expr.prioritize(emptyArgs);
    ASSERT_EQ(priority, priorType::EXACT_MATCH); // nInt::prioritize() -> is nInt::ctor() callable?
}

TEST_F(retExprTest, testTypeInheritance) {
    retExpr expr;

    // Should inherit from retStateExpr
    ASSERT_TRUE(expr.isSub<retStateExpr>());
    // Should inherit from expr
    ASSERT_TRUE(expr.isSub<by::expr>());
    // Should not be other types
    ASSERT_FALSE(expr.isSub<nInt>());
}

TEST_F(retExprTest, testWithStringReturn) {
    nStr returnValue("hello");
    retExpr expr(returnValue);

    const node& ret = expr.getRet();
    ASSERT_TRUE(ret.isSub<nStr>());
}

TEST_F(retExprTest, testWithVoidReturn) {
    nVoid returnValue;
    retExpr expr(returnValue);

    const node& ret = expr.getRet();
    ASSERT_TRUE(ret.isSub<nVoid>());
}

TEST_F(retExprTest, testMultipleInstances) {
    nInt val1(10);
    nInt val2(20);
    retExpr expr1(val1);
    retExpr expr2(val2);

    ASSERT_TRUE(expr1.getRet().isSub<nInt>());
    ASSERT_TRUE(expr2.getRet().isSub<nInt>());

    const nInt* int1 = expr1.getRet().cast<nInt>();
    const nInt* int2 = expr2.getRet().cast<nInt>();
    ASSERT_TRUE(int1 != nullptr);
    ASSERT_TRUE(int2 != nullptr);
    ASSERT_EQ(int1->get(), 10);
    ASSERT_EQ(int2->get(), 20);
}

TEST_F(retExprTest, testInferConsistency) {
    nInt returnValue(42);
    retExpr expr(returnValue);

    // Multiple calls to infer should work
    str infer1 = expr.infer();
    str infer2 = expr.infer();

    ASSERT_TRUE(infer1);
    ASSERT_TRUE(infer2);
}
