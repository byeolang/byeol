#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct typeAndDeclAntiTest: public byeolIntegTest {};
}

TEST_F(typeAndDeclAntiTest, paramTypeBeforeNameNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def foo(int a) void
            a + 1
        main() void: foo(5)
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntiTest, returnTypeBeforeFuncNameNegative) {
    make()
        .negative()
        .parse(R"SRC(
        int foo()
            ret 42
        main() void: foo()
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntiTest, propTypeBeforeNameNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def A
            int age
        main() void: 0
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntiTest, multipleParamTypesBeforeNamesNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def foo(int a, str b) void
            print(a as str + b)
        main() void: foo(5, "test")
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntiTest, partialParamTypeBeforeNameNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def foo(a int, int b) void
            a + b
        main() void: foo(1, 2)
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntiTest, assignWithoutDeclareNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a = 5
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(typeAndDeclAntiTest, doubleColonEqualNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := := 5
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntiTest, defAssignWithoutRhsNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a :=
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntiTest, chainedAssignmentNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := b := 5
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntiTest, asWithoutTypeNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 5
            b := a as
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntiTest, doubleAsNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 5
            b := a as as int
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntiTest, voidCastingNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 5
            b := a as void
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(typeAndDeclAntiTest, cStyleCastNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 5
            b := (int)a
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntiTest, isWithoutTypeNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 5
            if a is
                print("true")
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntiTest, twoStepImplicitCastNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo(a flt) void
            print(a)
        main() void
            val := 27 as byte
            foo(val)
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(typeAndDeclAntiTest, strImplicitCastNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo(a int) void
            print(a)
        main() void
            val := "123"
            foo(val)
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(typeAndDeclAntiTest, voidVariableNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a void
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(typeAndDeclAntiTest, voidArrayNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            arr void[]
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(typeAndDeclAntiTest, voidAssignmentNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := void
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(typeAndDeclAntiTest, voidReturnValueUseNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo() void
            ret
        main() void
            x := foo()
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

/* TODO:
TEST_F(typeAndDeclAntiTest, voidMapNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            map void[str]
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}*/
