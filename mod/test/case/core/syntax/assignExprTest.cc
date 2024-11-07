#include "../../../namuSyntaxTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct assignExprTest: public namuSyntaxTest {};
}

TEST_F(assignExprTest, simpleAssign) {
    if(make()
            .parse(R"SRC(
        age int
        main() int
            age = 5 # ret age implicitly
    )SRC")
            .shouldVerified(true)) {
        run();
        node& res = getSubPack();
        ASSERT_FALSE(nul(res));

        const nInt& o = res.sub<nInt>("age");
        ASSERT_FALSE(nul(o));
        ASSERT_EQ(o.cast<nint>(), 5);
    }
}

TEST_F(assignExprTest, simpleAssign1) {
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
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(assignExprTest, simpleAssign2) {
    make()
        .parse(R"SRC(
        main() int
            ans := "0"
            ans += "l"
            ret ans == "0l"
    )SRC")
        .shouldVerified(true);
}

TEST_F(assignExprTest, simpleAssignReturn) {
    make()
        .parse(R"SRC(
        age int
        main() void
            age = 5
    )SRC")
        .shouldVerified(true);
}

TEST_F(assignExprTest, assignLocalVariable) {
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
        node& res = getSubPack();
        ASSERT_FALSE(nul(res));

        const nInt& o = res.sub<nInt>("age");
        ASSERT_FALSE(nul(o));
        ASSERT_EQ(o.cast<int>(), 5);
    }
}

TEST_F(assignExprTest, assignTypeNegative) {
    make()
        .negative()
        .parse(R"SRC(
        age int
        main() int
            ret age = "wow"
    )SRC")
        .shouldParsed(false);
}

TEST_F(assignExprTest, mysteriousDeath) {
    make()
        .parse(R"SRC(
        age := 0
        main() int
            age = age + 1 # assignment is not expression but it can be returned.
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<int>(), 1);
}

TEST_F(assignExprTest, assignClassNegative) {
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

TEST_F(assignExprTest, assignClassNegative2) {
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

TEST_F(assignExprTest, assignAssignedValueNegative) {
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

TEST_F(assignExprTest, assignAssignedValue) {
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
    ASSERT_EQ(res.cast<nint>(), 55);
}

TEST_F(assignExprTest, assignAssignedValue2) {
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
    ASSERT_EQ(res.cast<nint>(), 55);
}

TEST_F(assignExprTest, assignForExprDeclaringLocalVariable) {
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
    ASSERT_EQ(res->cast<nint>(), 8);
}

TEST_F(assignExprTest, assignConstNegative) {
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
