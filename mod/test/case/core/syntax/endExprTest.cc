#include "../../../namuSyntaxTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct endExprTest: public namuSyntaxTest {};
}

TEST_F(endExprTest, simpleEnd) {
    make().parse(R"SRC(
        age := 33
        foo() int
            end: age++
            ret 5
        main() int: foo() + age
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 39);
}
