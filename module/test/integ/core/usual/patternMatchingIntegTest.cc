#include "test/byeolIntegTest.hpp"

using namespace by;

namespace {
    struct patternMatchingIntegTest: public byeolIntegTest {};
}

TEST_F(patternMatchingIntegTest, simpleIn) {
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

TEST_F(patternMatchingIntegTest, strNegative) {
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

TEST_F(patternMatchingIntegTest, wrongTypeNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            arr := {"1", "2", "3"}
            true in arr
    )SRC")
        .shouldVerified(false);
}

TEST_F(patternMatchingIntegTest, withStr) {
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
