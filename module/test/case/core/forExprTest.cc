#include "test/byeolTest.hpp"
#include "core/ast/exprs/forExpr.hpp"
#include "core/ast/exprs/blockExpr.hpp"
#include "core/builtin/scalar/nInt.hpp"
#include "core/builtin/scalar/nStr.hpp"
#include "core/builtin/container/mgd/arr.hpp"

using namespace by;
using namespace std;

struct forExprTest: public byeolTest {};

TEST_F(forExprTest, testConstructor) {
    std::string localName = "i";
    arr container;
    blockExpr body;

    forExpr expr(localName, container, body);

    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(forExprTest, testGetLocalName) {
    std::string localName = "item";
    arr container;
    blockExpr body;

    forExpr expr(localName, container, body);

    const std::string& retrievedName = expr.getLocalName();
    ASSERT_EQ(retrievedName, "item");
}

TEST_F(forExprTest, testGetContainer) {
    std::string localName = "x";
    arr container;
    blockExpr body;

    forExpr expr(localName, container, body);

    str retrievedContainer = expr.getContainer();
    ASSERT_TRUE(retrievedContainer);
}

TEST_F(forExprTest, testTypeInheritance) {
    std::string localName = "i";
    arr container;
    blockExpr body;

    forExpr expr(localName, container, body);

    // Should inherit from loopExpr
    ASSERT_TRUE(expr.isSub<loopExpr>());
    // Should inherit from expr
    ASSERT_TRUE(expr.isSub<by::expr>());
    // Should not be other types
    ASSERT_FALSE(expr.isSub<nInt>());
}

TEST_F(forExprTest, testMultipleInstances) {
    std::string name1 = "i";
    std::string name2 = "j";
    arr container1;
    arr container2;
    blockExpr body1;
    blockExpr body2;

    forExpr expr1(name1, container1, body1);
    forExpr expr2(name2, container2, body2);

    ASSERT_TRUE(expr1.isSub<forExpr>());
    ASSERT_TRUE(expr2.isSub<forExpr>());
    ASSERT_EQ(expr1.getLocalName(), "i");
    ASSERT_EQ(expr2.getLocalName(), "j");
}

TEST_F(forExprTest, testExprTypeCheck) {
    std::string localName = "elem";
    arr container;
    blockExpr body;

    forExpr expr(localName, container, body);

    // Verify it's recognized as an expr
    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(forExprTest, testLoopExprTypeCheck) {
    std::string localName = "n";
    arr container;
    blockExpr body;

    forExpr expr(localName, container, body);

    // Verify it's recognized as a loopExpr
    ASSERT_TRUE(expr.isSub<loopExpr>());
}

TEST_F(forExprTest, testGetLocalNameAfterConstruction) {
    std::string localName = "element";
    arr container;
    blockExpr body;

    forExpr expr(localName, container, body);

    const std::string& retrievedName = expr.getLocalName();
    ASSERT_EQ(retrievedName, "element");
}

TEST_F(forExprTest, testWithEmptyLocalName) {
    std::string localName = "";
    arr container;
    blockExpr body;

    forExpr expr(localName, container, body);

    const std::string& retrievedName = expr.getLocalName();
    ASSERT_EQ(retrievedName, "");
}

TEST_F(forExprTest, testWithSingleCharLocalName) {
    std::string localName = "x";
    arr container;
    blockExpr body;

    forExpr expr(localName, container, body);

    const std::string& retrievedName = expr.getLocalName();
    ASSERT_EQ(retrievedName, "x");
}

TEST_F(forExprTest, testWithLongLocalName) {
    std::string localName = "iterationVariable";
    arr container;
    blockExpr body;

    forExpr expr(localName, container, body);

    const std::string& retrievedName = expr.getLocalName();
    ASSERT_EQ(retrievedName, "iterationVariable");
}

TEST_F(forExprTest, testWithBlockContainingStatements) {
    std::string localName = "i";
    arr container;
    nInt stmt1(10);
    nInt stmt2(20);
    blockExpr body(stmt1, stmt2);

    forExpr expr(localName, container, body);

    ASSERT_TRUE(expr.isSub<forExpr>());
}

TEST_F(forExprTest, testForExprTypeCheck) {
    std::string localName = "item";
    arr container;
    blockExpr body;

    forExpr expr(localName, container, body);

    // Verify it's specifically recognized as a forExpr
    ASSERT_TRUE(expr.isSub<forExpr>());
}

TEST_F(forExprTest, testMultipleForExprsWithDifferentNames) {
    arr container1;
    arr container2;
    blockExpr body1;
    blockExpr body2;

    forExpr expr1("alpha", container1, body1);
    forExpr expr2("beta", container2, body2);

    ASSERT_EQ(expr1.getLocalName(), "alpha");
    ASSERT_EQ(expr2.getLocalName(), "beta");
}

TEST_F(forExprTest, testGetContainerAfterConstruction) {
    std::string localName = "item";
    arr container;
    blockExpr body;

    forExpr expr(localName, container, body);

    str retrievedContainer = expr.getContainer();
    ASSERT_TRUE(retrievedContainer);
    ASSERT_TRUE(retrievedContainer->isSub<arr>());
}

TEST_F(forExprTest, testConstructorPreservesLocalName) {
    std::string localName = "myVariable";
    arr container;
    blockExpr body;

    forExpr expr(localName, container, body);

    ASSERT_EQ(expr.getLocalName(), localName);
}

TEST_F(forExprTest, testWithEmptyBlock) {
    std::string localName = "i";
    arr container;
    blockExpr body;

    forExpr expr(localName, container, body);

    ASSERT_TRUE(expr.isSub<forExpr>());
}

TEST_F(forExprTest, testMultipleInstancesIndependence) {
    arr container1;
    arr container2;
    blockExpr body1;
    blockExpr body2;

    forExpr expr1("var1", container1, body1);
    forExpr expr2("var2", container2, body2);

    // Verify instances are independent
    ASSERT_NE(expr1.getLocalName(), expr2.getLocalName());
}
