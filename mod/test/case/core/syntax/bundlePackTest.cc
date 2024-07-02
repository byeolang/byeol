#include "../../../namuSyntaxTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct bundlePackTest : public namuSyntaxTest {};
}

TEST_F(bundlePackTest, defaultDef) {
    make().parse(R"SRC(
        main() void
            print("hello \\tworld!\n")
    )SRC").shouldVerified(true);
    run();
}

TEST_F(bundlePackTest, defaultDef2) {
    make().parse(R"SRC(
        msg := "hello\t"
        main() void
            print(msg + "world!\n")
    )SRC").shouldVerified(true);
    run();
}

TEST_F(bundlePackTest, withAsNegative) {
    make().negative().parse(R"SRC(
        Helloworld(age int) int
            ret age

        main() int
            a := Helloworld("g")
            print(a as str)
            ret 0
    )SRC").shouldParsed(true);
    shouldVerified(false);
}

TEST_F(bundlePackTest, withAs) {
    make().parse(R"SRC(
        Helloworld(age int) int
            ret age

        main() int
            a := Helloworld('g' as int)
            print(a as str)
            ret 0
    )SRC").shouldVerified(true);
    run();
}

TEST_F(bundlePackTest, defaultDef3) {
    make().parse(R"SRC(
        age int
        ge int

        main() int
            age = 48268
            ge = age + 3985
            print(age as str + " \n")
            print(ge as str)
            ret ge
    )SRC").shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<int>(), 48268 + 3985);
}
