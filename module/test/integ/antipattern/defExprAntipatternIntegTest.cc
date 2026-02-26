#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct defExprAntipatternIntegTest: public byeolIntegTest {};
}

// ============================================================================
// Category 8: Def Expression Errors
// ============================================================================

TEST_F(defExprAntipatternIntegTest, callCompleteWithExpressionNegative) {
    // BUG: Parser should reject expression in callComplete, but currently accepts it
    make()
        .negative()
        .parse(R"SRC(
        def myObj(a + b)
            age int
        main() void: 0
    )SRC")
        .shouldParsed(false);
}

TEST_F(defExprAntipatternIntegTest, functionWithoutBodyNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def foo()
        main() void: 0
    )SRC")
        .shouldParsed(false);
}

// ============================================================================
// Category 14: Lambda Syntax Errors
// ============================================================================

TEST_F(defExprAntipatternIntegTest, lambdaColonWrongPositionNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            arr := {1, 2, 3}
            arr.each((n:) print(n))
    )SRC")
        .shouldParsed(false);
}

// ============================================================================
// Category 17: Constructor Errors
// ============================================================================

TEST_F(defExprAntipatternIntegTest, pythonInitNegative) {
    // BUG: Verifier should reject __init__ (not a Byeol constructor name), but accepts it
    make()
        .negative()
        .parse(R"SRC(
        def Person
            name str
            __init__(n str): name = n
        main() void: 0
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defExprAntipatternIntegTest, ctorWithReturnTypeNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def Person
            name str
            ctor(n str) void: name = n
        main() void: 0
    )SRC")
        .shouldParsed(false);
}

// ============================================================================
// Category 19: Generic Errors
// ============================================================================

TEST_F(defExprAntipatternIntegTest, duplicateTypeParameterNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def MyGeneric<T, T>
            val T
        main() void: 0
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defExprAntipatternIntegTest, emptyTypeParameterNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def MyGeneric<>
            val int
        main() void: 0
    )SRC")
        .shouldParsed(false);
}
