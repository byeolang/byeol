#include "test/byeolTest.hpp"
#include "core/ast/exprs/getGenericExpr.hpp"
#include "core/builtin/scalar/nInt.hpp"
#include "core/builtin/scalar/nStr.hpp"
#include "core/type/ntype.hpp"

using namespace by;
using namespace std;

struct getGenericExprTest: public byeolTest {};

TEST_F(getGenericExprTest, testConstructorWithNameAndTypeParams) {
    std::string genericName = "Array";
    args typeParams;
    getGenericExpr expr(genericName, typeParams);

    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(getGenericExprTest, testConstructorWithMeNameAndTypeParams) {
    nInt me(42);
    std::string genericName = "List";
    args typeParams;
    getGenericExpr expr(me, genericName, typeParams);

    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(getGenericExprTest, testGetNameInherited) {
    std::string genericName = "Vector";
    args typeParams;
    getGenericExpr expr(genericName, typeParams);

    const std::string& retrievedName = expr.getName();
    ASSERT_EQ(retrievedName, "Vector");
}

TEST_F(getGenericExprTest, testTypeInheritance) {
    std::string genericName = "Map";
    args typeParams;
    getGenericExpr expr(genericName, typeParams);

    // Should inherit from getExpr
    ASSERT_TRUE(expr.isSub<getExpr>());
    // Should inherit from expr
    ASSERT_TRUE(expr.isSub<by::expr>());
    // Should not be other types
    ASSERT_FALSE(expr.isSub<nInt>());
}

TEST_F(getGenericExprTest, testMultipleInstances) {
    std::string name1 = "Array";
    std::string name2 = "List";
    args typeParams1;
    args typeParams2;

    getGenericExpr expr1(name1, typeParams1);
    getGenericExpr expr2(name2, typeParams2);

    ASSERT_TRUE(expr1.isSub<getGenericExpr>());
    ASSERT_TRUE(expr2.isSub<getGenericExpr>());
    ASSERT_EQ(expr1.getName(), "Array");
    ASSERT_EQ(expr2.getName(), "List");
}

TEST_F(getGenericExprTest, testExprTypeCheck) {
    std::string genericName = "Set";
    args typeParams;
    getGenericExpr expr(genericName, typeParams);

    // Verify it's recognized as an expr
    ASSERT_TRUE(expr.isSub<by::expr>());
}

TEST_F(getGenericExprTest, testGetExprTypeCheck) {
    std::string genericName = "Queue";
    args typeParams;
    getGenericExpr expr(genericName, typeParams);

    // Verify it's recognized as a getExpr
    ASSERT_TRUE(expr.isSub<getExpr>());
}

TEST_F(getGenericExprTest, testGetGenericExprTypeCheck) {
    std::string genericName = "Stack";
    args typeParams;
    getGenericExpr expr(genericName, typeParams);

    // Verify it's specifically recognized as a getGenericExpr
    ASSERT_TRUE(expr.isSub<getGenericExpr>());
}

TEST_F(getGenericExprTest, testGetMeInherited) {
    nInt me(100);
    std::string genericName = "Container";
    args typeParams;
    getGenericExpr expr(me, genericName, typeParams);

    const node* retrievedMe = expr.getMe();
    ASSERT_TRUE(retrievedMe != nullptr);
    ASSERT_TRUE(retrievedMe->isSub<nInt>());
}

TEST_F(getGenericExprTest, testGetArgsInherited) {
    std::string genericName = "Pair";
    args typeParams;
    getGenericExpr expr(genericName, typeParams);

    const args& retrievedArgs = expr.getArgs();
    // Just verify we can retrieve args
    (void)retrievedArgs; // Intentionally unused - just testing the function works
}

TEST_F(getGenericExprTest, testInferInheritedNegative) {
    getGenericExpr expr("Optional", args());

    str inferredType = expr.infer();
    ASSERT_FALSE(inferredType);
}

TEST_F(getGenericExprTest, testInferInherited) {
    origin org(typeMaker::make<obj>("optional"));
    org.getOwns().add("age", *new getExpr("T"));
    genericOrigin genericOrg(org, {"T"});
    obj o;
    o.getOwns().add("optional", genericOrg);

    threadUse th1; {
        o.inFrame();

        getGenericExpr expr("optional", *new args{narr{*new nInt()}});
        str inferredType = expr.infer();
        ASSERT_TRUE(inferredType);
    }
}

TEST_F(getGenericExprTest, testEvalNegative) {
    args typeParams;
    getGenericExpr expr("Result", typeParams);
    args emptyArgs;

    str result = expr.eval(emptyArgs);
    ASSERT_FALSE(result);
}

TEST_F(getGenericExprTest, testEval) {
    origin org(typeMaker::make<obj>("optional"));
    org.getOwns().add("age", *new getExpr("T"));
    genericOrigin genericOrg(org, {"T"});
    obj o;
    o.getOwns().add("optional", genericOrg);

    threadUse th1; {
        o.inFrame();

        getGenericExpr expr("optional", *new args{narr{*new nInt()}});
        str inferredType = expr.eval();
        ASSERT_TRUE(inferredType);
    }
}

TEST_F(getGenericExprTest, testSetNameInherited) {
    std::string genericName1 = "Type1";
    args typeParams;
    getGenericExpr expr(genericName1, typeParams);

    std::string genericName2 = "Type2";
    expr.setName(genericName2);

    const std::string& retrievedName = expr.getName();
    ASSERT_EQ(retrievedName, "Type2");
}

TEST_F(getGenericExprTest, testSetMeInherited) {
    std::string genericName = "Generic";
    args typeParams;
    getGenericExpr expr(genericName, typeParams);

    nInt me(42);
    expr.setMe(me);

    const node* retrievedMe = expr.getMe();
    ASSERT_TRUE(retrievedMe != nullptr);
}

TEST_F(getGenericExprTest, testInferConsistency) {
    origin org(typeMaker::make<obj>("Template"));
    org.getOwns().add("age", *new getExpr("T"));
    genericOrigin genericOrg(org, {"T"});
    obj o;
    o.getOwns().add("Template", genericOrg);

    threadUse th1; {
        o.inFrame();
        getGenericExpr expr("Template", *new args{narr{*new nInt()}});

        str infer1 = expr.infer();
        str infer2 = expr.infer();

        ASSERT_TRUE(infer1);
        ASSERT_TRUE(infer2);
    }
}

TEST_F(getGenericExprTest, testWithEmptyName) {
    std::string genericName = "";
    args typeParams;
    getGenericExpr expr(genericName, typeParams);

    const std::string& retrievedName = expr.getName();
    ASSERT_EQ(retrievedName, "");
}

TEST_F(getGenericExprTest, testWithLongName) {
    std::string genericName = "VeryLongGenericTypeName";
    args typeParams;
    getGenericExpr expr(genericName, typeParams);

    const std::string& retrievedName = expr.getName();
    ASSERT_EQ(retrievedName, "VeryLongGenericTypeName");
}

TEST_F(getGenericExprTest, testConstructorWithMePreservesName) {
    nInt me(42);
    std::string genericName = "CustomType";
    args typeParams;
    getGenericExpr expr(me, genericName, typeParams);

    ASSERT_EQ(expr.getName(), "CustomType");
}

TEST_F(getGenericExprTest, testConstructorWithMePreservesMe) {
    nInt me(123);
    std::string genericName = "Type";
    args typeParams;
    getGenericExpr expr(me, genericName, typeParams);

    const node* retrievedMe = expr.getMe();
    ASSERT_TRUE(retrievedMe != nullptr);
    ASSERT_TRUE(retrievedMe->isSub<nInt>());
    const nInt* intVal = retrievedMe->cast<nInt>();
    ASSERT_TRUE(intVal != nullptr);
    ASSERT_EQ(intVal->get(), 123);
}

TEST_F(getGenericExprTest, testMultipleInstancesWithDifferentTypes) {
    nInt me1(10);
    nStr me2("object");
    args typeParams1;
    args typeParams2;

    getGenericExpr expr1(me1, "IntGeneric", typeParams1);
    getGenericExpr expr2(me2, "StrGeneric", typeParams2);

    ASSERT_TRUE(expr1.getMe()->isSub<nInt>());
    ASSERT_TRUE(expr2.getMe()->isSub<nStr>());
}

TEST_F(getGenericExprTest, testMultipleInstancesIndependence) {
    args typeParams1;
    args typeParams2;

    getGenericExpr expr1("Type1", typeParams1);
    getGenericExpr expr2("Type2", typeParams2);

    // Verify instances are independent
    ASSERT_NE(expr1.getName(), expr2.getName());
}

TEST_F(getGenericExprTest, testSetArgsInherited) {
    std::string genericName = "Generic";
    args typeParams;
    getGenericExpr expr(genericName, typeParams);

    args newArgs;
    expr.setArgs(newArgs);

    // Just verify it doesn't crash
}
