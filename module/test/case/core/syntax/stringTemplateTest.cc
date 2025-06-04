#include "test/namuSyntaxTest.hpp"

using namespace std;
using namespace nm;

namespace {
    struct stringTemplateTest: public namuSyntaxTest {};
}

TEST_F(stringTemplateTest, simpleTest) {
    make()
        .parse(R"SRC(
        foo(value int) str
            "your value is $value"
        main() int
            foo(18) == "your value is 18"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->isSub<nInt>());
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(stringTemplateTest, testWithExpr) {
    make()
        .parse(R"SRC(
        def person
            age := 232
            race := "elf"
            hello() str
                "I'm $age yo, but still a child among ${race + "s"}."
        main() int
            person.hello() == "I'm 232 yo, but still a child among elfs."
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}
