#include "test/namuSyntaxTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct endExprTest: public namuSyntaxTest {};
}

TEST_F(endExprTest, simpleEnd) {
    make()
        .parse(R"SRC(
        age := 33
        foo() int
            end: age++
            ret 5
        main() int: foo() + age
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 39);
}

TEST_F(endExprTest, simpleEndNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def obj
            age := 22
            end
                age++
        main() int
            obj.age
    )SRC")
        .shouldParsed(false);
    shouldVerified(false);
}

/* TODO: uncomment this when 'end' can capture scope.
TEST_F(endExprTest, endUsesCapture) {
    make().parse(R"SRC(
        def person
            age := 0
            foo() void
                end: me.age++

                age := 33
                end: me.age += age

                if true
                    age := 5
                    end: me.age += age

        main() int
            person.foo()
            person.age
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 39);
}*/
