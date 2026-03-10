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

TEST_F(literalExprIntegTest, strDoubleQuoteBasic) {
    make()
        .parse(R"SRC(
    main() int
        msg := "hello world"
        ret msg.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 11);
}

TEST_F(literalExprIntegTest, strSingleQuoteBasic) {
    make()
        .parse(R"SRC(
    main() int
        msg := 'hello world'
        ret msg.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 11);
}

TEST_F(literalExprIntegTest, strDoubleQuoteWithSingleQuoteInside) {
    make()
        .parse(R"SRC(
    main() int
        msg := "it's working"
        ret msg.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 12);
}

TEST_F(literalExprIntegTest, strSingleQuoteWithDoubleQuoteInside) {
    make()
        .parse(R"SRC(
    main() int
        msg := 'say "hello"'
        ret msg.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 11);
}

TEST_F(literalExprIntegTest, strEmptyDoubleQuote) {
    make()
        .parse(R"SRC(
    main() int
        msg := ""
        ret msg.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 0);
}

TEST_F(literalExprIntegTest, strEmptySingleQuote) {
    make()
        .parse(R"SRC(
    main() int
        msg := ''
        ret msg.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 0);
}

TEST_F(literalExprIntegTest, strEscapedDoubleQuoteInDoubleQuotes) {
    make()
        .parse(R"SRC(
    main() int
        msg := "say \"hello\""
        ret msg.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 11);
}

TEST_F(literalExprIntegTest, strEscapedSingleQuoteInSingleQuotes) {
    make()
        .parse(R"SRC(
    main() int
        msg := 'it\'s working'
        ret msg.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 12);
}

TEST_F(literalExprIntegTest, escapeSequencesInDoubleQuote) {
    make()
        .parse(R"SRC(
    main() int
        msg := "line1\nline2"
        ret msg.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 11);
}

TEST_F(literalExprIntegTest, escapeSequencesInSingleQuote) {
    make()
        .parse(R"SRC(
    main() int
        msg := 'line1\nline2'
        ret msg.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 11);
}

TEST_F(literalExprIntegTest, strTemplateWithDoubleQuotes) {
    make()
        .parse(R"SRC(
    main() int
        name := "world"
        msg := "hello $name"
        ret msg.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 11);
}

TEST_F(literalExprIntegTest, strTemplateWithSingleQuotes) {
    make()
        .parse(R"SRC(
    main() int
        name := 'world'
        msg := 'hello $name'
        ret msg.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 11);
}

TEST_F(literalExprIntegTest, strMixedQuoteTypes) {
    make()
        .parse(R"SRC(
        main() int
            a := "double"
            b := 'single'
            c := a + b
            ret c.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 12);
}

TEST_F(literalExprIntegTest, strDoubleQuoteEquality) {
    make()
        .parse(R"SRC(
        main() int
            a := "test"
            b := "test"
            a == b
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(literalExprIntegTest, strSingleQuoteEquality) {
    make()
        .parse(R"SRC(
        main() int
            a := 'test'
            b := 'test'
            a == b
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(literalExprIntegTest, strCrossQuoteEquality) {
    make()
        .parse(R"SRC(
        main() int
            a := "test"
            b := 'test'
            a == b
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(literalExprIntegTest, strTemplateWithIntVar) {
    make()
        .parse(R"SRC(
        foo(value int) str
            "your value is $value"
        main() int
            foo(18) == "your value is 18"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->isSub<nInt>());
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(literalExprIntegTest, testWithExpr) {
    make()
        .parse(R"SRC(
        def person
            age := 232
            race := "elf"
            hello() str
                "I'm $age yo, but still a child among ${race + "s"}."
        main() int
            person.hello() == "I'm 232 yo, but still a child among elfs."
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(literalExprIntegTest, strTemplateDoubleDollar) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            name := "world"
            msg := "hello $$name"
            ret msg.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 12); // hello $world
}

TEST_F(literalExprIntegTest, strTemplateMissingVar) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            msg := "hello $"
            ret msg.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 7);
}
