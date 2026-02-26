#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct typeAndDeclAntipatternIntegTest: public byeolIntegTest {};
}

// ============================================================================
// Category 1: Type Declaration Order
// ============================================================================

TEST_F(typeAndDeclAntipatternIntegTest, paramTypeBeforeNameNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def foo(int a) void
            a + 1
        main() void: foo(5)
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntipatternIntegTest, returnTypeBeforeFuncNameNegative) {
    make()
        .negative()
        .parse(R"SRC(
        int foo()
            ret 42
        main() void: foo()
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntipatternIntegTest, propTypeBeforeNameNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def A
            int age
        main() void: 0
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntipatternIntegTest, multipleParamTypesBeforeNamesNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def foo(int a, str b) void
            print(a as str + b)
        main() void: foo(5, "test")
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntipatternIntegTest, partialParamTypeBeforeNameNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def foo(a int, int b) void
            a + b
        main() void: foo(1, 2)
    )SRC")
        .shouldParsed(false);
}

// ============================================================================
// Category 2: Variable Declaration vs Assignment
// ============================================================================

TEST_F(typeAndDeclAntipatternIntegTest, assignWithoutDeclareNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a = 5
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(typeAndDeclAntipatternIntegTest, doubleColonEqualNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := := 5
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntipatternIntegTest, defAssignWithoutRhsNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a :=
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntipatternIntegTest, chainedAssignmentNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := b := 5
    )SRC")
        .shouldParsed(false);
}

// ============================================================================
// Category 9: Type Casting Errors
// ============================================================================

TEST_F(typeAndDeclAntipatternIntegTest, asWithoutTypeNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 5
            b := a as
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntipatternIntegTest, doubleAsNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 5
            b := a as as int
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntipatternIntegTest, voidCastingNegative) {
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

TEST_F(typeAndDeclAntipatternIntegTest, cStyleCastNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 5
            b := (int)a
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntipatternIntegTest, isWithoutTypeNegative) {
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

// ============================================================================
// Category 20: Implicit Casting Misunderstanding
// ============================================================================

TEST_F(typeAndDeclAntipatternIntegTest, twoStepImplicitCastNegative) {
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

TEST_F(typeAndDeclAntipatternIntegTest, strImplicitCastNegative) {
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

// ============================================================================
// Category 23: Void Type Misuse
// ============================================================================

TEST_F(typeAndDeclAntipatternIntegTest, voidVariableNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a void
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntipatternIntegTest, voidArrayNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            arr void[]
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntipatternIntegTest, voidAssignmentNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := void
    )SRC")
        .shouldParsed(false);
}

TEST_F(typeAndDeclAntipatternIntegTest, voidReturnValueUseNegative) {
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

TEST_F(typeAndDeclAntipatternIntegTest, voidMapNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            map void[str]
    )SRC")
        .shouldParsed(false);
}
