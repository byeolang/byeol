#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct syntaxAntipatternIntegTest: public byeolIntegTest {};
}

// Category 1: Type Declaration Order - C++ style parameter
TEST_F(syntaxAntipatternIntegTest, paramTypeBeforeNameNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def foo(int a) void
            ret a + 1
        main() void
            foo(5)
    )SRC")
        .shouldParsed(false);
}

// Category 2: Variable Declaration - := duplication
TEST_F(syntaxAntipatternIntegTest, doubleColonEqualNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := := 5
    )SRC")
        .shouldParsed(false);
}

// Category 3: Unnecessary Parentheses - if condition
TEST_F(syntaxAntipatternIntegTest, ifWithParenthesesNegative) {
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

// Category 4: Block Syntax - double colon
TEST_F(syntaxAntipatternIntegTest, doubleColonBlockNegative) {
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

// Category 5: String Template - $$ duplication
TEST_F(syntaxAntipatternIntegTest, stringTemplateDoubleDollarNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            name := "world"
            msg := "$$name"
    )SRC")
        .shouldParsed(true)
        .shouldVerified(false);
}

// Category 6: Operator Errors - +++ operator
TEST_F(syntaxAntipatternIntegTest, tripleIncrementOperatorNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 5
            b := a +++ 3
    )SRC")
        .shouldParsed(false);
}

// Category 10: Access Errors - double dots
TEST_F(syntaxAntipatternIntegTest, doubleDotsAccessNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def myObj
            age := 5
        main() void
            myObj..age
    )SRC")
        .shouldParsed(false);
}

// Category 15: Return Statement - return keyword
TEST_F(syntaxAntipatternIntegTest, returnKeywordNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def foo() int
            return 5
        main() void
            foo()
    )SRC")
        .shouldParsed(false);
}

// Category 16: Me Keyword - this keyword
TEST_F(syntaxAntipatternIntegTest, thisKeywordNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def Person
            age := 0
            getAge() int
                ret this.age
        main() void
            Person().getAge()
    )SRC")
        .shouldParsed(false);
}

// Category 22: Loop Control - continue keyword
TEST_F(syntaxAntipatternIntegTest, continueKeywordNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            arr := {1, 2, 3}
            for n in arr
                if n == 2
                    continue
                print(n as str)
    )SRC")
        .shouldParsed(false);
}
