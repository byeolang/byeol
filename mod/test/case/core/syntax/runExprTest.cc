#include "../../../namuSyntaxTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct runExprTest : public namuSyntaxTest {};
}

TEST_F(runExprTest, simpleRunCheck) {
    make().parse(R"SRC(
        main() int
            me.main()
            ret 0
    )SRC").shouldVerified(true);
}

TEST_F(runExprTest, runWithoutSpecifyingMe) {
    make().parse(R"SRC(
        main() int
            main()
            ret 0
    )SRC").shouldVerified(true);
}

TEST_F(runExprTest, runAnotherFunc) {
    make().parse(R"SRC(
        foo() void
            ret
        main() void
            foo()
            ret
    )SRC").shouldVerified(true);
}

TEST_F(runExprTest, runNotExistFuncNegative) {
    make().negative().parse(R"SRC(
        main() int
            me.foo()
            ret 0
    )SRC").shouldParsed(true);
    shouldVerified(false);
}

TEST_F(runExprTest, runAndReturn) {
    make().parse(R"SRC(
        main() int
            main()
    )SRC").shouldVerified(true); // TODO: verify this as warning of infinite reculsive.
}

TEST_F(runExprTest, ArgumentMismatchNegative) {
    make().negative().parse(R"SRC(
        main(str arg) int
            main(3.5)
    )SRC").shouldVerified(false);
}

TEST_F(runExprTest, runAndReturn2) {
    make().parse(R"SRC(
        foo() flt
            ret 3.5
        main() int
            ret foo() == 3.5
    )SRC").shouldVerified(true);
}

TEST_F(runExprTest, runAndReturn3) {
    make().parse(R"SRC(
        foo() flt
            3.5
        main() int
            foo() == 3.5
    )SRC").shouldVerified(true);
}

TEST_F(runExprTest, runWithArgument) {
    make().parse(R"SRC(
        foo(age int, grade flt) flt
            ret grade
        main() int
            foo(22, 3.5) == 3.5
    )SRC").shouldParsed(true);
    shouldVerified(true);
}

TEST_F(runExprTest, runWithArgumentNegative) {
    make().negative().parse(R"SRC(
        foo(name str, grade flt) str
            ret name
        main() int
            foo("hello", 3.5)
    )SRC").shouldParsed(true);
    shouldVerified(false);
}

TEST_F(runExprTest, runWithArgument2) {
    make().negative().parse(R"SRC(
        foo(name str, grade flt) int
            3
        make() int
            foo("hello", 3.5)
    )SRC").shouldParsed(true);
    shouldVerified(true);
}
