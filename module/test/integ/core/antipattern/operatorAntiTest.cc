#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct operatorAntiTest: public byeolIntegTest {};
}

TEST_F(operatorAntiTest, doublePrefixIncrementNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 5
            b := ++a++
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(operatorAntiTest, doublePostfixDecrementNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 5
            b := --a--
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(operatorAntiTest, tripleStarOperatorNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 2
            b := a *** 3
    )SRC")
        .shouldParsed(false);
}

TEST_F(operatorAntiTest, dotAtStartNegative) {
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

TEST_F(operatorAntiTest, dotAtEndNegative) {
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

TEST_F(operatorAntiTest, tripleDotsAccessNegative) {
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

TEST_F(operatorAntiTest, capitalMeKeywordNegative) {
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

TEST_F(operatorAntiTest, meOutsideObjectScopeNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := me.age
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(operatorAntiTest, bitwiseAndLogicalMixNegative) {
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

TEST_F(operatorAntiTest, assignmentInConditionNegative) {
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
