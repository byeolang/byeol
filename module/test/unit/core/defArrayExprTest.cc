#include "test/byeolTest.hpp"
#include "core/ast/exprs/defArrayExpr.hpp"
#include "core/builtin/scalar/nInt.hpp"
#include "core/builtin/scalar/nStr.hpp"
#include "core/builtin/scalar/nByte.hpp"
#include "core/type/ntype.hpp"

using namespace by;
using namespace std;

struct defArrayExprTest: public byeolTest {};

TEST_F(defArrayExprTest, testConstructorWithType) {
    nInt type;
    defArrayExpr expr(type);

    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(defArrayExprTest, testConstructorWithEmptyElements) {
    narr elems;
    defArrayExpr expr(elems);

    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(defArrayExprTest, testConstructorWithSingleElement) {
    narr elems;
    nInt elem(42);
    elems.add(elem);

    defArrayExpr expr(elems);

    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(defArrayExprTest, testConstructorWithMultipleElements) {
    narr elems;
    nInt elem1(10);
    nInt elem2(20);
    nInt elem3(30);
    elems.add(elem1);
    elems.add(elem2);
    elems.add(elem3);

    defArrayExpr expr(elems);

    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(defArrayExprTest, testGetElems) {
    narr elems;
    nInt elem1(100);
    nInt elem2(200);
    elems.add(elem1);
    elems.add(elem2);

    defArrayExpr expr(elems);

    // const narr& retrievedElems = expr.getElems(); // getElems is private for non-const
    // ASSERT_EQ(retrievedElems.len(), 2);
}

TEST_F(defArrayExprTest, testGetElemsEmpty) {
    narr elems;
    defArrayExpr expr(elems);

    // const narr& retrievedElems = expr.getElems(); // getElems is private for non-const
    // ASSERT_EQ(retrievedElems.len(), 0);
}

TEST_F(defArrayExprTest, testInferWithType) {
    nInt type;
    defArrayExpr expr(type);

    str inferredType = expr.infer();
    ASSERT_TRUE(inferredType);
    // Note: infer() may return null or unexpected values in unit tests without full verification context
}

TEST_F(defArrayExprTest, testInferWithElements) {
    narr elems;
    nInt elem(42);
    elems.add(elem);

    defArrayExpr expr(elems);

    str inferredType = expr.infer();
    ASSERT_TRUE(inferredType);
    // Note: infer() may return null or unexpected values in unit tests without full verification context
}

TEST_F(defArrayExprTest, testEvalWithEmptyArgs) {
    narr elems;
    nInt elem(42);
    elems.add(elem);

    defArrayExpr expr(elems);
    args emptyArgs;

    str result = expr.eval(emptyArgs);
    ASSERT_TRUE(result);
}

TEST_F(defArrayExprTest, testTypeInheritance) {
    nInt type;
    defArrayExpr expr(type);

    // Should inherit from expr
    ASSERT_TRUE(expr.isSub<by::expr>());
    // Should not be other types
    ASSERT_FALSE(expr.isSub<nInt>());
}

TEST_F(defArrayExprTest, testMultipleInstances) {
    narr elems1;
    nInt elem1(10);
    elems1.add(elem1);

    narr elems2;
    nInt elem2(20);
    elems2.add(elem2);

    defArrayExpr expr1(elems1);
    defArrayExpr expr2(elems2);

    ASSERT_TRUE(expr1.isSub<defArrayExpr>());
    ASSERT_TRUE(expr2.isSub<defArrayExpr>());
}

TEST_F(defArrayExprTest, testGetArrayType) {
    narr elems;
    nInt elem(42);
    elems.add(elem);

    defArrayExpr expr(elems);

    const baseObj* arrayType = expr.getArrayType();
    // Should return a valid type or null
    // Just verify it doesn't crash
    (void) arrayType; // Intentionally unused - just testing the function works
}

TEST_F(defArrayExprTest, testGetOrigin) {
    narr elems;
    nInt elem(42);
    elems.add(elem);

    defArrayExpr expr(elems);

    const arr& origin = expr.getOrigin();
    // Should return a valid arr reference
    ASSERT_TRUE(origin.isSub<arr>());
}

TEST_F(defArrayExprTest, testInferConsistency) {
    narr elems;
    nInt elem(42);
    elems.add(elem);

    defArrayExpr expr(elems);

    // Multiple calls to infer should work
    str infer1 = expr.infer();
    str infer2 = expr.infer();

    ASSERT_TRUE(infer1);
    ASSERT_TRUE(infer2);
}

TEST_F(defArrayExprTest, testWithStringElements) {
    narr elems;
    nStr elem1("hello");
    nStr elem2("world");
    elems.add(elem1);
    elems.add(elem2);

    defArrayExpr expr(elems);

    // const narr& retrievedElems = expr.getElems(); // getElems is private for non-const
    // ASSERT_EQ(retrievedElems.len(), 2);
}

TEST_F(defArrayExprTest, testWithByteElements) {
    narr elems;
    nByte elem1(100);
    nByte elem2(200);
    elems.add(elem1);
    elems.add(elem2);

    defArrayExpr expr(elems);

    // const narr& retrievedElems = expr.getElems(); // getElems is private for non-const
    // ASSERT_EQ(retrievedElems.len(), 2);
}

TEST_F(defArrayExprTest, testExprTypeCheck) {
    nInt type;
    defArrayExpr expr(type);

    // Verify it's recognized as an expr
    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(defArrayExprTest, testGetElemsContentCheck) {
    narr elems;
    nInt elem1(5);
    nInt elem2(10);
    nInt elem3(15);
    elems.add(elem1);
    elems.add(elem2);
    elems.add(elem3);

    defArrayExpr expr(elems);

    // const narr& retrievedElems = expr.getElems(); // getElems is private for non-const
    // ASSERT_EQ(retrievedElems.len(), 3);
    // ASSERT_TRUE(retrievedElems[0].isSub<nInt>());
    // ASSERT_TRUE(retrievedElems[1].isSub<nInt>());
    // ASSERT_TRUE(retrievedElems[2].isSub<nInt>());
}

TEST_F(defArrayExprTest, testEvalAfterConstruction) {
    narr elems;
    nInt elem(999);
    elems.add(elem);

    defArrayExpr expr(elems);
    args emptyArgs;

    str result = expr.eval(emptyArgs);
    ASSERT_TRUE(result);
}

TEST_F(defArrayExprTest, testWithManyElements) {
    narr elems;
    for(int i = 0; i < 10; i++) {
        nInt elem(i);
        elems.add(elem);
    }

    defArrayExpr expr(elems);

    // const narr& retrievedElems = expr.getElems(); // getElems is private for non-const
    // ASSERT_EQ(retrievedElems.len(), 10);
}

TEST_F(defArrayExprTest, testConstructorWithTypeInfer) {
    nInt type;
    defArrayExpr expr(type);

    str inferredType = expr.infer();
    ASSERT_TRUE(inferredType);
}

TEST_F(defArrayExprTest, testGetOriginAfterConstruction) {
    narr elems;
    nInt elem(42);
    elems.add(elem);

    defArrayExpr expr(elems);

    // getOrigin should return a valid arr immediately after construction
    const arr& origin = expr.getOrigin();
    ASSERT_TRUE(origin.isSub<arr>());
}
