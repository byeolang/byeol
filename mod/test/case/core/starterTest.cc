#include "../../namuSyntaxTest.hpp"

using namespace nm;

struct starterTest: public namuSyntaxTest {};

TEST_F(starterTest, simpleStarter) {
    make().parse(R"SRC(
        main() void
            ret
    )SRC");
    shouldVerified(true);

    str res = starter().setPack(getSlot().getPack()).work();
    ASSERT_EQ(*res, nVoid());
}

TEST_F(starterTest, mainReturnInt) {
    make()
        .parse(R"SRC(
        main() int
            ret 23
    )SRC")
        .shouldVerified(true);

    tstr<nInt> res = starter().setPack(getSlot().getPack()).work();
    ASSERT_EQ(res->cast<nInt>().get(), 23);
    ASSERT_EQ(res->cast<int>(), 23);
}

TEST_F(starterTest, mainCallFuncAndCheckReturn) {
    make()
        .parse(R"SRC(
        foo() int
            ret 23
        main() int
            ret foo()
    )SRC")
        .shouldVerified(true);

    tstr<nInt> res = starter().setPack(getSlot().getPack()).work();
    ASSERT_EQ(res->cast<nInt>().get(), 23);
    ASSERT_EQ(res->cast<int>(), 23);
}
