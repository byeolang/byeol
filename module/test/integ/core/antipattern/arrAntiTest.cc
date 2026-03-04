#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct arrAntiTest : public byeolIntegTest {};  
}

TEST_F(arrAntiTest, negativeArrayIndexNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            arr := {1, 2, 3}
            val := arr[-1]
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    nerr& resErr = res.cast<nerr>() OR_ASSERT(resErr);
    ASSERT_EQ(resErr.getErrCode(), errCode::OUT_OF_RANGE);
}

TEST_F(arrAntiTest, arrayIndexOutOfBoundsNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            arr := {1, 2, 3}
            val := arr[10]
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    nerr& resErr = res.cast<nerr>() OR_ASSERT(resErr);
    ASSERT_EQ(resErr.getErrCode(), errCode::OUT_OF_RANGE);
}

TEST_F(arrAntiTest, emptyArrayWithoutType) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            arr := {}
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}
