#include "../../../namuSyntaxTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct whileExprTest: public namuSyntaxTest {};
}

TEST_F(whileExprTest, simple) {
    make()
        .parse(R"SRC(
        foo(msg str) void: 1
        main() int
            sum := 0
            n := 0
            while n <= 3
                sum = sum + n
                n = n + 1
                foo("sum=" + sum as str + ", n=" + n as str + "\n")
            ret sum
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 6);
}

TEST_F(whileExprTest, conditionClauseCheckNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            sum := 0
            n := 0
            while n <= foo()
                sum = sum + n
                n = n + 1
                print("sum=" + sum as str + ", n=" + n as str + "\n")
            ret sum
    )SRC")
        .shouldVerified(false);
}

TEST_F(whileExprTest, conditionClauseCheck) {
    make()
        .parse(R"SRC(
        print(msg str) void: 1

        foo() int
            ret 3

        main() int
            sum := 0
            n := 0
            while n <= foo()
                sum = sum + n
                n = n + 1
                print("sum=" + sum as str + ", n=" + n as str + "\n")
            ret sum
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 6);
}

TEST_F(whileExprTest, breakInsideOfFor) {
    make()
        .parse(R"SRC(
        main() int
            n := 0
            (while n < 5
                break
            ).len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 0);
}

TEST_F(whileExprTest, breakInsideOfIfExpr) {
    make()
        .parse(R"SRC(
        main() int
            n := 0
            (while n < 5
                if ++n == 3
                    break
                else
                    n
            )[1] // {1, 2}
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 2);
}

TEST_F(whileExprTest, breakNestedForLoop) {
    make()
        .parse(R"SRC(
        main() int
            sum := 0
            n := 0
            n2 := 0

            while ++n < 8
                while ++n2 < 4
                    ++sum
                    if sum > 15
                        break
                n2 = 0
                if sum > 15
                    break
            sum
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 16);
}

TEST_F(whileExprTest, evalOfForLoop) {
    make()
        .parse(R"SRC(
        main() int
            sum := 0
            n := 0
            (while ++n < 8
                if sum > 3
                    break
                sum += n
            )[2] // {1, 3, 6}
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 6);
}

TEST_F(whileExprTest, evalOfForLoop2) {
    make()
        .parse(R"SRC(
        main() int
            sum := 0
            n := 0
            while ++n < 8
                if sum > 3
                    break
                ++sum
            sum
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 4);
}
