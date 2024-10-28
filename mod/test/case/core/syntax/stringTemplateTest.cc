#include "../../../namuSyntaxTest.hpp"

using namespace std;
using namespace nm;

namespace {
    struct stringTemplateTest: public namuSyntaxTest {};
}

TEST_F(stringTemplateTest, simpleTest) {
    make().parse(R"SRC(
        foo(value int) str
            "your value is $value"
        main() int
            foo(18) == "your value is 18"
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->isSub<nInt>());
    ASSERT_EQ(res.cast<nint>(), 1);
}
