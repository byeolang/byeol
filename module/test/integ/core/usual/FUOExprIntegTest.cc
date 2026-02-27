#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct FUOExprIntegTest: public byeolIntegTest {};
}

TEST_F(FUOExprIntegTest, testUnaryPrefixOp) {
    make()
        .parse(R"SRC(
        main() int
            a := 3
            ++a
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 4);
}

TEST_F(FUOExprIntegTest, testUnaryPrefixOp2) {
    make()
        .parse(R"SRC(
        main() int
            a := 3
            --a
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 2);
}

TEST_F(FUOExprIntegTest, testUnaryPrefixOp3) {
    make()
        .parse(R"SRC(
        main() int
            a := 3
            -a
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), -3);
}

TEST_F(FUOExprIntegTest, testUnaryPrefixOp4) {
    make()
        .parse(R"SRC(
        main() int
            a := 3
            if !(a < 3)
                11
            else
                22
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 11);
}

TEST_F(FUOExprIntegTest, testUnaryPrefixOp5) {
    make()
        .parse(R"SRC(
        main() int
            a := 3
            if !a
                22
            else
                11
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 11);
}

TEST_F(FUOExprIntegTest, testUnaryPrefixOp6) {
    make()
        .parse(R"SRC(
        main() int
            a := 0
            if !a
                22
            else
                11
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 22);
}

TEST_F(FUOExprIntegTest, testUnaryPrefixOpNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            a := "false"
            if !a
                22
            else
                11
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(FUOExprIntegTest, testUnaryPrefixOpNegative2) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := "false"
            ++a
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(FUOExprIntegTest, testUnaryPrefixOpNegative3) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := "false"
            --a
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(FUOExprIntegTest, testUnaryPostfixDoublePlus) {
    make()
        .parse(R"SRC(
        main() int
            a := 2
            ret a++
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 2);
}

TEST_F(FUOExprIntegTest, testUnaryPostfixDoublePlus2) {
    make()
        .parse(R"SRC(
        main() int
            a := 2
            a++
            ret a
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 3);
}

TEST_F(FUOExprIntegTest, testUnaryPostfixDoubleMinus) {
    make()
        .parse(R"SRC(
        main() int
            a := 2
            ret a--
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 2);
}

TEST_F(FUOExprIntegTest, testUnaryPostfixDoubleMinus2) {
    make()
        .parse(R"SRC(
        main() int
            a := 2
            a--
            ret a
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(FUOExprIntegTest, testUnaryPostfixDoubleMinus3) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            a := "hello"
            a--
            ret a == "hello"
    )SRC")
        .shouldVerified(false);
}

TEST_F(FUOExprIntegTest, strNotSuitableToOpNegative1) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := "wow"
            ++a
    )SRC")
        .shouldVerified(false);
}

TEST_F(FUOExprIntegTest, strNotSuitableToOpNegative2) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := "wow"
            a--
    )SRC")
        .shouldVerified(false);
}
