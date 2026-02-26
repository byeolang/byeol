#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct operatorAntipatternIntegTest: public byeolIntegTest {};
}

// ============================================================================
// Category 6: Operator Errors
// ============================================================================

TEST_F(operatorAntipatternIntegTest, doublePrefixIncrementNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 5
            b := ++a++
    )SRC")
        .shouldParsed(false);
}

TEST_F(operatorAntipatternIntegTest, doublePostfixDecrementNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 5
            b := --a--
    )SRC")
        .shouldParsed(false);
}

TEST_F(operatorAntipatternIntegTest, tripleMinusOperatorNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 5
            b := a --- 3
    )SRC")
        .shouldParsed(false);
}

TEST_F(operatorAntipatternIntegTest, tripleStarOperatorNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 2
            b := a *** 3
    )SRC")
        .shouldParsed(false);
}

TEST_F(operatorAntipatternIntegTest, referenceEqualityOnPrimitiveNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 5
            b := 5
            if a === b
                print("same")
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

// ============================================================================
// Category 10: Access Errors
// ============================================================================

TEST_F(operatorAntipatternIntegTest, dotAtStartNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def obj
            age := 0
        main() void
            .obj
    )SRC")
        .shouldParsed(false);
}

TEST_F(operatorAntipatternIntegTest, dotAtEndNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def obj
            age := 0
        main() void
            o := obj()
            o.
    )SRC")
        .shouldParsed(false);
}

TEST_F(operatorAntipatternIntegTest, tripleDotsAccessNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def obj
            age := 0
        main() void
            obj...age
    )SRC")
        .shouldParsed(false);
}

// ============================================================================
// Category 16: me Keyword Errors
// ============================================================================

TEST_F(operatorAntipatternIntegTest, capitalMeKeywordNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def Person
            age := 0
            getAge() int
                ret Me.age
        main() void: Person().getAge()
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(operatorAntipatternIntegTest, meOutsideObjectScopeNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := me.age
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

// ============================================================================
// Category 18: Scope Errors
// ============================================================================

TEST_F(operatorAntipatternIntegTest, doubleColonScopeOperatorNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def obj
            age := 0
        main() void
            obj::age
    )SRC")
        .shouldParsed(false);
}

TEST_F(operatorAntipatternIntegTest, nestedAccessWithoutProperPathNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def Outer
            def Inner
                val := 0
        main() void
            x := Outer.Inner.val
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

// ============================================================================
// Category 21: Operator Precedence Errors
// ============================================================================

TEST_F(operatorAntipatternIntegTest, bitwiseAndLogicalMixNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 5
            b := 3
            if a & 1 == 1
                print("odd")
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(operatorAntipatternIntegTest, assignmentInConditionNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 0
            if a = 5
                print("assigned")
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}
