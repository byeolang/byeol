#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct asExprIntegTest: public byeolIntegTest {};
}

TEST_F(asExprIntegTest, castInt) {
    make()
        .parse(R"SRC(
        foo(age int) void
            main()
        main() void
            foo(23)
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprIntegTest, castInt1) {
    make()
        .parse(R"SRC(
        foo(age flt) void
            main()
        main() void
            foo(23)
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprIntegTest, castInt2) {
    make()
        .parse(R"SRC(
        foo(age bool) void
            main()
        main() void
            foo(23)
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprIntegTest, castInt3) {
    make()
        .parse(R"SRC(
        foo(age byte) void
            main()
        main() void
            foo(23)
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprIntegTest, castInt4) {
    make()
        .negative()
        .parse(R"SRC(
        foo(age str) void
            main()
        main() void
            foo(23)
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(asExprIntegTest, castFlt) {
    make()
        .parse(R"SRC(
        foo(age int) void
            main()
        main() void
            foo(23.0)
    )SRC")
        .shouldVerified(true);

    make()
        .parse(R"SRC(
        foo(age flt) void
            main()
        main() void
            foo(23.0)
    )SRC")
        .shouldVerified(true);

    make()
        .parse(R"SRC(
        foo(age byte) void
            main()
        main() void
            foo(23.0)
    )SRC")
        .shouldVerified(true);

    make()
        .parse(R"SRC(
        foo(age bool) void
            main()
        main() void
            foo(23.0)
    )SRC")
        .shouldVerified(true);

    make()
        .negative()
        .parse(R"SRC(
        foo(age str) void
            main()
        main() void
            foo(23.0)
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(asExprIntegTest, castByteNegative) {
    // implicit casting of byte:
    //  -> nbool
    make()
        .negative()
        .parse(R"SRC(
        foo(age int) void
            main()
        main() void
            foo(233 as byte)
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprIntegTest, castByteNegative2) {
    make()
        .negative()
        .parse(R"SRC(
        foo(age flt) void
            main()
        main() void
            foo(0xff)
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprIntegTest, castByteNegative3) {
    make()
        .negative()
        .parse(R"SRC(
        foo(age str) void
            main()
        main() void
            foo(0xff as byte)
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(asExprIntegTest, castByte3) {
    make()
        .parse(R"SRC(
        foo(age byte) void
            main()
        main() void
            foo(0xff)
    )SRC")
        .shouldVerified(true);

    make()
        .parse(R"SRC(
        foo(age bool) void
            main()
        main() void
            foo(0xff as byte)
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprIntegTest, castBool) {
    // implicit casting of nbool:
    //  N/A
    make()
        .negative()
        .parse(R"SRC(
        foo(age int) void
            main()
        main() void
            foo(false)
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);

    make()
        .negative()
        .parse(R"SRC(
        foo(age flt) void
            main()
        main() void
            foo(false)
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);
}

TEST_F(asExprIntegTest, castBool1) {
    make()
        .negative()
        .parse(R"SRC(
        foo(age byte) void
            main()
        main() void
            foo(true)
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);
}

TEST_F(asExprIntegTest, castBool2) {
    make()
        .parse(R"SRC(
        foo(age bool) void
            main()
        main() void
            foo(false)
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprIntegTest, castBool3) {
    make()
        .negative()
        .parse(R"SRC(
        foo(age str) void
            main()
        main() void
            foo(true)
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(asExprIntegTest, castStrNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo(age int) void
            main()
        main() void
            foo("wow")
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);

    make()
        .negative()
        .parse(R"SRC(
        foo(age flt) void
            main()
        main() void
            foo("wow")
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);

    make()
        .negative()
        .parse(R"SRC(
        foo(age byte) void
            main()
        main() void
            foo("wow")
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);

    make()
        .negative()
        .parse(R"SRC(
        foo(age bool) void
            main()
        main() void
            foo("wow")
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(asExprIntegTest, castStr) {
    make()
        .parse(R"SRC(
        foo(age str) void
            main()
        main() void
            foo("wow")
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprIntegTest, manualAsInt) {
    threadUse thr(*new errReport(false)); // prevent exception logging for this negative test

    nInt a = 55;
    str as = a.as<nStr>();
    ASSERT_TRUE(as);
    ASSERT_EQ(*as->cast<std::string>(), "55");

    nFlt b(3.5);
    as = b.as<nInt>();
    ASSERT_TRUE(as);
    ASSERT_EQ(*as->cast<nint>(), (int) 3.5);

    as = nStr("False").as<nBool>();
    ASSERT_FALSE(as);
    as = nStr("false").as<nBool>();
    ASSERT_TRUE(as);
    ASSERT_FALSE(*as->cast<nbool>()); // false -> false
    as = nFlt(0.1f).as<nBool>();
    ASSERT_TRUE(as);
    ASSERT_EQ(*as->cast<nbool>(), (bool) 0.1f); // 0 -> false

    as = nFlt(1.1f).as<nBool>();
    ASSERT_TRUE(as);
    ASSERT_TRUE(*as->cast<nbool>()); // except for 0 -> true
}

TEST_F(asExprIntegTest, simpleAsInt1) {
    make()
        .parse(R"SRC(
        foo(age int) void
            main()
        main() void
            foo(23 as int)
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprIntegTest, simpleAsInt2) {
    make()
        .parse(R"SRC(
        foo(age flt) void
            main()
        main() void
            foo(23 as flt)
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprIntegTest, simpleAsInt3) {
    make()
        .parse(R"SRC(
        foo(age bool) void
            main()
        main() void
            foo(23 as bool)
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprIntegTest, simpleAsInt4) {
    make()
        .parse(R"SRC(
        foo(age byte) void
            main()
        main() void
            foo(23 as byte)
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprIntegTest, simpleAsInt5) {
    make()
        .parse(R"SRC(
        foo(age str) void
            main()
        main() void
            foo(23 as str)
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);
}

TEST_F(asExprIntegTest, asExprNotAllowedNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo(age str) void
            main()
        main() void
            foo(23 as foo("wow"))
    )SRC")
        .shouldParsed(false);
}

TEST_F(asExprIntegTest, exprAsAllowed) {
    make()
        .parse(R"SRC(
        foo(age str) void
            main()
        main() void
            foo(foo("wow") as str)
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);
}

TEST_F(asExprIntegTest, asAs) {
    make()
        .parse(R"SRC(
        foo(age str) void
            main()
        main() void
            foo(23.5 as int as str)
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);
}

TEST_F(asExprIntegTest, asAsNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo(age str) void
            main()
        main() void
            foo(23.5 as str as int)
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(asExprIntegTest, AsAllowed) {
    make()
        .parse(R"SRC(
        foo(age str) void
            main()
        main() void
            foo("False" as str) # "False" will cause runtime error
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);
}

TEST_F(asExprIntegTest, promoteTest1) {
    const ntype& it = ttype<nInt>::get();
    const ntype* res = it.promote<nFlt>();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res, ttype<nFlt>::get());

    res = it.promote<nInt>();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res, ttype<nInt>::get());

    res = it.promote<nBool>();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res, ttype<nInt>::get());

    res = it.promote<nByte>();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res, ttype<nInt>::get());

    res = it.promote<nStr>();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res, ttype<nVoid>::get());
}

TEST_F(asExprIntegTest, doubleAs) {
    make()
        .parse(R"SRC(
        print(msg str) void: 1

        foo(idx int) int
           ret idx

        main() int
            print(foo("54634" as int) as str)
            ret 0
    )SRC")
        .shouldVerified(true);
    str ret = run();
    ASSERT_TRUE(ret);
    ASSERT_EQ(*ret->cast<int>(), 0);
}

TEST_F(asExprIntegTest, floatAs) {
    make()
        .parse(R"SRC(
        print(msg str) void: 1

        def obj
            foo() int
                5

        main() int
            o := obj()
            print(o.foo() as str)
            ret (o.foo() as flt) == 5.0
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(asExprIntegTest, implicitCastBetweenArithmeticTypes) {
    make()
        .parse(R"SRC(
        foo(n int) int
            ret n
        main() int
            ret foo(3.5)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res->getType(), ttype<nInt>::get());
    ASSERT_EQ(*res->cast<nint>(), 3);
}

TEST_F(asExprIntegTest, castToVoidNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := 0
            a as void
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(asExprIntegTest, castStrToIntIsParsing) {
    make()
        .parse(R"SRC(
        main() int
            out := "01010"
            ans := "hello"[2] + out[2] # "l" + "0"
            ans == "l0"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(asExprIntegTest, castCharToIntIsNotParsing) {
    make()
        .parse(R"SRC(
        main() int
            out := "01010"
            ans := "00300"[2] + out[1] # "3" + "1" ==> "31"
            ans as int == 31
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(asExprIntegTest, castToObject) {
    make()
        .parse(R"SRC(
        def A
            age := 0
        main() void
            a := A()
            a as A
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprIntegTest, castToObjectNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def A
            age := 0
        main() void
            a := "r"
            a as A
    )SRC")
        .shouldVerified(true);
}

// TODO: make a TC for 'as' nonprimitive types
