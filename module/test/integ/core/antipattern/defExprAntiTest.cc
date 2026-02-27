#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct defExprAntiTest: public byeolIntegTest {};
}

TEST_F(defExprAntiTest, callCompleteWithExpressionNegative) {
    // BUG: Valid syntax, but a, b are not variables.
    make()
        .negative()
        .parse(R"SRC(
        def myObj(a + b)
            age int
        main() void: 0
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defExprAntiTest, defObjWithoutBodyNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def foo
        main() void: 0
    )SRC")
        .shouldParsed(false);
}

TEST_F(defExprAntiTest, lambdaColonWrongPositionNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            arr := {1, 2, 3}
            arr.each((n:) print(n))
    )SRC")
        .shouldParsed(false);
}

TEST_F(defExprAntiTest, ctorWithReturnTypeNegative) {
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

TEST_F(defExprAntiTest, ctorWithReturnNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def Person
            name str
            ctor(n str)
                if n.len() <= 1
                    ret # <- err
                name = n
        main() void
            print(Person("wow").name)
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defExprAntiTest, duplicateTypeParameterNegative) {
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

TEST_F(defExprAntiTest, emptyTypeParameterNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def MyGeneric<>
            val int
        main() void: 0
    )SRC")
        .shouldParsed(false);
}
