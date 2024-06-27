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
        ASSERT_EQ(defProp.getRight().getType(), ttype<nInt>());
    }
}

TEST_F(defPropExprTest, definePackVariableNegative) {
    make().negative().parse(R"SRC(
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

TEST_F(defPropExprTest, defPropWithObj) {
    make().parse(R"SRC(
        def a
            age := 22
        a1 a
        a2 a
        main() int
            a2.age
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 22);
}

TEST_F(defPropExprTest, simpleGlobalDefAssign) {
    // control group.
    make().parse(R"SRC(
        age int // age is age
        main() int // main is also a main
            age := 5
            ret 0
    )SRC").shouldVerified(true);
    auto& owns = (scope::super&) getSlot().subs().getContainer();
    auto& shares = (scope::super&) getSlot().subs().getNext().getContainer();
    ASSERT_FALSE(nul(shares));
    ASSERT_FALSE(nul(owns));
    ASSERT_EQ(owns.len(), 1);
    ASSERT_EQ(shares.len(), 3);

    run();
    ASSERT_EQ(getSubPack().sub<nInt>("age").cast<int>(), 0);
}

TEST_F(defPropExprTest, simpleLocalDefAssign) {
    // control group.
    make().parse(R"SRC(
        age int // age is age
        main() int // main is also a main
            age = 3
            age := 5
            age = 2
            ret 0
    )SRC").shouldVerified(true);
    run();
    ASSERT_EQ(getSubPack().sub("age").cast<int>(), 3);
}

TEST_F(defPropExprTest, testCircularDependencies) {
    make("holymoly").negative().parse(R"SRC(
        pack holymoly

        a := c
        b := a
        c := b // type can't be defined.

        main() int
            ret 0
    )SRC").shouldParsed(true);
    shouldVerified(false);
    // however when runs it, it throws an error.
}

TEST_F(defPropExprTest, testNearCircularDependencies) {
    make("holymoly").parse(R"SRC(
        pack holymoly

        c := 1 // type can be defined.
        a := c
        b := a

        main() int
            print(a as str)
            print(b as str)
            ret 0
    )SRC").shouldParsed(true);
    shouldVerified(true);
    // however when runs it, it throws an error.
    // because assigning 1 to c will be done after evaluating of assignment of the 'a'.
}

TEST_F(defPropExprTest, testDefAssign1) {
    make().parse(R"SRC(
        foo() int
            a = 2

        a := foo() + 5

        main() int
            print("a=" + a as str)
            ret 0
    )SRC").shouldVerified(true);
    run();
}

TEST_F(defPropExprTest, defAssignInObjectRefersInvalidFuncNegative) {
    make().negative().parse(R"SRC(
        nickname := foo()

        foo() str
            print("I'm foo!\n")
            ret 1 // this is invalid function.

        main() void
            print("your nickname is " + nickname)
    )SRC").shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defPropExprTest, defAssignInObjectRefersInvalidFuncNegative2) {
    make().negative().parse(R"SRC(
        nickname := boo() // refers the func that doesn't exist.

        foo() str
            print("I'm foo!\n")
            ret 1 // this is invalid function.

        main() void
            print("your nickname is " + nickname)
    )SRC").shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defPropExprTest, defAssignRefersItsIdentifier) {
    make().negative().parse(R"SRC(
        con := sys.con
        con1 := con.add(1, 2)

        main() void
            print("res=" + con1 as str)
    )SRC").shouldParsed(true);
    shouldVerified(true);
}

TEST_F(defPropExprTest, defAssignRefersItsIdentifierNegative1) {
    make().negative().parse(R"SRC(
        con := sys.con
        con := con.add(1, 2)

        main() void
            print("res=" + con)
    )SRC").shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defPropExprTest, defAssignRefersItsIdentifierNegative2) {
    make().negative().parse(R"SRC(
        con := sys.con
        con := con.add(1, 2)

        main() void
            print("res=" + con.add(1, 2))
    )SRC").shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defPropExprTest, defAssignAsParameter) {
    make().parse(R"SRC(
        def f
            foo(a int, b int) int
                ret a + b

        main() void
            f1 f
            a := 5
            a = f1.foo(a, 5)
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->is<nVoid>());
}

TEST_F(defPropExprTest, defAssignAsParameter2) {
    make().negative().parse(R"SRC(
        def f
            foo(a int, b int) int
                ret a + b

        main() int
            f1 f
            a := 5
            a = f1.foo((a), 5)
    )SRC").shouldParsed(true);
    shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 10);
}

TEST_F(defPropExprTest, defAssignAsParameterNegative3) {
    make().negative().parse(R"SRC(
        def f
            foo(a int, b int) int
                ret a + b

        main() int
            f1 f
            a2 = f1.foo(a2 := 5, 5)
            a2 = a2 + a
    )SRC").shouldVerified(false);
}

TEST_F(defPropExprTest, defAssignAsParameter3) {
    make().parse(R"SRC(
        def f
            foo(a int, b int) int
                ret a + b

        main() int
            f1 f
            a := 5
            a2 := f1.foo(a, 5)
            a2 = a2 + a
    )SRC").shouldParsed(true);
    shouldVerified(true);
}

TEST_F(defPropExprTest, defAssignDefAssignedValue) {
    make().parse(R"SRC(
        def a
            age int
        main() int
            a2 := a()
            a1 := a2
            ret a1.age
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 0);
}

TEST_F(defPropExprTest, defAssignVoidNegative) {
    make().negative().parse(R"SRC(
        main() void
            a := void()
    )SRC").shouldVerified(false);
}

TEST_F(defPropExprTest, defAssignVoidNegative2) {
    make().negative().parse(R"SRC(
        foo() void
            ret

        main() void
            a := for n in 0..1
                foo()
            ret a
    )SRC").shouldVerified(false);
}

TEST_F(defPropExprTest, defAssignIfWithoutElseNegative) {
    make().negative().parse(R"SRC(
        main() void
            a := if true
                "hello"
    )SRC").shouldVerified(false);
}

TEST_F(defPropExprTest, defAssignIfWithElse) {
    make().parse(R"SRC(
        main() int
            a := if true
                "hello"
            else
                "world"
            ret a == "hello"
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(defPropExprTest, defAssignEvalOfSetElemConversion) {
    make().parse(R"SRC(
        foo() int
            0
        boo() int
            1
        main() int
            arr := {1, 2}
            arr[foo()] -= boo()
            val := arr[foo()]
            val == true
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), false);
}

TEST_F(defPropExprTest, defAssignAtIf) {
    make().parse(R"SRC(
        main() int
            res := 0.3
            if res
                ret res == 0.3
            ret 0
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(defPropExprTest, defAssignAtBlockNegative) {
    make().negative().parse(R"SRC(
        main() int
            if true
                res := 0.8
            ret res == 0.8
    )SRC").shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defPropExprTest, cantAssignWithForLoopReturningSomethingNegative) {
    make().negative().parse(R"SRC(
        main() int
            abc := for strArr in {{"hello"}}
                for s in strArr
                    ret s
    )SRC").shouldVerified(false);
}

TEST_F(defPropExprTest, cantAssignWithForLoopReturningSomething) {
    make().parse(R"SRC(
        main() int
            abc := for strArr in {{"child", "hello"}, {"parent", "world"}}
                for s in strArr
                    if s == "world!"
                        ret s.len()
                    else: s
            ret abc[1][0] == "parent"
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res->cast<nint>(), 1);
}

TEST_F(defPropExprTest, assignFromForExprDeclaringLocalVariable) {
    make().parse(R"SRC(
        main() int
            abc := for n in 0..5
                x := n * 2
                x + n
            abc[4] // abc = {0, 3, 6, 9, 12}
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res->cast<nint>(), 12);
}
/* TODO: TEST_F(defPropExprTest, defineVarWithoutCtorNegative) {
}*/
