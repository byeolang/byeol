#include "test/byeolSyntaxTest.hpp"

using namespace nm;

namespace {
    struct patternMatchingTest: public byeolSyntaxTest {};
}

TEST_F(patternMatchingTest, simpleIn) {
    make()
        .parse(R"SRC(
        main() int
            arr := {1, 2, 3}
            3 in arr
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res->cast<nint>(), 1);
}

TEST_F(patternMatchingTest, strNegative) {
    make()
        .parse(R"SRC(
        main() int
            arr := {"1", "2", "3"}
            "hello" in arr
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res->cast<nint>(), 0);
}

TEST_F(patternMatchingTest, wrongTypeNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            arr := {"1", "2", "3"}
            true in arr
    )SRC")
        .shouldVerified(false);
}

TEST_F(patternMatchingTest, withStr) {
    make()
        .parse(R"SRC(
        main() int
            arr := {"1", "2", "3"}
            "2" in arr
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res->cast<nint>(), 1);
}
