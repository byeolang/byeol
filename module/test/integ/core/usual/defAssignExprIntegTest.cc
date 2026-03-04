#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct defAssignExprIntegTest: public byeolIntegTest {};
}

TEST_F(defAssignExprIntegTest, simpleGlobalDefAssign) {
    // control group.
    make()
        .parse(R"SRC(
        age int # age is age
        main() int # main is also a main
            age := 5
            ret 0
    )SRC")
        .shouldVerified(true);

    scope::super& owns = (scope::super*) (getSlot() TO(subs().getContainer())) OR_ASSERT(owns);
    scope::super& shares = (scope::super*) (getSlot() TO(subs().getNext()) TO(getContainer())) OR_ASSERT(shares);
    ASSERT_EQ(owns.len(), 1);
    ASSERT_EQ(shares.len(), 3);

    run();

    ASSERT_EQ(*getSubPack()->sub<nInt>("age")->cast<int>(), 0);
}

TEST_F(defAssignExprIntegTest, simpleLocalDefAssign) {
    // control group.
    make()
        .parse(R"SRC(
        age int # age is age
        main() int # main is also a main
            age = 3
            age := 5
            age = 2
            ret 0
    )SRC")
        .shouldVerified(true);

    run();

    ASSERT_EQ(*getSubPack()->sub("age")->cast<int>(), 3);
}

TEST_F(defAssignExprIntegTest, testCircularDependencies) {
    make("holymoly")
        .negative()
        .parse(R"SRC(
        pack holymoly

        a := c
        b := a
        c := b # type can't be defined.

        main() int
            ret 0
    )SRC")
        .shouldParsed(true);

    shouldVerified(false);
    // however when runs it, it throws an error.
}

TEST_F(defAssignExprIntegTest, testNearCircularDependencies) {
    make("holymoly")
        .parse(R"SRC(
        pack holymoly

        c := 1 # type can be defined.
        a := c
        b := a

        main() int
            print(a as str)
            print(b as str)
            ret 0
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);

    // however when runs it, it throws an error.
    // because assigning 1 to c will be done after runuating of assignment of the 'a'.
}

TEST_F(defAssignExprIntegTest, testDefAssignNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo() int
            a = 2

        a := foo() + 5 # to define `a`, foo() refers `a` but, it doesn't exist yet.

        main() int
            print("a=" + a as str)
            ret 0
    )SRC")
        .shouldVerified(false);
}

TEST_F(defAssignExprIntegTest, defAssignInObjectRefersInvalidFuncNegative) {
    make()
        .negative()
        .parse(R"SRC(
        print(msg str) void: 1

        nickname := foo()

        foo() str
            print("I'm foo!\n")
            ret 1 # this is invalid function.

        main() void
            print("your nickname is " + nickname)
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defAssignExprIntegTest, defAssignInObjectRefersInvalidFuncNegative2) {
    make()
        .negative()
        .parse(R"SRC(
        nickname := boo() # refers the func that doesn't exist.

        foo() str
            print("I'm foo!\n")
            ret 1 # this is invalid function.

        main() void
            print("your nickname is " + nickname)
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defAssignExprIntegTest, defAssignRefersItsIdentifier) {
    make()
        .parse(R"SRC(
        con := sys.con
        con1 := con.add(1, 2)

        main() void
            print("res=" + con1 as str)
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);
}

TEST_F(defAssignExprIntegTest, defAssignRefersItsIdentifierNegative1) {
    make()
        .negative()
        .parse(R"SRC(
        con := sys.con
        con := con.add(1, 2)

        main() void
            print("res=" + con)
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defAssignExprIntegTest, defAssignRefersItsIdentifierNegative2) {
    make()
        .negative()
        .parse(R"SRC(
        con := sys.con
        con := con.add(1, 2)

        main() void
            print("res=" + con.add(1, 2))
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defAssignExprIntegTest, defAssignAsParameter) {
    make()
        .parse(R"SRC(
        def f
            foo(a int, b int) int
                ret a + b

        main() void
            f1 f
            a := 5
            a = f1.foo(a, 5)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->is<nVoid>());
}

TEST_F(defAssignExprIntegTest, defAssignAsParameter2) {
    make()
        .negative()
        .parse(R"SRC(
        def f
            foo(a int, b int) int
                ret a + b

        main() int
            f1 f
            a := 5
            a = f1.foo((a), 5)
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 10);
}

TEST_F(defAssignExprIntegTest, defAssignAsParameterNegative3) {
    make()
        .negative()
        .parse(R"SRC(
        def f
            foo(a int, b int) int
                ret a + b

        main() int
            f1 f
            a2 = f1.foo(a2 := 5, 5)
            a2 = a2 + a
    )SRC")
        .shouldVerified(false);
}

TEST_F(defAssignExprIntegTest, defAssignAsParameter3) {
    make()
        .parse(R"SRC(
        def f
            foo(a int, b int) int
                ret a + b

        main() int
            f1 f
            a := 5
            a2 := f1.foo(a, 5)
            a2 = a2 + a
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);
}

TEST_F(defAssignExprIntegTest, defAssignDefAssignedValue) {
    make()
        .parse(R"SRC(
        def a
            age int
        main() int
            ##a2 := a()
            a1 := a2##
            a1 := a()
            ret a1.age
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 0);
}

TEST_F(defAssignExprIntegTest, defAssignVoidNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := void()
    )SRC")
        .shouldVerified(false);
}

TEST_F(defAssignExprIntegTest, defAssignVoidNegative2) {
    make()
        .negative()
        .parse(R"SRC(
        foo() void
            ret

        main() void
            a := for n in 0..1
                foo()
            ret a
    )SRC")
        .shouldVerified(false);
}

TEST_F(defAssignExprIntegTest, defAssignIfWithoutElseNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := if true
                "hello"
    )SRC")
        .shouldVerified(false);
}

TEST_F(defAssignExprIntegTest, defAssignIfWithElse) {
    make()
        .parse(R"SRC(
        main() int
            a := if true
                "hello"
            else
                "world"
            ret a == "hello"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(defAssignExprIntegTest, defAssignrunOfSetElemConversion) {
    make()
        .parse(R"SRC(
        foo() int
            0
        boo() int
            1
        main() int
            arr := {1, 2}
            arr[foo()] -= boo()
            val := arr[foo()]
            val == true
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), false);
}

TEST_F(defAssignExprIntegTest, defAssignAtIf) {
    make()
        .parse(R"SRC(
        main() int
            res := 0.3
            if res
                ret res == 0.3
            ret 0
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(defAssignExprIntegTest, defAssignAtBlockNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            if true
                res := 0.8
            ret res == 0.8
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defAssignExprIntegTest, cantAssignWithForLoopReturningSomethingNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            abc := for strArr in {{"hello"}}
                for s in strArr
                    ret s
    )SRC")
        .shouldVerified(false);
}

TEST_F(defAssignExprIntegTest, cantAssignWithForLoopReturningSomething) {
    make()
        .parse(R"SRC(
        main() int
            abc := for strArr in {{"child", "hello"}, {"parent", "world"}}
                for s in strArr
                    if s == "world!"
                        ret s.len()
                    else: s
            ret abc[1][0] == "parent"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res->cast<nint>(), 1);
}

TEST_F(defAssignExprIntegTest, assignFromForExprDeclaringLocalVariable) {
    make()
        .parse(R"SRC(
        main() int
            abc := for n in 0..5
                x := n * 2
                x + n
            abc[4] # abc = {0, 3, 6, 9, 12}
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res->cast<nint>(), 12);
}

TEST_F(defAssignExprIntegTest, selfDefAssigningIsNotAllowedNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def a
            a1 := a()
        main() void
            a().a1
    )SRC")
        .shouldVerified(false);
}

TEST_F(defAssignExprIntegTest, selfDefAssigningIsNotAllowedNegative2) {
    make()
        .negative()
        .parse(R"SRC(
        def a
            a1 a # if a1 is nullable of a, this is valid syntax.
        main() void
            a().a1
    )SRC")
        .shouldVerified(false);
}

TEST_F(defAssignExprIntegTest, defineVarWithoutCtorNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def A
            _age int
            ctor(n int): age = n
        main() int
            a1 A # err: A don't have 'ctor()' func.
    )SRC")
        .shouldVerified(false);
}

TEST_F(defAssignExprIntegTest, defAssignWithTypes) {
    make()
        .parse(R"SRC(
        def Person
            age int
            ctor(a age): age = a
            hello() str
                ret "I'm " + age as str + "yo"

        foo(newAge int) Person: Person(newAge)

        main() int
            p1 Person := foo(23)
            p1.hello() == "I'm 23yo"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(defAssignExprIntegTest, defAssignWithTypesVoidNotAllowedNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            age void := 33
    )SRC")
        .shouldVerified(false);
}

TEST_F(defAssignExprIntegTest, defAssignWithTypesWithArray) {
    make()
        .parse(R"SRC(
        main() int
            arr := {1.1, 1.2, 1.3}
            arr2 int[] := {1.1, 1.2, 1.3}
            for n in 0..arr.len()
                if arr[n] as int == arr2[n]: ret 0
                if arr2[n] is flt: ret 0
            ret 1
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

/*
TEST_F(defAssignExprTest, selfDefAssigningOfNullableIsAllowed) {
    make().parse(R"SRC(
        def a
            a1 a?
        main() int
            a().a1 == null
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}*/
