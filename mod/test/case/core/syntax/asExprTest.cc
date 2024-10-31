#include "../../../namuSyntaxTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct asExprTest: public namuSyntaxTest {};
}

TEST_F(asExprTest, castInt) {
    make()
        .parse(R"SRC(
        foo(age int) void
            main()
        main() void
            foo(23)
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprTest, castInt1) {
    make()
        .parse(R"SRC(
        foo(age flt) void
            main()
        main() void
            foo(23)
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprTest, castInt2) {
    make()
        .parse(R"SRC(
        foo(age bool) void
            main()
        main() void
            foo(23)
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprTest, castInt3) {
    make()
        .parse(R"SRC(
        foo(age byte) void
            main()
        main() void
            foo(23)
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprTest, castInt4) {
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

TEST_F(asExprTest, castFlt) {
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

TEST_F(asExprTest, castByteNegative) {
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

TEST_F(asExprTest, castByteNegative2) {
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

TEST_F(asExprTest, castByteNegative3) {
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

TEST_F(asExprTest, castByte3) {
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

TEST_F(asExprTest, castBool) {
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

TEST_F(asExprTest, castBool1) {
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

TEST_F(asExprTest, castBool2) {
    make()
        .parse(R"SRC(
        foo(age bool) void
            main()
        main() void
            foo(false)
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprTest, castBool3) {
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

TEST_F(asExprTest, castStrNegative) {
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

TEST_F(asExprTest, castStr) {
    make()
        .parse(R"SRC(
        foo(age str) void
            main()
        main() void
            foo("wow")
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprTest, manualAsInt) {
    nInt a = 55;
    str as = a.as<nStr>();
    ASSERT_TRUE(as);
    ASSERT_EQ(as->cast<std::string>(), "55");

    nFlt b(3.5);
    as = b.as<nInt>();
    ASSERT_TRUE(as);
    ASSERT_EQ(as->cast<nint>(), (int) 3.5);

    as = nStr("False").as<nBool>();
    ASSERT_FALSE(as);
    as = nStr("false").as<nBool>();
    ASSERT_TRUE(as);
    ASSERT_FALSE(as->cast<nbool>()); // false -> false
    as = nFlt(0.1f).as<nBool>();
    ASSERT_TRUE(as);
    ASSERT_EQ(as->cast<nbool>(), (bool) 0.1f); // 0 -> false

    as = nFlt(1.1f).as<nBool>();
    ASSERT_TRUE(as);
    ASSERT_TRUE(as->cast<nbool>()); // except for 0 -> true
}

TEST_F(asExprTest, simpleAsInt1) {
    make()
        .parse(R"SRC(
        foo(age int) void
            main()
        main() void
            foo(23 as int)
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprTest, simpleAsInt2) {
    make()
        .parse(R"SRC(
        foo(age flt) void
            main()
        main() void
            foo(23 as flt)
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprTest, simpleAsInt3) {
    make()
        .parse(R"SRC(
        foo(age bool) void
            main()
        main() void
            foo(23 as bool)
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprTest, simpleAsInt4) {
    make()
        .parse(R"SRC(
        foo(age byte) void
            main()
        main() void
            foo(23 as byte)
    )SRC")
        .shouldVerified(true);
}

TEST_F(asExprTest, simpleAsInt5) {
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

TEST_F(asExprTest, asExprNotAllowedNegative) {
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

TEST_F(asExprTest, exprAsAllowed) {
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

TEST_F(asExprTest, asAs) {
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

TEST_F(asExprTest, asAsNegative) {
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

TEST_F(asExprTest, AsAllowed) {
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

TEST_F(asExprTest, deduceTest1) {
    const ntype& it = ttype<nInt>::get();
    const ntype* res = &it.deduce<nFlt>();
    ASSERT_FALSE(nul(res));
    ASSERT_EQ(*res, ttype<nFlt>::get());

    res = &it.deduce<nInt>();
    ASSERT_FALSE(nul(res));
    ASSERT_EQ(*res, ttype<nInt>::get());

    res = &it.deduce<nBool>();
    ASSERT_FALSE(nul(res));
    ASSERT_EQ(*res, ttype<nInt>::get());

    res = &it.deduce<nByte>();
    ASSERT_FALSE(nul(res));
    ASSERT_EQ(*res, ttype<nInt>::get());

    res = &it.deduce<nStr>();
    ASSERT_FALSE(nul(res));
    ASSERT_EQ(*res, ttype<nVoid>::get());
}

TEST_F(asExprTest, doubleAs) {
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
    ASSERT_EQ(ret->cast<int>(), 0);
}

TEST_F(asExprTest, floatAs) {
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
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(asExprTest, implicitCastBetweenArithmeticTypes) {
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
    ASSERT_EQ(res->cast<nint>(), 3);
}

TEST_F(asExprTest, castToVoidNegative) {
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

TEST_F(asExprTest, castStrToIntIsParsing) {
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
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(asExprTest, castCharToIntIsNotParsing) {
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
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(asExprTest, castToObject) {
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

TEST_F(asExprTest, castToObjectNegative) {
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
