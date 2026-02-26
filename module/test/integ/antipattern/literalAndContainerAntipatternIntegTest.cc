#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct literalAndContainerAntipatternIntegTest: public byeolIntegTest {};
}

// ============================================================================
// Category 5: String Template Errors
// ============================================================================

TEST_F(literalAndContainerAntipatternIntegTest, stringTemplateDoubleDollarNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            name := "world"
            msg := "hello $$name"
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(literalAndContainerAntipatternIntegTest, stringTemplateMissingVarNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            msg := "hello $"
    )SRC")
        .shouldParsed(false);
}

TEST_F(literalAndContainerAntipatternIntegTest, stringTemplateSpaceAfterDollarNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            name := "world"
            msg := "hello $ name"
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

// ============================================================================
// Category 11: Comment Syntax Errors
// ============================================================================

TEST_F(literalAndContainerAntipatternIntegTest, cStyleSingleLineCommentNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 5  // this is a comment
    )SRC")
        .shouldParsed(false);
}

TEST_F(literalAndContainerAntipatternIntegTest, cStyleBlockCommentNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            /* this is a comment */
            a := 5
    )SRC")
        .shouldParsed(false);
}

TEST_F(literalAndContainerAntipatternIntegTest, pythonStyleCommentNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            # this is a comment
            a := 5
    )SRC")
        .shouldParsed(false);
}

// ============================================================================
// Category 12: Sequence Errors
// ============================================================================

TEST_F(literalAndContainerAntipatternIntegTest, arrayLiteralWithSemicolonNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            arr := {1; 2; 3}
    )SRC")
        .shouldParsed(false);
}

TEST_F(literalAndContainerAntipatternIntegTest, arrayLiteralTrailingCommanNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            arr := {1, 2, 3,}
    )SRC")
        .shouldParsed(false);
}

TEST_F(literalAndContainerAntipatternIntegTest, emptyArrayWithoutTypeNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            arr := {}
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

// ============================================================================
// Category 13: Map/Array Literal Errors
// ============================================================================

TEST_F(literalAndContainerAntipatternIntegTest, mapLiteralWithColonNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            map := {"key": "value"}
    )SRC")
        .shouldParsed(false);
}

TEST_F(literalAndContainerAntipatternIntegTest, mapLiteralWithArrowNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            map := {"key" -> "value"}
    )SRC")
        .shouldParsed(false);
}

TEST_F(literalAndContainerAntipatternIntegTest, mixedMapArraySyntaxNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            arr := [1, 2, 3]
    )SRC")
        .shouldParsed(false);
}

TEST_F(literalAndContainerAntipatternIntegTest, emptyMapWithoutTypeNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            map := {}
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

// ============================================================================
// Category 24: Array Index Errors
// ============================================================================

TEST_F(literalAndContainerAntipatternIntegTest, negativeArrayIndexNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            arr := {1, 2, 3}
            val := arr[-1]
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(literalAndContainerAntipatternIntegTest, arrayIndexOutOfBoundsNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            arr := {1, 2, 3}
            val := arr[10]
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(literalAndContainerAntipatternIntegTest, doubleIndexingNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            arr := {1, 2, 3}
            val := arr[[0]]
    )SRC")
        .shouldParsed(false);
}

// ============================================================================
// Category 25: String Operation Errors
// ============================================================================

TEST_F(literalAndContainerAntipatternIntegTest, stringDivisionNegative) {
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

TEST_F(literalAndContainerAntipatternIntegTest, stringSubtractionNegative) {
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

TEST_F(literalAndContainerAntipatternIntegTest, stringMultiplicationWithStringNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := "hello"
            b := a * "world"
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}
