#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct assignExprIntegTest: public byeolIntegTest {};
}

TEST_F(assignExprIntegTest, simpleAssign) {
    if(make()
            .parse(R"SRC(
        age int
        main() int
            age = 5 # ret age implicitly
    )SRC")
            .shouldVerified(true)) {
        run();
        node& res = getSubPack() OR_ASSERT(res);

        const nInt& o = res.sub<nInt>("age") OR_ASSERT(o);
        ASSERT_EQ(*o.cast<nint>(), 5);
    }
}

TEST_F(assignExprIntegTest, simpleAssign1) {
    make()
        .parse(R"SRC(
        main() int
            ans := ""
            ans += "l"
            ans == "l"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(assignExprIntegTest, simpleAssign2) {
    make()
        .parse(R"SRC(
        main() int
            ans := "0"
            ans += "l"
            ret ans == "0l"
    )SRC")
        .shouldVerified(true);
}

TEST_F(assignExprIntegTest, simpleAssignReturn) {
    make()
        .parse(R"SRC(
        age int
        main() void
            age = 5
    )SRC")
        .shouldVerified(true);
}

TEST_F(assignExprIntegTest, assignLocalVariable) {
    if(make()
            .parse(R"SRC(
        age int
        main() int
            age = 5

            age int
            age = 3
            ret age
    )SRC")
            .shouldVerified(true)) {
        run();
        node& res = getSubPack() OR_ASSERT(res);

        const nInt& o = res.sub<nInt>("age") OR_ASSERT(o);
        ASSERT_EQ(*o.cast<int>(), 5);
    }
}

TEST_F(assignExprIntegTest, assignTypeNegative) {
    make()
        .negative()
        .parse(R"SRC(
        age int
        main() int
            ret age = "wow"
    )SRC")
        .shouldParsed(false);
}

TEST_F(assignExprIntegTest, mysteriousDeath) {
    make()
        .parse(R"SRC(
        age := 0
        main() int
            age = age + 1 # assignment is not expression but it can be returned.
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<int>(), 1);
}

TEST_F(assignExprIntegTest, assignClassNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def A
            foo() void
                ret

        main() void
            a A
            a = 5
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(assignExprIntegTest, assignClassNegative2) {
    make()
        .negative()
        .parse(R"SRC(
        def A
            foo() void
                ret

        main() void
            a := 25
            b A
            a = b
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(assignExprIntegTest, assignAssignedValueNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            a1 int
            a1 = a2 := 55
            ret a1
    )SRC")
        .shouldParsed(false);
}

TEST_F(assignExprIntegTest, assignAssignedValue) {
    make()
        .parse(R"SRC(
        main() int
            a1 int
            a2 := 55
            a1 = a2
            ret a1
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 55);
}

TEST_F(assignExprIntegTest, assignAssignedValue2) {
    make()
        .parse(R"SRC(
        main() int
            a1 int
            a2 int
            a2 = 55
            a1 = a2
            ret a1
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 55);
}

TEST_F(assignExprIntegTest, assignForExprDeclaringLocalVariable) {
    make()
        .parse(R"SRC(
        main() int
            abc int[]
            abc = for n in 0..5
                x := n + 1
                x * 2
            abc[3] # abc = {2, 4, 6, 8, 10}
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res->cast<nint>(), 8);
}

TEST_F(assignExprIntegTest, assignConstNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def person
            AGE := 33
            foo() void
                AGE = 22
                ret AGE
        main() int
            person.foo()
    )SRC")
        .shouldVerified(false);
}

// TODO: assignNegative inheritence
// TODO: assignDotChain: A.B.name
// TODO: assignComplexDotChain: B[A.foo() + 3].name
