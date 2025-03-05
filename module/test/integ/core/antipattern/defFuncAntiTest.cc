#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct defFuncAntiTest: public byeolIntegTest {};
}

TEST_F(defFuncAntiTest, voidFuncWithRetValueNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo() int
            ret 22
        main() void
            ret foo()
    )SRC")
        .shouldVerified(false);
}
