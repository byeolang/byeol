#include "test/namuSyntaxTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct isExprTest: public namuSyntaxTest {};
}

TEST_F(isExprTest, simpleIs) {
    make()
        .parse(R"SRC(
        def A
            age := 0
        main() int
            A().age is flt
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res->cast<nint>(), 1);
}
