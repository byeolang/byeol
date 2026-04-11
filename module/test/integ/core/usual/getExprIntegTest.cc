#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct getExprIntegTest: public byeolIntegTest {};
}

TEST_F(getExprIntegTest, getSymbolOnPodScope) {
    make()
        .parse(R"SRC(
        main() void
            main
    )SRC")
        .shouldParsed(true);
    shouldVerified(true); // retType is void so implicit return won't work.
    scope::super& shares = (scope::super*) (getPod() TO(subs().getNext()) TO(getContainer())) OR_ASSERT(shares);
    ASSERT_EQ(shares.len(), 3);
}

TEST_F(getExprIntegTest, getSymbolOnPodScope1) {
    // control group.
    make()
        .parse(R"SRC(
        age int
        main() int
            ret 0
    )SRC")
        .shouldVerified(true);
    scope::super& shares = (scope::super*) (getPod() TO(subs().getNext()) TO(getContainer())) OR_ASSERT(shares);
    ASSERT_EQ(shares.len(), 3);
}

TEST_F(getExprIntegTest, getSymbolOnPodScope2) {
    // experimental group.
    make()
        .parse(R"SRC(
        age int
        main() int
            ret age
    )SRC")
        .shouldVerified(true);
    scope::super& shares = (scope::super*) (getPod() TO(subs().getNext()) TO(getContainer())) OR_ASSERT(shares);
    ASSERT_EQ(shares.len(), 3);
    node& age = getSubPod() TO(sub("age")) OR_ASSERT(age);
    nInt& cast = age.cast<nInt>() OR_ASSERT(cast);
    ASSERT_EQ(cast.get(), 0); // default value of nInt
}

TEST_F(getExprIntegTest, getSymbolOnPodScope3Negative) {
    make()
        .negative()
        .parse(R"SRC(
        age str
        main() int
            ret age
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
    scope::super& shares = (scope::super*) (getPod() TO(subs().getNext()) TO(getContainer())) OR_ASSERT(shares);
    ASSERT_EQ(shares.len(), 3);
    node& age = getSubPod() TO(sub("age")) OR_ASSERT(age);
    nStr& cast = age.cast<nStr>() OR_ASSERT(cast);
    ASSERT_EQ(cast.get(), ""); // default value of nStr
}

TEST_F(getExprIntegTest, getInvalidVariableNegative) {
    make()
        .negative()
        .parse(R"SRC(
        age str
        main() str
            ret age1
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(getExprIntegTest, getInnerScopeVar) {
    make()
        .parse(R"SRC(
        age int
        main() int
            age
            age int
            age
    )SRC")
        .shouldVerified(true);
}

TEST_F(getExprIntegTest, getInnerScopeVarNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            age int
            age
            age int
            age
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

struct myObj: public obj {
    BY(CLASS(myObj, obj))

public:
    myObj(): executed(false) {}

    str eval(const args& a) override {
        executed = true;
        return super::eval(a);
    }

    nbool executed;
};

TEST_F(getExprIntegTest, getExprSkipEvalTotscalar) {
    myObj obj1;
    obj1.subs().add("age", new nInt(22));

    ASSERT_FALSE(obj1.executed);
    getExpr exp(obj1, "age");
    ASSERT_FALSE(obj1.executed);
}

struct myGetExpr: public getExpr {
    BY(CLASS(myGetExpr, getExpr, expr::exprType))

public:
    myGetExpr(const node& from, const std::string& name): super(from, name), executed(false) {}

    str eval(const args& a) override {
        executed = true;
        return super::eval(a);
    }

    nbool executed;
};

TEST_F(getExprIntegTest, getExprEvalToExpr) {
    myObj obj1;
    obj1.subs().add("age", new nInt(22));
    myObj obj2;
    obj2.subs().add("obj1", obj1);
    myGetExpr exp(obj2, "obj1");
    getExpr exp2(exp, "age"); // == obj2.obj1.age

    ASSERT_FALSE(exp.executed);
    tstr<nInt> ret = exp2.eval();
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret->get(), 22);
    ASSERT_TRUE(exp.executed);
    ASSERT_EQ(exp2.infer()->getType(), ttype<nInt>());
}
