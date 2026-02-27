#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct literalExprIntegTest: public byeolIntegTest {};
}

TEST_F(literalExprIntegTest, hexValue) {
    make()
        .parse(R"SRC(
        main() int
            0xff
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 255);
}

TEST_F(literalExprIntegTest, hexValue2) {
    make()
        .parse(R"SRC(
        foo() byte
            0xff
        main() int
            foo()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 255);
}

TEST_F(literalExprIntegTest, octalValue) {
    make()
        .parse(R"SRC(
        main() int
            017
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 15);
}

TEST_F(literalExprIntegTest, octalValue2) {
    make()
        .parse(R"SRC(
        foo() byte
            017
        main() int
            foo()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 15);
}

TEST_F(literalExprIntegTest, byteAsInt) {
    make()
        .parse(R"SRC(
        main() int
            0xff as byte
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 255);
}

TEST_F(literalExprIntegTest, byteAsInt1) {
    make()
        .parse(R"SRC(
        main() int
            0xff as flt
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 255);
}

TEST_F(literalExprIntegTest, byteAsInt2) {
    make()
        .parse(R"SRC(
        main() int
            "0xff" as int
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 255);
}

TEST_F(literalExprIntegTest, byteAsInt3) {
    make()
        .parse(R"SRC(
        main() int
            "0x00" as int
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 0);
}

TEST_F(literalExprIntegTest, byteAsInt4) {
    make()
        .parse(R"SRC(
        main() int
            "00" as int
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 0);
}

TEST_F(literalExprIntegTest, useCtor) {
    make()
        .parse(R"SRC(
        main() int
            byte(0xff)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 255);
}

TEST_F(literalExprIntegTest, useCtor1) {
    make()
        .parse(R"SRC(
        main() int
            byte(0x100)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 0);
}

TEST_F(literalExprIntegTest, escapeSequence1) {
    make()
        .parse(R"SRC(
        main() int
            a := "\0x2B"
            a == "+"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(literalExprIntegTest, escapeSequence2) {
    make()
        .parse(R"SRC(
        main() int
            a := "\43"
            a == "+"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(literalExprIntegTest, escapeSequence3) {
    make()
        .parse(R"SRC(
        main() void
            a := "\'"
    )SRC")
        .shouldVerified(true);
}

TEST_F(literalExprIntegTest, escapeSequence4) {
    make()
        .parse(R"SRC(
        main() void
            a := "👍"
    )SRC")
        .shouldVerified(true);
}

TEST_F(literalExprIntegTest, escapeSequence5) {
    make()
        .parse(R"SRC(
        main() int
            a := "\r"
            a as byte == 13
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}
