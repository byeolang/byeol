#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct ifExprIntegTest: public byeolIntegTest {};
}

TEST_F(ifExprIntegTest, simpleNestedTest) {
    make("demo")
        .parse(R"SRC(
        pack demo
        main() void
            if 11
                        22
                        if 22
                                33)SRC")
        .shouldVerified(true);
}

TEST_F(ifExprIntegTest, simpleIfTest) {
    make()
        .parse(R"SRC(
        main() int
            if true
                ret 11
            else
                ret 22
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 11);
}

TEST_F(ifExprIntegTest, simpleIfAssignTest) {
    make()
        .parse(R"SRC(
        main() int
            age := (if false
                11
            else
                22
            )
            ret age
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 22);
}

TEST_F(ifExprIntegTest, simpleIfAssignWithoutParenthesisTest) {
    make()
        .parse(R"SRC(
        main() int
            age := if false
                11
            else
                22
            ret age
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 22);
}

TEST_F(ifExprIntegTest, simpleReturnIfWithoutParenthesisTest) {
    make()
        .parse(R"SRC(
        main() int
            ret if true
                11
            else
                22
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 11);
}

TEST_F(ifExprIntegTest, simpleReturnDefAssignWithoutParenthesisTest) {
    make()
        .parse(R"SRC(
        main() int
            a := if true
                11
            else
                22
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 11);
}

TEST_F(ifExprIntegTest, NestIfTest) {
    make("demo")
        .parse(R"SRC(
        pack demo
        foo(abc int) void
           if "hello"
                       "hell  '  o"
                       if 33
                               "hel'lo")SRC")
        .shouldVerified(true);
}

TEST_F(ifExprIntegTest, NestIfTestNegative) {
    make("demo")
        .negative()
        .parse(R"SRC(
        pack demo
        foo(abc int) int
           if "hello"
                       "hell  '  o"
                       if 33
                               "hel'lo")SRC")
        .shouldVerified(false);
}

TEST_F(ifExprIntegTest, ifAsArgument) {
    make()
        .parse(R"SRC(
        abc(val int, val2 int) int
            ret val + val2

        main() int
            abc(if false
                22
            else
                11
            , 5)
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 16);
}

TEST_F(ifExprIntegTest, elif) {
    make()
        .parse(R"SRC(
        main() int
            if 222 < 33
                0
            else if 33 > 115
                1
            else if 44 < 235
                2
            else if 77 < 113
                3
            else
                7
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 2);
}

TEST_F(ifExprIntegTest, elifNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            if 222 < 33
                0
            else if 33 > 115
                1
            else if 44 < 235
                2
            else
                7
            else if 77 < 113
                3
    )SRC")
        .shouldParsed(false);
}

TEST_F(ifExprIntegTest, elifNegative2) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            if 222 < 33
                0
            else if 33 > 115
                1
            else if 44 < 235
                2
            else if 77 < 113
                3
            # there is no else block. so ifExpr eventually will be evaluated
            # to the void.
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(ifExprIntegTest, elif2) {
    make()
        .parse(R"SRC(
        main() int
            if 222 < 33
                0
            else if 33 > 115
                1
            else if 44 >= 235
                2
            else
                3
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 3);
}

TEST_F(ifExprIntegTest, evalIfWithStrAndIntNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            if 23 == 23.0
                "hello"
            else
                22
    )SRC")
        .shouldVerified(false);
}

TEST_F(ifExprIntegTest, evalIfWithStrAndIntNegative2) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            if 23 == 23.0
                "hello"
            else
                22
    )SRC")
        .shouldVerified(false);
}

TEST_F(ifExprIntegTest, evalIfWithStrAndIntNegative3) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            a := if 23 == 23.0
                if true
                    false
                else
                    "hello"
            else
                22
            ret a
    )SRC")
        .shouldVerified(false);
}

TEST_F(ifExprIntegTest, evalIfWithStrAndInt) {
    make()
        .parse(R"SRC(
        main() int
            a := if 23 == 23.0
                if true
                    "true"
                else
                    "never"
            else
                "false"
            ret a == "true"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(ifExprIntegTest, testAssignWithCompoundExpr) {
    make()
        .parse(R"SRC(
        main() int
            a := "initial"
            a = if 23 == 23.0
                if true
                    "true"
                else
                    "never"
            else
                "false"
            ret a == "true"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(ifExprIntegTest, evalIfExprReturningSomething) {
    make()
        .parse(R"SRC(
        main() int
            val := 2 > 5
            a := if val
                "seven_angel"
            else
                23 as str + "wow"
            ret a.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res->cast<nint>(), 5);
}

TEST_F(ifExprIntegTest, evalIfExprReturningSomething2) {
    make()
        .parse(R"SRC(
        main() int
            val := 2 > 5
            a := if val
                ret -1 # if whole block uses 'ret', then it would be ignored when infer type of ifExpr.
            else
                23 as str + "wow"
            ret a.len()
    )SRC")
        .shouldVerified(true);
}

TEST_F(ifExprIntegTest, evalIfExprReturningSomething3) {
    make()
        .parse(R"SRC(
        main() int
            val := 2 > 5
            a := if val
                23 as str + "wow"
            else
                ret -1 # if whole block uses 'ret', then it would be ignored when infer type of ifExpr.
            ret a.len()
    )SRC")
        .shouldVerified(true);
}

TEST_F(ifExprIntegTest, evalIfExprReturningSomethingNegative2) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            val := 2 > 5
            a := if val
                ret -1
            else
                ret 1
            ret a
    )SRC")
        .shouldVerified(false);

    const auto& rpt = getReport();
    ASSERT_TRUE(rpt.inErr());
}

TEST_F(ifExprIntegTest, nestedIfBlockReturnSomething) {
    make()
        .parse(R"SRC(
        main() int
            a := if 3 > 5
                "no"
            else
                if 35 > 22
                    "yes"
                else
                    "no"
            ret a.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 3);
}
