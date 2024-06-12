#include "../../../namuSyntaxTest.hpp"

using namespace namu;
using namespace std;

namespace {
    struct defPropExprTest : public namuSyntaxTest {};
}

TEST_F(defPropExprTest, simpleDefineVariable) {
    if(make().parse(R"SRC(
        main() void
            age int
            ret
    )SRC").shouldVerified(true)) {
        node& res = getSubPack();
        ASSERT_FALSE(nul(res));

        const func& f = res.sub<func>("main", narr());
        ASSERT_FALSE(nul(f));
        const narr& stmts = f.getBlock().getStmts();
        ASSERT_FALSE(nul(stmts));
        ASSERT_EQ(stmts.len(), 2);
        const defPropExpr& defProp = stmts[0].cast<defPropExpr>();
        ASSERT_FALSE(nul(defProp));
        ASSERT_EQ(defProp.getName(), "age");
        ASSERT_EQ(defProp.getOrigin().getType(), ttype<nInt>());
    }
}

TEST_F(defPropExprTest, definePackVariableNegative) {
    negative().make().parse(R"SRC(
        name str
        age int
        grade flt
        main() void
            age int
            ret age
    )SRC").shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defPropExprTest, definePackVariable2) {
    make().parse(R"SRC(
        name str
        age int
        grade flt
        main() int
            age int
            ret age
    )SRC").shouldParsed(true);
    shouldVerified(true);
    slot& s = getSlot();
    ASSERT_EQ(s.getManifest().name, manifest::DEFAULT_NAME);

    auto& owns = ((scope&) getSlot().subs()).getContainer();
    auto& shares = ((scope&) getSlot().subs()).getNext().getContainer();
    ASSERT_FALSE(nul(owns));
    ASSERT_FALSE(nul(shares));
    ASSERT_EQ(owns.len(), 3);
    ASSERT_EQ(shares.len(), 3);
    ASSERT_EQ(owns.getAll<baseObj>().len(), 3);
    ASSERT_EQ(s.subAll<func>().len(), 1);

    nStr& name = s.sub<nStr>("name");
    ASSERT_FALSE(nul(name));
    ASSERT_EQ(name.get(), "");

    nInt& age = s.sub<nInt>("age");
    ASSERT_FALSE(nul(age));
    ASSERT_EQ(age.get(), 0); // 0 is default value.

    nFlt& grade = s.sub<nFlt>("grade");
    ASSERT_FALSE(nul(grade));
    ASSERT_EQ(grade.get(), 0.0f);
}

TEST_F(defPropExprTest, defPropVoidNegative) {
    make().negative().parse(R"SRC(
        main() void
            a void
            ret a
    )SRC").shouldVerified(false);
}

TEST_F(defPropExprTest, passingVoidIsOk) {
    make().parse(R"SRC(
        foo() void
            ret
        main() void
            ret foo() // <-- ok
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->isSub<nVoid>());
}

TEST_F(defPropExprTest, defVoidContainerNegative) {
    make().negative().parse(R"SRC(
        main() void
            a void[]
    )SRC").shouldParsed(false);
    shouldVerified(false);
}
/* TODO: TEST_F(defPropExprTest, defineVarWithoutCtorNegative) {
}*/
