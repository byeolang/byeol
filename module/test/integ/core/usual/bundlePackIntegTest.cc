#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct bundlePackIntegTest: public byeolIntegTest {};
}

TEST_F(bundlePackIntegTest, defaultDef) {
    make()
        .parse(R"SRC(
        print(msg str) void: 1

        main() void
            print("hello \\tworld!\n")
    )SRC")
        .shouldVerified(true);
    run();
}

TEST_F(bundlePackIntegTest, defaultDef2) {
    make()
        .parse(R"SRC(
        print(msg str) void: 1

        msg := "hello\t"
        main() void
            print(msg + "world!\n")
    )SRC")
        .shouldVerified(true);
    run();
}

TEST_F(bundlePackIntegTest, withAsNegative) {
    make()
        .negative()
        .parse(R"SRC(
        Helloworld(age int) int
            ret age

        main() int
            a := Helloworld("g")
            print(a as str)
            ret 0
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(bundlePackIntegTest, withAs) {
    make()
        .parse(R"SRC(
        print(msg str) void: 1

        Helloworld(age int) int
            ret age

        main() int
            a := Helloworld("g" as byte)
            print(a as str)
            ret 0
    )SRC")
        .shouldVerified(true);
    run();
}

TEST_F(bundlePackIntegTest, defaultDef3) {
    make()
        .parse(R"SRC(
        print(msg str) void: 1

        age int
        ge int

        main() int
            age = 48268
            ge = age + 3985
            print(age as str + " \n")
            print(ge as str)
            ret ge
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<int>(), 48268 + 3985);
}

TEST_F(bundlePackIntegTest, cloneBridgeObj) {
    make()
        .parse(R"SRC(
        main() int
            s := sys.con()
            ret s.add(1, 2)
    )SRC")
        .shouldVerified(true);
}
