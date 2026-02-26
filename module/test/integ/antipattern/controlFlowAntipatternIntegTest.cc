#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct controlFlowAntipatternIntegTest: public byeolIntegTest {};
}

// ============================================================================
// Category 3: Unnecessary Parentheses
// ============================================================================

TEST_F(controlFlowAntipatternIntegTest, ifWithParenthesesNegative) {
    // BUG: Parser should reject parentheses in if condition, but currently accepts it
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 5
            if (a == 5)
                print("yes")
    )SRC")
        .shouldParsed(false);
}

TEST_F(controlFlowAntipatternIntegTest, whileWithParenthesesNegative) {
    // BUG: Parser should reject parentheses in while condition, but currently accepts it
    make()
        .negative()
        .parse(R"SRC(
        main() void
            n := 0
            while (n < 5)
                n++
    )SRC")
        .shouldParsed(false);
}

TEST_F(controlFlowAntipatternIntegTest, forWithParenthesesNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            arr := {1, 2, 3}
            for (n in arr)
                print(n as str)
    )SRC")
        .shouldParsed(false);
}

// ============================================================================
// Category 4: Block Syntax Errors
// ============================================================================

TEST_F(controlFlowAntipatternIntegTest, doubleColonBlockNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def foo() void::
            ret
        main() void
            foo()
    )SRC")
        .shouldParsed(false);
}

TEST_F(controlFlowAntipatternIntegTest, emptyBlockWithoutSemicolonNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            if true:
    )SRC")
        .shouldParsed(false);
}

TEST_F(controlFlowAntipatternIntegTest, curlyBracesBlockNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def obj {
            age := 0
        }
        main() void: 0
    )SRC")
        .shouldParsed(false);
}

TEST_F(controlFlowAntipatternIntegTest, tripleSemicolonNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            ;;;
    )SRC")
        .shouldParsed(false);
}

// ============================================================================
// Category 7: Loop Syntax Errors
// ============================================================================

TEST_F(controlFlowAntipatternIntegTest, cStyleForNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            for(int i=0; i<10; i++)
                print(i as str)
    )SRC")
        .shouldParsed(false);
}

TEST_F(controlFlowAntipatternIntegTest, forWithoutDefAssignNegative) {
    // BUG: Parser should require := in for loop (for n := in ...), but accepts without it
    make()
        .negative()
        .parse(R"SRC(
        main() void
            for n in 1..10
                print(n as str)
    )SRC")
        .shouldParsed(false);
}

TEST_F(controlFlowAntipatternIntegTest, doWhileNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            n := 0
            do
                n++
            while n < 5
    )SRC")
        .shouldParsed(false);
}

// ============================================================================
// Category 15: ret Statement Errors
// ============================================================================

TEST_F(controlFlowAntipatternIntegTest, doubleRetNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo() int
            ret ret 5
        main() void: foo()
    )SRC")
        .shouldParsed(false);
}

TEST_F(controlFlowAntipatternIntegTest, retWithUnnecessarySemicolonNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo() int
            ret 42;
        main() void: foo()
    )SRC")
        .shouldParsed(false);
}

// ============================================================================
// Category 22: Loop Control Errors
// ============================================================================

TEST_F(controlFlowAntipatternIntegTest, breakWithValueNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            for n in 1..10
                if n == 5
                    break 5
    )SRC")
        .shouldParsed(false);
}

TEST_F(controlFlowAntipatternIntegTest, nextWithValueNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            for n in 1..10
                if n == 3
                    next 3
    )SRC")
        .shouldParsed(false);
}
