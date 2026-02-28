#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct literalAntiTest: public byeolIntegTest {};
}

TEST_F(literalAntiTest, stringTemplateSpaceAfterDollarNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            name := "world"
            msg := "hello $ name"
            ret msg.len()
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res->cast<nint>(), 12); // hello $ name
}

TEST_F(literalAntiTest, cStyleSingleLineCommentNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 5  // this is a comment
    )SRC")
        .shouldParsed(false);
}

TEST_F(literalAntiTest, cStyleBlockCommentNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            /* this is a comment */
            a := 5
    )SRC")
        .shouldParsed(false);
}

TEST_F(literalAntiTest, arrayLiteralWithSemicolonNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            arr := {1; 2; 3}
    )SRC")
        .shouldParsed(false);
}

TEST_F(literalAntiTest, doubleIndexingNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            arr := {1, 2, 3}
            val := arr[[0]]
    )SRC")
        .shouldParsed(false);
}

TEST_F(literalAntiTest, stringDivisionNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := "hello"
            b := a / 2
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(literalAntiTest, stringSubtractionNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := "hello"
            b := a - "ll"
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(literalAntiTest, stringMultiplicationNegative) {
    make()
        .parse(R"SRC(
        main() void
            a := "hello"
            b := a * "world"
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}
