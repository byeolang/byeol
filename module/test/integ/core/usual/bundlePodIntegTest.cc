#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct bundlePodIntegTest: public byeolIntegTest {};
}

TEST_F(bundlePodIntegTest, defaultDef) {
    make()
        .parse(R"SRC(
        print(msg str) void: 1

        main() void
            print("hello \\tworld!\n")
    )SRC")
        .shouldVerified(true);
    run();
}

TEST_F(bundlePodIntegTest, defaultDef2) {
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

TEST_F(bundlePodIntegTest, withAsNegative) {
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

TEST_F(bundlePodIntegTest, withAs) {
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

TEST_F(bundlePodIntegTest, defaultDef3) {
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

TEST_F(bundlePodIntegTest, cloneBridgeObj) {
    make()
        .parse(R"SRC(
        main() int
            s := sys.con()
            ret s.add(1, 2)
    )SRC")
        .shouldVerified(true);
}

TEST_F(bundlePodIntegTest, testWrapper) {
    make()
        .parse(R"SRC(
        wrapper := sys.wrapper
        main() int
             wrapper.multiplyToAdd(2, 3)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), (2 + 3) * 2);
}
