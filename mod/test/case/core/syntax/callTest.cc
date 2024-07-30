#include "../../../namuSyntaxTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct callTest: public namuSyntaxTest {};
}

TEST_F(callTest, callFunc) {
    make("demo")
        .negative()
        .parse(R"SRC(
        pack demo
        main() void
            foo()
    )SRC")
        .shouldParsed(true);
}

TEST_F(callTest, callFunc1) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a.foo()
    )SRC")
        .shouldParsed(true);
}

TEST_F(callTest, callFunc2) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            --a++.foo()
    )SRC")
        .shouldParsed(true);
}

TEST_F(callTest, callFunc3) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            (--a++.foo()).doo().goo()
    )SRC")
        .shouldParsed(true);
}

TEST_F(callTest, callFuncShouldNotExistOnPack) {
    make()
        .negative()
        .parse(R"SRC(
        foo()
    )SRC")
        .shouldParsed(false);
}

TEST_F(callTest, callFuncWithExprInside) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a.foo(22, 34)
    )SRC")
        .shouldParsed(true);
}

TEST_F(callTest, callFuncWithExprInside1) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            if "test"
                a.foo(22, 34, boo(22))
    )SRC")
        .shouldParsed(true);
}

TEST_F(callTest, callFuncWithExprInside2) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            if "test"
                a.foo(22, 34, (a int) void
                    b.boo(a)
                )
    )SRC")
        .shouldParsed(true);
}

TEST_F(callTest, callFuncWithExprInside3) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            if "test"
                a.foo(22, 34, ((a int) void
                    b.boo(a)
                ))
    )SRC")
        .shouldParsed(false);
}

TEST_F(callTest, callObjCtor) {
    make()
        .parse(R"SRC(
        main() int
            a := 55
            a -= 20
            a(a)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 35);
}
