#include "test/byeolTest.hpp"
#include "core/ast/exprs/getExpr.hpp"
#include "core/builtin/scalar/nInt.hpp"
#include "core/builtin/scalar/nStr.hpp"
#include "core/type/ntype.hpp"

using namespace by;
using namespace std;

struct getExprTest: public byeolTest {};

TEST_F(getExprTest, testConstructorWithName) {
    std::string name = "property";
    getExpr expr(name);

    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(getExprTest, testConstructorWithNameAndArgs) {
    std::string name = "method";
    args emptyArgs;
    getExpr expr(name, emptyArgs);

    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(getExprTest, testConstructorWithMeAndName) {
    nInt me(42);
    std::string name = "value";
    getExpr expr(me, name);

    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(getExprTest, testConstructorWithMeNameAndArgs) {
    nInt me(42);
    std::string name = "calculate";
    args emptyArgs;
    getExpr expr(me, name, emptyArgs);

    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(getExprTest, testGetName) {
    std::string name = "testProperty";
    getExpr expr(name);

    const std::string& retrievedName = expr.getName();
    ASSERT_EQ(retrievedName, "testProperty");
}

TEST_F(getExprTest, testSetName) {
    std::string name1 = "prop1";
    getExpr expr(name1);

    std::string name2 = "prop2";
    expr.setName(name2);

    const std::string& retrievedName = expr.getName();
    ASSERT_EQ(retrievedName, "prop2");
}

TEST_F(getExprTest, testSetNamePointer) {
    std::string name1 = "method1";
    getExpr expr(name1);

    std::string* name2 = new std::string("method2");
    expr.setName(name2);

    const std::string& retrievedName = expr.getName();
    ASSERT_EQ(retrievedName, "method2");
    delete name2;
}

TEST_F(getExprTest, testGetMe) {
    nInt me(100);
    std::string name = "value";
    getExpr expr(me, name);

    const node* retrievedMe = expr.getMe();
    ASSERT_TRUE(retrievedMe != nullptr);
    ASSERT_TRUE(retrievedMe->isSub<nInt>());
}

TEST_F(getExprTest, testGetMeNull) {
    std::string name = "property";
    getExpr expr(name);

    const node* retrievedMe = expr.getMe();
    // May be null when constructed without me
    (void)retrievedMe; // Intentionally unused - just testing the function works
}

TEST_F(getExprTest, testSetMe) {
    std::string name = "property";
    getExpr expr(name);

    nInt me(42);
    expr.setMe(me);

    const node* retrievedMe = expr.getMe();
    ASSERT_TRUE(retrievedMe != nullptr);
}

TEST_F(getExprTest, testGetArgs) {
    std::string name = "method";
    args testArgs;
    getExpr expr(name, testArgs);

    const args& retrievedArgs = expr.getArgs();
    // Just verify we can retrieve args
    (void)retrievedArgs; // Intentionally unused - just testing the function works
}

TEST_F(getExprTest, testSetArgs) {
    std::string name = "method";
    getExpr expr(name);

    args testArgs;
    expr.setArgs(testArgs);

    const args& retrievedArgs = expr.getArgs();
    // Just verify we can retrieve args after setting
    (void)retrievedArgs; // Intentionally unused - just testing the function works
}

TEST_F(getExprTest, testInferNegative) {
    std::string name = "property";
    getExpr expr(name);

    str inferredType = expr.infer();
    ASSERT_FALSE(inferredType);
}

TEST_F(getExprTest, testInfer) {
    obj o;
    std::string name = "property";
    o.getOwns().add(name, new nInt(77));

    threadUse th1; {
        o.inFrame();
        getExpr expr(name);
        str inferredType = expr.infer();
        ASSERT_TRUE(inferredType);
        ASSERT_TRUE(inferredType->isSub<nInt>());
    }
}

TEST_F(getExprTest, testEvalNegative) {
    std::string name = "property";
    getExpr expr(name);
    args emptyArgs;

    str result = expr.eval(emptyArgs);
    ASSERT_FALSE(result);
}

TEST_F(getExprTest, testEval) {
    std::string name = "property";
    obj o;
    o.getOwns().add(name, new nInt(77));

    threadUse th1; {
        o.inFrame();
        getExpr expr(name);
        str inferredType = expr.eval(args());
        ASSERT_TRUE(inferredType);
        ASSERT_TRUE(inferredType->isSub<nInt>());
        ASSERT_EQ(*inferredType->cast<nint>(), 77);
    }
}

TEST_F(getExprTest, testTypeInheritance) {
    std::string name = "property";
    getExpr expr(name);

    // Should inherit from expr
    ASSERT_TRUE(expr.isSub<by::expr>());
    // Should not be other types
    ASSERT_FALSE(expr.isSub<nInt>());
}

TEST_F(getExprTest, testMultipleInstances) {
    std::string name1 = "prop1";
    std::string name2 = "prop2";

    getExpr expr1(name1);
    getExpr expr2(name2);

    ASSERT_TRUE(expr1.isSub<getExpr>());
    ASSERT_TRUE(expr2.isSub<getExpr>());
    ASSERT_EQ(expr1.getName(), "prop1");
    ASSERT_EQ(expr2.getName(), "prop2");
}

TEST_F(getExprTest, testInferConsistency) {
    obj o;
    std::string name = "property";
    o.getOwns().add(name, new nInt(77));

    threadUse th1; {
        o.inFrame();
        getExpr expr(name);

        // Multiple calls to infer should work
        str infer1 = expr.infer();
        str infer2 = expr.infer();

        ASSERT_TRUE(infer1);
        ASSERT_TRUE(infer2);
        ASSERT_EQ(infer1->getType(), infer2->getType());
    }
}

TEST_F(getExprTest, testExprTypeCheck) {
    std::string name = "property";
    getExpr expr(name);

    // Verify it's recognized as an expr
    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(getExprTest, testGetNameAfterConstruction) {
    std::string name = "myProperty";
    getExpr expr(name);

    const std::string& retrievedName = expr.getName();
    ASSERT_EQ(retrievedName, "myProperty");
}

TEST_F(getExprTest, testSetNameMultipleTimes) {
    std::string name1 = "prop1";
    getExpr expr(name1);

    expr.setName("prop2");
    ASSERT_EQ(expr.getName(), "prop2");

    expr.setName("prop3");
    ASSERT_EQ(expr.getName(), "prop3");
}

TEST_F(getExprTest, testSetMeMultipleTimes) {
    std::string name = "property";
    getExpr expr(name);

    nInt me1(10);
    expr.setMe(me1);
    ASSERT_TRUE(expr.getMe() != nullptr);

    nInt me2(20);
    expr.setMe(me2);
    ASSERT_TRUE(expr.getMe() != nullptr);
}

TEST_F(getExprTest, testWithEmptyName) {
    std::string name = "";
    getExpr expr(name);

    const std::string& retrievedName = expr.getName();
    ASSERT_EQ(retrievedName, "");
}

TEST_F(getExprTest, testWithLongName) {
    std::string name = "veryLongPropertyNameForTesting";
    getExpr expr(name);

    const std::string& retrievedName = expr.getName();
    ASSERT_EQ(retrievedName, "veryLongPropertyNameForTesting");
}

TEST_F(getExprTest, testGetMeAfterSetMe) {
    std::string name = "value";
    getExpr expr(name);

    nInt me(123);
    expr.setMe(me);

    const node* retrievedMe = expr.getMe();
    ASSERT_TRUE(retrievedMe != nullptr);
    ASSERT_TRUE(retrievedMe->isSub<nInt>());
    const nInt* intVal = retrievedMe->cast<nInt>();
    ASSERT_TRUE(intVal != nullptr);
    ASSERT_EQ(intVal->get(), 123);
}

TEST_F(getExprTest, testConstructorWithStringMe) {
    nStr me("object");
    std::string name = "length";
    getExpr expr(me, name);

    const node* retrievedMe = expr.getMe();
    ASSERT_TRUE(retrievedMe != nullptr);
    ASSERT_TRUE(retrievedMe->isSub<nStr>());
}

TEST_F(getExprTest, testSetArgsMultipleTimes) {
    std::string name = "method";
    getExpr expr(name);

    args args1;
    expr.setArgs(args1);
    // Just verify it doesn't crash

    args args2;
    expr.setArgs(args2);
    // Just verify it doesn't crash
}

TEST_F(getExprTest, testGetExprTypeCheck) {
    std::string name = "property";
    getExpr expr(name);

    // Verify it's specifically recognized as a getExpr
    ASSERT_TRUE(expr.isSub<getExpr>());
}
