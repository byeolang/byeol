#include "../../../namuSyntaxTest.hpp"

using namespace namu;
using namespace std;

namespace {
    struct getExprTest : public namuSyntaxTest {};
}

TEST_F(getExprTest, getSymbolOnPackScope) {
    make().parse(R"SRC(
        main() void
            main
    )SRC").shouldParsed(true);
    shouldVerified(true); // retType is void so implicit return won't work.
    scope& shares = (scope&) (((scopes&) getSlot().subs()).getNext().getContainer());
    ASSERT_FALSE(nul(shares));
    ASSERT_EQ(shares.len(), 2);
}

TEST_F(getExprTest, getSymbolOnPackScope1) {
    // control group.
    make().parse(R"SRC(
        age int
        main() int
            return 0
    )SRC").shouldVerified(true);
    scope& shares = (scope&) (((scopes&) getSlot().subs()).getNext().getContainer());
    ASSERT_FALSE(nul(shares));
    ASSERT_EQ(shares.len(), 2);
}

TEST_F(getExprTest, getSymbolOnPackScope2) {
    // experimental group.
    make().parse(R"SRC(
        age int
        main() int
            return age
    )SRC").shouldVerified(true);
    scope& shares = (scope&) (((scopes&) getSlot().subs()).getNext().getContainer());
    ASSERT_FALSE(nul(shares));
    ASSERT_EQ(shares.len(), 2);
    node& age = getSubPack().sub("age");
    ASSERT_FALSE(nul(age));
    nInt& cast = age.cast<nInt>();
    ASSERT_FALSE(nul(cast));
    ASSERT_EQ(cast.get(), 0); // default value of nInt
}

TEST_F(getExprTest, getSymbolOnPackScope3) {
    make().parse(R"SRC(
        age str
        main() int
            return age
    )SRC").shouldParsed(true);
    shouldVerified(false);
    scope& shares = (scope&) (((scopes&) getSlot().subs()).getNext().getContainer());
    ASSERT_FALSE(nul(shares));
    ASSERT_EQ(shares.len(), 2);
    node& age = getSubPack().sub("age");
    ASSERT_FALSE(nul(age));
    nStr& cast = age.cast<nStr>();
    ASSERT_FALSE(nul(cast));
    ASSERT_EQ(cast.get(), ""); // default value of nStr
}

TEST_F(getExprTest, getInvalidVariableNegative) {
    negative().make().parse(R"SRC(
        age str
        main() str
            return age1
    )SRC").shouldParsed(true);
    shouldVerified(false);
}

TEST_F(getExprTest, getInnerScopeVar) {
    make().parse(R"SRC(
        age int
        main() int
            age
            age int
            age
    )SRC").shouldVerified(true);
}

TEST_F(getExprTest, getInnerScopeVarNegative) {
    negative().make().parse(R"SRC(
        main() int
            age int
            age
            age int
            age
    )SRC").shouldParsed(true);
    shouldVerified(false);
}

struct myObj : public obj {
    NAMU(CLASS(myObj, obj))

public:
    myObj(): executed(false) {}

    str run(const args& a) override {
        executed = true;
        return super::run(a);
    }

    nbool executed;
};

TEST_F(getExprTest, getExprSkipEvalToPrimitiveObj) {
    myObj obj1;
    obj1.subs().add("age", new nInt(22));

    ASSERT_FALSE(obj1.executed);
    getExpr exp(obj1, "age");
    ASSERT_FALSE(obj1.executed);
}

struct myGetExpr : public getExpr {
    NAMU(CLASS(myGetExpr, getExpr, expr::exprType))

public:
    myGetExpr(const node& from, const std::string& name): super(from, name), executed(false) {}

    str run(const args& a) override {
        executed = true;
        return super::run(a);
    }

    nbool executed;
};

TEST_F(getExprTest, getExprEvalToExpr) {
    myObj obj1;
    obj1.subs().add("age", new nInt(22));
    myObj obj2;
    obj2.subs().add("obj1", obj1);
    myGetExpr exp(obj2, "obj1");
    getExpr exp2(exp, "age"); // == obj2.obj1.age

    ASSERT_FALSE(exp.executed);
    tstr<nInt> ret = exp2.run();
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret->get(), 22);
    ASSERT_TRUE(exp.executed);
    ASSERT_EQ(exp2.getEval().getType(), ttype<nInt>());
}
