#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct evalExprIntegTest: public byeolIntegTest {};
}

TEST_F(evalExprIntegTest, simpleRunCheck) {
    make()
        .parse(R"SRC(
        main() int
            me.main()
            ret 0
    )SRC")
        .shouldVerified(true);
}

TEST_F(evalExprIntegTest, runWithoutSpecifyingMe) {
    make()
        .parse(R"SRC(
        main() int
            main()
            ret 0
    )SRC")
        .shouldVerified(true);
}

TEST_F(evalExprIntegTest, runAnotherFunc) {
    make()
        .parse(R"SRC(
        foo() void
            ret
        main() void
            foo()
            ret
    )SRC")
        .shouldVerified(true);
}

TEST_F(evalExprIntegTest, runNotExistFuncNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            me.foo()
            ret 0
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(evalExprIntegTest, runAndReturn) {
    make()
        .parse(R"SRC(
        main() int
            main()
    )SRC")
        .shouldVerified(true); // TODO: verify this as warning of infinite reculsive.
}

TEST_F(evalExprIntegTest, ArgumentMismatchNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main(str arg) int
            main(3.5)
    )SRC")
        .shouldVerified(false);
}

TEST_F(evalExprIntegTest, runAndReturn2) {
    make()
        .parse(R"SRC(
        foo() flt
            ret 3.5
        main() int
            ret foo() == 3.5
    )SRC")
        .shouldVerified(true);
}

TEST_F(evalExprIntegTest, runAndReturn3) {
    make()
        .parse(R"SRC(
        foo() flt
            3.5
        main() int
            foo() == 3.5
    )SRC")
        .shouldVerified(true);
}

TEST_F(evalExprIntegTest, runWithArgument) {
    make()
        .parse(R"SRC(
        foo(age int, grade flt) flt
            ret grade
        main() int
            foo(22, 3.5) == 3.5
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);
}

TEST_F(evalExprIntegTest, runWithArgumentNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo(name str, grade flt) str
            ret name
        main() int
            foo("hello", 3.5)
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(evalExprIntegTest, runWithArgument2) {
    make()
        .negative()
        .parse(R"SRC(
        foo(name str, grade flt) int
            3
        make() int
            foo("hello", 3.5)
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);
}
