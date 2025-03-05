#include "test/byeolIntegTest.hpp"

using namespace std;
using namespace by;

namespace {
    struct stringTemplateIntegTest: public byeolIntegTest {};
}

TEST_F(stringTemplateIntegTest, simpleTest) {
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
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(stringTemplateIntegTest, testWithExpr) {
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
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(stringTemplateIntegTest, stringTemplateDoubleDollar) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            name := "world"
            msg := "hello $$name"
            ret msg.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 12); // hello $world
}

TEST_F(stringTemplateIntegTest, stringTemplateMissingVar) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            msg := "hello $"
            ret msg.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 7);
}
