#include "../../../namuSyntaxTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct literalExprTest: public namuSyntaxTest {};
}

TEST_F(literalExprTest, hexValue) {
    make()
        .parse(R"SRC(
        main() int
            0xff
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 255);
}

TEST_F(literalExprTest, hexValue2) {
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
    ASSERT_EQ(res.cast<nint>(), 255);
}

TEST_F(literalExprTest, octalValue) {
    make()
        .parse(R"SRC(
        main() int
            017
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 15);
}

TEST_F(literalExprTest, octalValue2) {
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
    ASSERT_EQ(res.cast<nint>(), 15);
}

TEST_F(literalExprTest, byteAsInt) {
    make()
        .parse(R"SRC(
        main() int
            0xff as byte
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 255);
}

TEST_F(literalExprTest, byteAsInt1) {
    make()
        .parse(R"SRC(
        main() int
            0xff as flt
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 255);
}

TEST_F(literalExprTest, byteAsInt2) {
    make()
        .parse(R"SRC(
        main() int
            "0xff" as int
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 255);
}

TEST_F(literalExprTest, byteAsInt3) {
    make()
        .parse(R"SRC(
        main() int
            "0x00" as int
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 0);
}

TEST_F(literalExprTest, byteAsInt4) {
    make()
        .parse(R"SRC(
        main() int
            "00" as int
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 0);
}

TEST_F(literalExprTest, useCtor) {
    make()
        .parse(R"SRC(
        main() int
            byte(0xff)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 255);
}

TEST_F(literalExprTest, useCtor1) {
    make()
        .parse(R"SRC(
        main() int
            byte(0x100)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 0);
}

TEST_F(literalExprTest, escapeSequence1) {
    make()
        .parse(R"SRC(
        main() int
            a := '\0x2B'
            a == '+'
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(literalExprTest, escapeSequence2) {
    make()
        .parse(R"SRC(
        main() int
            a := '\43'
            a == '+'
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(literalExprTest, escapeSequence3) {
    make()
        .parse(R"SRC(
        main() void
            a := '\''
    )SRC")
        .shouldVerified(true);
}

TEST_F(literalExprTest, escapeSequenceNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := '👍'
    )SRC")
        .shouldVerified(false);
}

TEST_F(literalExprTest, escapeSequence4) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := "👍"
    )SRC")
        .shouldVerified(true);
}

TEST_F(literalExprTest, escapeSequence5) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            a := '\r'
            a as int == 13
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}
