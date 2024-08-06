#include "../../../namuSyntaxTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct forExprTest: public namuSyntaxTest {};
}

TEST_F(forExprTest, simpleTest) {
    make()
        .parse(R"SRC(
        print(msg str) void: 1

        main() int
            sum := 0
            for n in {1, 2, 3}
                print("sum=" + sum as str + ", n=" + n as str + "\n")
                sum = sum + n
            ret sum
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 6);
}

TEST_F(forExprTest, simpleTest2) {
    make()
        .parse(R"SRC(
        print(msg str) void: 1

        main() int
            sum := 0
            for n in {1, 2, 3}
                sum = sum + n
                print("sum=" + sum as str + ", n=" + n as str + "\n")
            ret sum
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 6);
}

TEST_F(forExprTest, testWhatFromFunc) {
    make()
        .parse(R"SRC(
        print(msg str) void: 1

        foo() int[]
            ret {1, 2, 3}

        main() int
            sum := 0
            for n in foo()
                sum = sum + n
                print("sum=" + sum as str + ", n=" + n as str + "\n")

            ret sum
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 6);
}

TEST_F(forExprTest, putAkaMiddleOfLoop) {
    make()
        .parse(R"SRC(
        print(msg str) void: 1

        foo() int[]
            ret {1, 2, 3}

        main() int
            sum := 0
            for n in foo()
                sum = sum + n
                print("sum=" + sum as str + ", n=" + n as str + "\n")

            ret sum
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 6);
}

TEST_F(forExprTest, sequenceLoop) {
    make()
        .parse(R"SRC(
        print(msg str) void: 1

        main() int
            sum := 0
            for n in 2..5
                sum = sum + n
                print("sum=" + sum as str + ", n=" + n as str + "\n")

            ret sum
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 9);
}

TEST_F(forExprTest, validationCheckNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() str
            for n in {1, 2, 3}
                print(n)
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(forExprTest, loopObjects) {
    make()
        .parse(R"SRC(
        print(msg str) str: msg

        def person1
            name := "unknown"
            age := 0
            say() str
                print("I'm " + name + " and " + age as str + " years old.\n")

        main() int
            p1 person1
            p2 := person1()
            p3 := person1()
            p1.name = "Chales"
            p1.age = 36
            p2.name = "Mario"
            p2.age = 45
            p3.name = "Peach"
            p3.age = 44

            sum := ""
            for p in {p1, p2, p3}
                sum = sum + p.say()
            ret sum == "I'm Chales and 36 years old.\n" + "I'm Mario and 45 years old.\n" + "I'm Peach and 44 years old.\n"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res->cast<nint>(), 1);
}

TEST_F(forExprTest, useObjectAsContainer) {
    make()
        .parse(R"SRC(
        print(msg str) void: 1
        def person
            name := ""

        main() void
            p1 := person()
            p1.name = "Chales"
            people := {person(), p1}

            for p in people
                print(p.name)
    )SRC")
        .shouldVerified(true);
    run();
}

TEST_F(forExprTest, returnMiddleOfLoop) {
    make()
        .parse(R"SRC(
        def person
            name := ""

        main() int
            p1 := person()
            p1.name = "Chales"

            for p in {p1, person()}
                ret p.name == "Chales"
            ret 0
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(forExprTest, returnMiddleOfLoop1) {
    make()
        .parse(R"SRC(
        def person
            name := ""

        main() int
            p1 := person()
            p1.name = "Chales"

            res := (for p in {p1, person()}
                if p.name != p1.name
                    break
                p.name
            )
            ret (res[0] + " Lee") == "Chales Lee"
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(forExprTest, returnMiddleOfLoop1WithoutParenthesisNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def person
            name := ""

        main() int
            p1 := person()
            p1.name = "Chales"

            res := for p in {p1, person()}
                ret p1.name == "Chales" # <-- you can't (def)assign what return something inside.
            ret 0
    )SRC")
        .shouldVerified(false);
}

TEST_F(forExprTest, returnMiddleOfLoop1WithoutParenthesis) {
    make()
        .parse(R"SRC(
        def person
            name := ""

        main() int
            p1 := person()
            p1.name = "Chales"

            res := for p in {p1, person()}
                if p.name == "Chales"
                    ret 100
                p.name == "Chales"
            res[0] # {1, 0}
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 100);
}

TEST_F(forExprTest, retMiddleOfLoop) {
    make()
        .parse(R"SRC(
        def person
            name := ""

        main() int
            p1 := person()
            p1.name = "Chales"

            res := (for p in {person(), p1, person()}
                p1.name
            )
            ret res[1] + " Lee" == "Chales Lee"
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(forExprTest, retMiddleOfLoopWithoutParenthesis) {
    make()
        .parse(R"SRC(
        def person
            name := ""

        main() int
            p1 := person()
            p1.name = "Chales"

            res := for p in {p1, person()}
                p1.name # it's p1, not p.
            ret res[1] + " Lee" == "Chales Lee"
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(forExprTest, retMiddleOfLoopNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def person
            name := ""

        main() int
            p1 := person()
            p1.name = "Chales"

            res := 0
            res = (for p in {p1, person()}
                p1.name
            )
            ret res # res is array.
    )SRC")
        .shouldVerified(false);
}

TEST_F(forExprTest, evalForExprWithoutRet) {
    make()
        .parse(R"SRC(
        def person
            name := ""

        main() int
            p1 := person()
            p1.name = "Chales"

            res := (for p in {p1, person()}
                p.name
            )
            ret res[1] == ""
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(forExprTest, simpleBreakTest) {
    make()
        .parse(R"SRC(
        main() int
            res := (for n in 1..10
                if n < 5
                    n
                else
                    break
            ) # res will be {1, 2, 3, 4}
            ret res[res.len() - 1] # 4
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 4);
}

TEST_F(forExprTest, simpleBreakTest2) {
    make()
        .parse(R"SRC(
        main() int
            res := (for n in 1..10
                if n >= 5
                    break
                else
                    n
            ) # res will be {1, 2, 3, 4}
            ret res[res.len() - 1] # 4
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 4);
}

TEST_F(forExprTest, simpleBreakTestWithoutParenthesis) {
    make()
        .parse(R"SRC(
        main() int
            res := for n in 1..10
                if n < 5
                    next
                else if n > 5
                    break
                else
                    n
            ret res[0] + res.len() # 5 + 1
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 6);
}

TEST_F(forExprTest, retForExpr) {
    make()
        .parse(R"SRC(
        main() int
            answer := for n in 1..10
                if true
                    5
                else
                    7
            answer[3]
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 5);
}

TEST_F(forExprTest, breakIsNotExpressionNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo() int: 3
        main() int
            answer := for n in 1..5
                foo(break)
            answer[0]
    )SRC")
        .shouldParsed(false);
}

TEST_F(forExprTest, nextIsNotExpressionNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo() int: 3
        main() int
            answer := for n in 1..5
                foo(next)
            answer[0]
    )SRC")
        .shouldParsed(false);
}

TEST_F(forExprTest, breakInsideOfFor) {
    make()
        .parse(R"SRC(
        main() int
            answer := for n in 1..5
                if n < 4
                    n
                else: break
            answer[answer.len() - 1]
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 3);
}

TEST_F(forExprTest, breakInsideOfIfExprNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            for n in 1..5
                if n == 3
                    break
    )SRC")
        .shouldVerified(false);
}

TEST_F(forExprTest, breakInsideOfIfExprNegative2) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            ## this stmt returns what forExpr evaluated.
               but its type is void. because ifExpr doesn't have else
               block. ##
            res := for n in 1..5
                if n == 3
                    break
            ret res[res.len() - 1]
    )SRC")
        .shouldVerified(false);
}

TEST_F(forExprTest, breakInsideOfIfExpr2) {
    make()
        .parse(R"SRC(
        main() int
            res := for n in 1..5
                if n == 3
                    break
                else
                    n
            ret res[res.len() - 1] # res = {1, 2}
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 2);
}

TEST_F(forExprTest, breakNestedForLoop) {
    make()
        .parse(R"SRC(
        main() int
            sum := 0
            for n in 0..7
                for n2 in 0..4
                    ++sum
                    if sum > 15
                        break
                if sum > 15
                    break
            sum
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 16);
}

TEST_F(forExprTest, evalOfForLoop) {
    make()
        .parse(R"SRC(
        main() int
            sum := 0
            ans := for n in 0..8
                if sum > 3
                    break
                print(++sum as str)
            ret ans[ans.len() - 1] == "4"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(forExprTest, evalOfForLoop2) {
    make()
        .parse(R"SRC(
        main() int
            sum := 0
            for n in 0..8
                if sum > 3
                    break
                sum += n # sum = 0, 1, 3, 6
            sum
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 6);
}

TEST_F(forExprTest, evalOfForLoop3) {
    make()
        .parse(R"SRC(
        def a
            val := 0

        main() int
            res := for n in 0..8
                if n > 3
                    break
                a1 := a()
                a1.val = n
                a1
            ret res[res.len() - 1].val
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 3);
}

TEST_F(forExprTest, evalOfForLoop4) {
    make()
        .parse(R"SRC(
        main() int
            sum int
            answer := for n in 0..8
                if sum > 11
                    if true
                        break
                sum += n
            answer[answer.len() - 1] # answer = {0, 1, 3, 6, 10, 15}
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 15);
}

TEST_F(forExprTest, evalOfForLoop5) {
    make()
        .parse(R"SRC(
        def a
            foo() void
                ret

        main() int
            sum := 0
            for n in 0..8
                if sum > 3
                    break
                ++sum
            sum
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 4);
}

TEST_F(forExprTest, evalOfForLoopIntAndBoolIsCompatible) {
    make()
        .parse(R"SRC(
        main() int
            sum := 0
            answer := for n in 0..8
                if sum > 11
                    if true
                        break
                ++sum
            answer[answer.len() - 1]
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 8);
}

TEST_F(forExprTest, evalOfForLoopIntAndBoolIsCompatible2) {
    make()
        .parse(R"SRC(
        main() int
            sum := 0
            answer := for n in 0..8
                if sum > 3
                    if true
                        break
                ++sum
            answer[answer.len() - 1]
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res->getType(), ttype<nInt>::get());
    ASSERT_EQ(res.cast<nint>(), 4);
}

TEST_F(forExprTest, defAssignWhatLoops) {
    make()
        .parse(R"SRC(
        main() int
            a := for n in 0..2
                n++
            ret a[a.len() - 1]
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res->cast<nint>(), 1);
}

TEST_F(forExprTest, callFuncWithForExprArgument) {
    make()
        .parse(R"SRC(
        foo(age int) int: age + 1
        main() int
            foo((for n in 0..5
                x := n + 1
                x * 2
            )[3]) # {2, 4, 6, 8, 10}
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res->cast<nint>(), 9); // 8 + 1
}
