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

TEST_F(defFuncAntiTest, unregisteredMagicFuncInClassNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def Person
            name str
            @____invalid(n str): name = n
        main() void: 0
    )SRC")
        .shouldParsed(false);
}

TEST_F(defFuncAntiTest, unregisteredMagicFuncGlobalNegative) {
    make()
        .negative()
        .parse(R"SRC(
        @____notExist() void: 1
        main() void: 0
    )SRC")
        .shouldParsed(false);
}

TEST_F(defFuncAntiTest, unregisteredMagicFuncCallNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def MyClass
            foo() void: 1
        main() void
            obj := MyClass()
            obj.@____unknown()
    )SRC")
        .shouldParsed(false);
}
