#include "test/byeolSyntaxTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct FBOExprTest: public byeolSyntaxTest {};
}

TEST_F(FBOExprTest, simpleAdd) {
    make()
        .parse(R"SRC(
        a := 5
        b := 2
        main() int
            ret a + b
    )SRC")
        .shouldVerified(true);

    nInt& a = getSubPack() TO(template sub<nInt>("a")) OR_ASSERT(a);
    ASSERT_EQ(a.get(), 5);
    nInt b = getSubPack() TO(template sub<nInt>("b")) OR_ASSERT(b);
    ASSERT_EQ(b.get(), 2);

    str res(getSubPack() TO(run("main")));
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->isSub<baseErr>());

    {
        threadUse thr;
        tstr<nInt> res(getSubPack() TO(run("main")));
        ASSERT_TRUE(res);
        ASSERT_EQ(*res.cast<nint>(), 7);
    }
}

TEST_F(FBOExprTest, addWithDefAssign) {
    make()
        .parse(R"SRC(
        foo() int
            1 + 2
        a := 5
        b := a + 2
        main() int
            ret a + b
    )SRC")
        .shouldVerified(true);

    nInt& a = getSubPack() TO(template sub<nInt>("a")) OR_ASSERT(a);
    ASSERT_EQ(a.get(), 5);
    nInt b = getSubPack() TO(template sub<nInt>("b")) OR_ASSERT(b);
    ASSERT_EQ(b.get(), 7);

    threadUse thr1;
    tstr<nInt> res(getSubPack() TO(run("main")));
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), 12);
}

TEST_F(FBOExprTest, addWithDefAssignReversed) {
    make()
        .negative()
        .parse(R"SRC(
        b := a + 2
        a := 5
        main() int
            ret a + b
    )SRC")
        .shouldVerified(true);

    nInt& a = getSubPack() TO(template sub<nInt>("a")) OR_ASSERT(a);
    ASSERT_EQ(a.get(), 5);
    nInt& b = getSubPack() TO(template sub<nInt>("b")) OR_ASSERT(b);
    ASSERT_EQ(b.get(), 7);
}

TEST_F(FBOExprTest, addIntAndStrNegative) {
    make()
        .negative()
        .parse(R"SRC(
        a := "hello" + 12
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(FBOExprTest, addIntAndStr) {
    make()
        .parse(R"SRC(
        a := "hello" + 12 as str
        main() int
            ret 0
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);

    nStr& a = getSubPack() TO(template sub<nStr>("a")) OR_ASSERT(a);
    ASSERT_EQ(a.get(), std::string("hello12"));
}

TEST_F(FBOExprTest, simpleSub) {
    make()
        .parse(R"SRC(
        a := 5
        b := 2
        main() int
            ret a - b
    )SRC")
        .shouldVerified(true);

    nInt& a = getSubPack() TO(template sub<nInt>("a")) OR_ASSERT(b);
    ASSERT_EQ(a.get(), 5);
    nInt b = getSubPack() TO(template sub<nInt>("b")) OR_ASSERT(b);
    ASSERT_EQ(b.get(), 2);

    threadUse thr;
    tstr<nInt> res(getSubPack() TO(run("main")));
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), 3);
}

TEST_F(FBOExprTest, modWithDefAssign) {
    make()
        .parse(R"SRC(
        a := 10
        b := a / 2
        main() int
            ret a * b % 7
    )SRC")
        .shouldVerified(true);

    nInt& a = getSubPack() TO(template sub<nInt>("a")) OR_ASSERT(a);
    ASSERT_EQ(a.get(), 10);
    nInt b = getSubPack() TO(template sub<nInt>("b")) OR_ASSERT(b);
    ASSERT_EQ(b.get(), 5);

    threadUse thr;
    tstr<nInt> res(getSubPack() TO(run("main")));
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), 1);
}

TEST_F(FBOExprTest, testStringAddSequence) {
    make()
        .parse(R"SRC(
    Helloworld(age int) int
        ret age

    main() int
        ret (Helloworld(4) as str + "low\n") == "4low\n"
    )SRC")
        .shouldVerified(true);

    nm::str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res->cast<nint>(), 1);
}

TEST_F(FBOExprTest, testStringAddBoolean) {
    make()
        .parse(R"SRC(
    Helloworld(age int) int
        ret age

    main() int
        ret (Helloworld(false as int) as str + "low\n" ) == "0low\n"
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);


    NM_E("start run!");
    nm::str res = run();
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(FBOExprTest, testStringAddBoolean2) {
    make()
        .parse(R"SRC(
    Helloworld(age int) int
        ret age

    main() int
        ret (Helloworld(false as int) as str + "low\n" ) == "0low\n"
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);

    nm::str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res->cast<nint>(), 1);
}

TEST_F(FBOExprTest, testLogicalBinaryOp) {
    make()
        .parse(R"SRC(
        main() int
            a := 1
            b := 3
            a <= b
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(FBOExprTest, testLogicalBinaryOpWithDifferentType) {
    make()
        .parse(R"SRC(
        main() int
            a := 1
            b := 3.5
            a > b
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 0);
}

TEST_F(FBOExprTest, testLogicalBinaryOpFltPrecision) {
    make()
        .parse(R"SRC(
        main() int
            a := 3.4 + 0.1
            b := 3.5
            a == b
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(FBOExprTest, testLogicalBinaryOpStr) {
    make()
        .parse(R"SRC(
        main() int
            a := "hello"
            b := "helwo"
            a >= b
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 0);
}

TEST_F(FBOExprTest, testLogicalBinaryOpChar) {
    make()
        .parse(R"SRC(
        main() int
            a := "l"
            b := "w"
            (a as byte) > (b as byte)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 0);
}

TEST_F(FBOExprTest, testStringToBoolean) {
    make()
        .parse(R"SRC(
        main() int
            a := "false"
            if (a as bool) <= 0
                22
            else
                11
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 22);
}

TEST_F(FBOExprTest, testStringToBooleanNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            a := "false"
            if (a as int) <= 0
                22
            else
                11
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);

    // TODO: but runtime exception?
}

TEST_F(FBOExprTest, testLogicalAndOp) {
    make()
        .parse(R"SRC(
        main() int
            ret true && 3 < 27
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(FBOExprTest, testLogicalAndOpNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo() bool
            false

        main() int
            a := 0
            if (foo() && a = 1) # assignment is not expression
                print("ok")
            ret a
    )SRC")
        .shouldVerified(false);
}

TEST_F(FBOExprTest, testLogicalAndOp2) {
    make()
        .parse(R"SRC(
        print(msg str) void: 1

        foo() bool
            true

        main() int
            a := 1
            if foo()
                print("ok")
            ret a
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res->getType(), ttype<nInt>::get());
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(FBOExprTest, testLogicalAndOpShortCircuit) {
    make()
        .parse(R"SRC(
        print(msg str) void: 1

        main() int
            if true || false
                print("ok")
            ret 0
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res->getType(), ttype<nInt>::get());
    ASSERT_EQ(*res.cast<nint>(), 0);
}

TEST_F(FBOExprTest, testAddAssign) {
    make()
        .parse(R"SRC(
        main() int
            a := 5
            a += 3
            ret a 
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 8);
}

TEST_F(FBOExprTest, testSubAssign) {
    make()
        .parse(R"SRC(
        main() int
            a := 5
            a -= -3
            ret a
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 8);
}

TEST_F(FBOExprTest, testMulAssign) {
    make()
        .parse(R"SRC(
        main() int
            a := 5
            a *= 3
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 15);
}

TEST_F(FBOExprTest, testDivAssign) {
    make()
        .parse(R"SRC(
        main() int
            a := 6
            a /= 3
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 2);
}

TEST_F(FBOExprTest, testModAssign) {
    make()
        .parse(R"SRC(
        main() int
            a := 5
            a %= 3
            ret a
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 2);
}

TEST_F(FBOExprTest, testBitwiseOperator) {
    make()
        .parse(R"SRC(
        main() int
            a := 60
            b := 13
            a & b
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 12);
}

TEST_F(FBOExprTest, testBitwiseOperator2) {
    make()
        .parse(R"SRC(
        main() int
            a := 60
            b := 13
            a | b
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 61);
}

TEST_F(FBOExprTest, testBitwiseOperator3) {
    make()
        .parse(R"SRC(
        main() int
            a := 60
            b := 13
            a ^ b
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 49);
}

TEST_F(FBOExprTest, testBitwiseOperator4) {
    make()
        .parse(R"SRC(
        main() int
            ~60
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), -61);
}

TEST_F(FBOExprTest, testBitwiseOperator5) {
    make()
        .parse(R"SRC(
        main() int
            a := 60
            b := 2
            a << b
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 240);
}

TEST_F(FBOExprTest, testBitwiseOperator6) {
    make()
        .parse(R"SRC(
        main() int
            a := 60
            a >> 2
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 15);
}

TEST_F(FBOExprTest, strNotSuitableToSomeOpNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := "jokbal"
            a - a
    )SRC")
        .shouldVerified(false);
}

TEST_F(FBOExprTest, strNotSuitableToSomeOpNegative2) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := "jokbal"
            a / a
    )SRC")
        .shouldVerified(false);
}

TEST_F(FBOExprTest, strNotSuitableToSomeOpNegative3) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := "jokbal"
            a % a
    )SRC")
        .shouldVerified(false);
}

TEST_F(FBOExprTest, strNotSuitableToSomeOpNegative4) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := "jokbal"
            a >> 2
    )SRC")
        .shouldVerified(false);
}

TEST_F(FBOExprTest, strNotSuitableToSomeOpNegative5) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := "jokbal"
            a & 2
    )SRC")
        .shouldVerified(false);
}
