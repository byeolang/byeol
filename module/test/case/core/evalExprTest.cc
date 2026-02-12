#include "test/byeolTest.hpp"
#include "core/ast/exprs/evalExpr.hpp"
#include "core/builtin/scalar/nInt.hpp"
#include "core/builtin/scalar/nStr.hpp"
#include "core/type/ntype.hpp"

using namespace by;
using namespace std;

struct evalExprTest: public byeolTest {};

TEST_F(evalExprTest, testConstructorWithSubject) {
    nInt meObj(42);
    nStr subject("function");
    args emptyArgs;

    evalExpr expr(&meObj, subject, emptyArgs);

    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(evalExprTest, testConstructorWithoutSubject) {
    nInt meObj(42);
    args emptyArgs;

    evalExpr expr(&meObj, emptyArgs);

    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(evalExprTest, testGetMe) {
    nInt meObj(100);
    args emptyArgs;

    evalExpr expr(&meObj, emptyArgs);

    const node* me = expr.getMe();
    ASSERT_TRUE(me != nullptr);
    ASSERT_TRUE(me->isSub<nInt>());
}

TEST_F(evalExprTest, testGetMeNonConst) {
    nInt meObj(100);
    args emptyArgs;

    evalExpr expr(&meObj, emptyArgs);

    node* me = expr.getMe();
    ASSERT_TRUE(me != nullptr);
    ASSERT_TRUE(me->isSub<nInt>());
}

TEST_F(evalExprTest, testSetMe) {
    nInt meObj1(10);
    args emptyArgs;

    evalExpr expr(&meObj1, emptyArgs);

    nInt meObj2(20);
    expr.setMe(meObj2);

    const node* me = expr.getMe();
    ASSERT_TRUE(me != nullptr);
}

TEST_F(evalExprTest, testSetMePointer) {
    nInt meObj1(10);
    args emptyArgs;

    evalExpr expr(&meObj1, emptyArgs);

    nInt* meObj2 = new nInt(30);
    expr.setMe(meObj2);

    const node* me = expr.getMe();
    ASSERT_TRUE(me != nullptr);
}

TEST_F(evalExprTest, testGetSubj) {
    nInt meObj(42);
    nStr subject("testFunc");
    args emptyArgs;

    evalExpr expr(&meObj, subject, emptyArgs);

    const node& subj = expr.getSubj();
    ASSERT_TRUE(subj.isSub<nStr>());
}

TEST_F(evalExprTest, testGetSubjNonConst) {
    nInt meObj(42);
    nStr subject("testFunc");
    args emptyArgs;

    evalExpr expr(&meObj, subject, emptyArgs);

    node& subj = expr.getSubj();
    ASSERT_TRUE(subj.isSub<nStr>());
}

TEST_F(evalExprTest, testSetSubj) {
    nInt meObj(42);
    nStr subject1("func1");
    args emptyArgs;

    evalExpr expr(&meObj, subject1, emptyArgs);

    nStr subject2("func2");
    expr.setSubj(subject2);

    const node& subj = expr.getSubj();
    ASSERT_TRUE(subj.isSub<nStr>());
}

TEST_F(evalExprTest, testSetSubjPointer) {
    nInt meObj(42);
    nStr subject1("func1");
    args emptyArgs;

    evalExpr expr(&meObj, subject1, emptyArgs);

    nStr* subject2 = new nStr("func2");
    expr.setSubj(subject2);

    const node& subj = expr.getSubj();
    ASSERT_TRUE(subj.isSub<nStr>());
}

TEST_F(evalExprTest, testGetArgs) {
    nInt meObj(42);
    args testArgs;

    evalExpr expr(&meObj, testArgs);

    const args& retrievedArgs = expr.getArgs();
    // Just verify we can retrieve args
}

TEST_F(evalExprTest, testGetArgsNonConst) {
    nInt meObj(42);
    args testArgs;

    evalExpr expr(&meObj, testArgs);

    args& retrievedArgs = expr.getArgs();
    // Just verify we can retrieve args
}

TEST_F(evalExprTest, testInfer) {
    nInt meObj(42);
    args emptyArgs;

    evalExpr expr(&meObj, emptyArgs);

    str inferredType = expr.infer();
    ASSERT_TRUE(inferredType);
    ASSERT_TRUE(inferredType->isSub<ntype>());
}

TEST_F(evalExprTest, testEvalWithEmptyArgs) {
    nInt meObj(42);
    args emptyArgs;

    evalExpr expr(&meObj, emptyArgs);

    str result = expr.eval(emptyArgs);
    // eval should return a result
    ASSERT_TRUE(result);
}

TEST_F(evalExprTest, testTypeInheritance) {
    nInt meObj(42);
    args emptyArgs;

    evalExpr expr(&meObj, emptyArgs);

    // Should inherit from expr
    ASSERT_TRUE(expr.isSub<by::expr>());
    // Should not be other types
    ASSERT_FALSE(expr.isSub<nInt>());
}

TEST_F(evalExprTest, testMultipleInstances) {
    nInt meObj1(10);
    nInt meObj2(20);
    args args1;
    args args2;

    evalExpr expr1(&meObj1, args1);
    evalExpr expr2(&meObj2, args2);

    ASSERT_TRUE(expr1.isSub<evalExpr>());
    ASSERT_TRUE(expr2.isSub<evalExpr>());
}

TEST_F(evalExprTest, testInferConsistency) {
    nInt meObj(42);
    args emptyArgs;

    evalExpr expr(&meObj, emptyArgs);

    // Multiple calls to infer should work
    str infer1 = expr.infer();
    str infer2 = expr.infer();

    ASSERT_TRUE(infer1);
    ASSERT_TRUE(infer2);
}

TEST_F(evalExprTest, testExprTypeCheck) {
    nInt meObj(42);
    args emptyArgs;

    evalExpr expr(&meObj, emptyArgs);

    // Verify it's recognized as an expr
    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(evalExprTest, testWithNullMe) {
    args emptyArgs;
    nStr subject("func");

    evalExpr expr(nullptr, subject, emptyArgs);

    ASSERT_TRUE(expr.isSub<evalExpr>());
}

TEST_F(evalExprTest, testGetMeAfterConstruction) {
    nInt meObj(123);
    args emptyArgs;

    evalExpr expr(&meObj, emptyArgs);

    const node* me = expr.getMe();
    ASSERT_TRUE(me != nullptr);
    ASSERT_TRUE(me->isSub<nInt>());
    const nInt* intVal = me->cast<nInt>();
    ASSERT_TRUE(intVal != nullptr);
    ASSERT_EQ(intVal->get(), 123);
}

TEST_F(evalExprTest, testGetSubjAfterConstruction) {
    nInt meObj(42);
    nStr subject("testFunction");
    args emptyArgs;

    evalExpr expr(&meObj, subject, emptyArgs);

    const node& subj = expr.getSubj();
    ASSERT_TRUE(subj.isSub<nStr>());
}

TEST_F(evalExprTest, testSetMeMultipleTimes) {
    nInt meObj1(10);
    args emptyArgs;

    evalExpr expr(&meObj1, emptyArgs);

    nInt meObj2(20);
    expr.setMe(meObj2);
    ASSERT_TRUE(expr.getMe() != nullptr);

    nInt meObj3(30);
    expr.setMe(meObj3);
    ASSERT_TRUE(expr.getMe() != nullptr);
}

TEST_F(evalExprTest, testSetSubjMultipleTimes) {
    nInt meObj(42);
    nStr subject1("func1");
    args emptyArgs;

    evalExpr expr(&meObj, subject1, emptyArgs);

    nStr subject2("func2");
    expr.setSubj(subject2);
    ASSERT_TRUE(expr.getSubj().isSub<nStr>());

    nStr subject3("func3");
    expr.setSubj(subject3);
    ASSERT_TRUE(expr.getSubj().isSub<nStr>());
}

TEST_F(evalExprTest, testEvalAfterConstruction) {
    nInt meObj(999);
    args emptyArgs;

    evalExpr expr(&meObj, emptyArgs);

    str result = expr.eval(emptyArgs);
    ASSERT_TRUE(result);
}
