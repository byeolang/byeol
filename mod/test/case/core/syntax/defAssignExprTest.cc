#include "../../../namuSyntaxTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct defAssignExprTest : public namuSyntaxTest {};
}

TEST_F(defAssignExprTest, simpleGlobalDefAssign) {
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

TEST_F(defAssignExprTest, simpleLocalDefAssign) {
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

TEST_F(defAssignExprTest, testCircularDependencies) {
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

TEST_F(defAssignExprTest, testNearCircularDependencies) {
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

TEST_F(defAssignExprTest, testDefAssign1) {
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

TEST_F(defAssignExprTest, defAssignInObjectRefersInvalidFuncNegative) {
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

TEST_F(defAssignExprTest, defAssignInObjectRefersInvalidFuncNegative2) {
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

TEST_F(defAssignExprTest, defAssignRefersItsIdentifier) {
    make().negative().parse(R"SRC(
        con := sys.con
        con1 := con.add(1, 2)

        main() void
            print("res=" + con1 as str)
    )SRC").shouldParsed(true);
    shouldVerified(true);
}

TEST_F(defAssignExprTest, defAssignRefersItsIdentifierNegative1) {
    make().negative().parse(R"SRC(
        con := sys.con
        con := con.add(1, 2)

        main() void
            print("res=" + con)
    )SRC").shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defAssignExprTest, defAssignRefersItsIdentifierNegative2) {
    make().negative().parse(R"SRC(
        con := sys.con
        con := con.add(1, 2)

        main() void
            print("res=" + con.add(1, 2))
    )SRC").shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defAssignExprTest, defAssignAsParameter) {
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

TEST_F(defAssignExprTest, defAssignAsParameter2) {
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

TEST_F(defAssignExprTest, defAssignAsParameterNegative3) {
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

TEST_F(defAssignExprTest, defAssignAsParameter3) {
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

TEST_F(defAssignExprTest, defAssignDefAssignedValue) {
    make().parse(R"SRC(
        def a
            age int
        main() int
            /*a2 := a()
            a1 := a2*/
            a1 := a()
            ret a1.age
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 0);
}

TEST_F(defAssignExprTest, defAssignVoidNegative) {
    make().negative().parse(R"SRC(
        main() void
            a := void()
    )SRC").shouldVerified(false);
}

TEST_F(defAssignExprTest, defAssignVoidNegative2) {
    make().negative().parse(R"SRC(
        foo() void
            ret

        main() void
            a := for n in 0..1
                foo()
            ret a
    )SRC").shouldVerified(false);
}

TEST_F(defAssignExprTest, defAssignIfWithoutElseNegative) {
    make().negative().parse(R"SRC(
        main() void
            a := if true
                "hello"
    )SRC").shouldVerified(false);
}

TEST_F(defAssignExprTest, defAssignIfWithElse) {
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

TEST_F(defAssignExprTest, defAssignEvalOfSetElemConversion) {
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

TEST_F(defAssignExprTest, defAssignAtIf) {
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

TEST_F(defAssignExprTest, defAssignAtBlockNegative) {
    make().negative().parse(R"SRC(
        main() int
            if true
                res := 0.8
            ret res == 0.8
    )SRC").shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defAssignExprTest, cantAssignWithForLoopReturningSomethingNegative) {
    make().negative().parse(R"SRC(
        main() int
            abc := for strArr in {{"hello"}}
                for s in strArr
                    ret s
    )SRC").shouldVerified(false);
}

TEST_F(defAssignExprTest, cantAssignWithForLoopReturningSomething) {
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

TEST_F(defAssignExprTest, assignFromForExprDeclaringLocalVariable) {
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

TEST_F(defAssignExprTest, selfDefAssigningIsNotAllowedNegative) {
    make().negative().parse(R"SRC(
        def a
            a1 := a()
        main() void
            a().a1
    )SRC").shouldVerified(false);
}

TEST_F(defAssignExprTest, selfDefAssigningIsNotAllowedNegative2) {
    make().negative().parse(R"SRC(
        def a
            a1 a // if a1 is nullable of a, this is valid syntax.
        main() void
            a().a1
    )SRC").shouldVerified(false);
}
/* TODO: TEST_F(defAssignExprTest, defineVarWithoutCtorNegative) {
}
TEST_F(defAssignExprTest, selfDefAssigningOfNullableIsAllowed) {
    make().parse(R"SRC(
        def a
            a1 a?
        main() int
            a().a1 == null
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}*/
