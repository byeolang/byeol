#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct controlFlowAntiTest: public byeolIntegTest {};
}

TEST_F(controlFlowAntiTest, forWithParenthesesNegative) {
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

TEST_F(controlFlowAntiTest, doubleColonBlockNegative) {
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

TEST_F(controlFlowAntiTest, emptyBlockWithoutSemicolonNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            if true:
    )SRC")
        .shouldParsed(false);
}

TEST_F(controlFlowAntiTest, curlyBracesBlockNegative) {
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

TEST_F(controlFlowAntiTest, tripleSemicolonNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            ;;;
    )SRC")
        .shouldParsed(false);
}

TEST_F(controlFlowAntiTest, cStyleForNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            for(int i=0; i<10; i++)
                print(i as str)
    )SRC")
        .shouldParsed(false);
}

TEST_F(controlFlowAntiTest, forWithoutDefAssignNegative) {
    // BUG: Parser should require := in for loop (for n := in ...), but accepts without it
    make()
        .negative()
        .parse(R"SRC(
        main() void
            for n := 0 in 1..10
                print(n as str)
    )SRC")
        .shouldParsed(false);
}

TEST_F(controlFlowAntiTest, doubleRetNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo() int
            ret ret 5
        main() void: foo()
    )SRC")
        .shouldParsed(false);
}

TEST_F(controlFlowAntiTest, retWithUnnecessarySemicolonNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo() int
            ret 42;
        main() void: foo()
    )SRC")
        .shouldParsed(false);
}

TEST_F(controlFlowAntiTest, breakWithValueNegative) {
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

TEST_F(controlFlowAntiTest, nextWithValueNegative) {
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
