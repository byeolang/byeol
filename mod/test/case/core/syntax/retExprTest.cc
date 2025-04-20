#include "test/namuSyntaxTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct retExprTest: public namuSyntaxTest {};
}

TEST_F(retExprTest, simpleReturnTypeCheck) {
    make()
        .parse(R"SRC(
        main() int
            ret 33
    )SRC")
        .shouldVerified(true);
}

TEST_F(retExprTest, simpleReturnTypeCheckStr) {
    make()
        .parse(R"SRC(
        main() int
            ret "wow" == "wow"
    )SRC")
        .shouldVerified(true);
}

TEST_F(retExprTest, simpleReturnTypeNegative) {
    make()
        .negative()
        .parse(R"SRC(
        make() int
            ret
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);

    func& make = getSubPack().sub<func>("make");
    ASSERT_FALSE(nul(make));
    blockExpr& blkExp = make.getBlock();
    ASSERT_FALSE(nul(blkExp));
    nucontainer& con = blkExp.getStmts();
    ASSERT_FALSE(nul(con));
    retExpr& ret = con.begin().get<retExpr>();
    ASSERT_FALSE(nul(ret));

    ASSERT_TRUE(ret.getEval()->isSub<retExpr>());
    ASSERT_EQ(ret.getRet().getEval()->getType(), ttype<nVoid>::get());
}

TEST_F(retExprTest, implicitReturn) {
    make()
        .parse(R"SRC(
        make() int
            22
    )SRC")
        .shouldVerified(true);
}

TEST_F(retExprTest, implicitReturnNegative) {
    make()
        .negative()
        .parse(R"SRC(
        make() flt
            "wow"
    )SRC")
        .shouldVerified(false);
}

TEST_F(retExprTest, implicitReturnShouldNotWorkOnVoid) {
    make()
        .parse(R"SRC(
        main() void
            35
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);
}

TEST_F(retExprTest, returnLocalVariable) {
    make()
        .parse(R"SRC(
        main() int
            age int
            ret age
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);
}

TEST_F(retExprTest, returnTypeImplicitCasting) {
    make()
        .parse(R"SRC(
        make() int
            ret 3.5
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);
}

TEST_F(retExprTest, returnVoidNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            ret 3
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(retExprTest, retLocalVariable) {
    make()
        .parse(R"SRC(
        main() int
            age int
            age
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);
}

TEST_F(retExprTest, retTypeImplicitCasting) {
    make()
        .parse(R"SRC(
        make() int
            3.5
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);
}

TEST_F(retExprTest, retVoid) {
    make()
        .parse(R"SRC(
        main() void
            3
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);
}

TEST_F(retExprTest, retDefAssign) {
    make()
        .parse(R"SRC(
        main() int
            a := 5
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 5);
}

TEST_F(retExprTest, retIsNotExpressionNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo(n int) void
            ret

        main() int
            foo(ret 3)
    )SRC")
        .shouldParsed(false);
}

TEST_F(retExprTest, retException) {
    make()
        .parse(R"SRC(
        foo(n int) int
            ret err("err found")
        main() void
            print(foo(2) as str)
            print(foo(5) as str)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_FALSE(nul(res));
    baseErr& cast = res.cast<baseErr>();
    ASSERT_FALSE(nul(cast));

    const auto& ex = getReport();
    ASSERT_FALSE(nul(ex));
    ASSERT_TRUE(ex.len() > 0);
    ASSERT_EQ(ex.len(), 1);
}

TEST_F(retExprTest, retExceptionNoThrowAgain) {
    make()
        .parse(R"SRC(
        foo(n int) int
            ret err("just an err")
        main() int
            foo(3)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_FALSE(nul(res));
    baseErr& cast = res.cast<baseErr>();
    ASSERT_FALSE(nul(cast));

    const auto& ex = getReport();
    ASSERT_FALSE(nul(ex));
    ASSERT_EQ(ex.len(), 1);
    ASSERT_EQ(ex[0].getMsg(), "just an err");
}

TEST_F(retExprTest, retExceptionNoThrowAgain2) {
    make()
        .parse(R"SRC(
        foo(n int) int
            ret err("just an err")
        main() int
            ret foo(3)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_FALSE(nul(res));
    baseErr& cast = res.cast<baseErr>();
    ASSERT_FALSE(nul(cast));

    const auto& ex = getReport();
    ASSERT_FALSE(nul(ex));
    ASSERT_EQ(ex.len(), 2);
}

TEST_F(retExprTest, dontUseRetAtMiddleOfBlockNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo() int
            a := 2
            b := 3 + 1
            ret a + b
            c := a * b
            ret c

        main() int
            foo()
    )SRC")
        .shouldVerified(false);

    const auto& rpt = getReport();
    ASSERT_TRUE(rpt.inErr());
}

TEST_F(retExprTest, dontUseRetAtMiddleOfBlockNegative2) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            a := for n in 0..5
                b := 3 + 1
                ret b + n
                c := b * n
            ret a[1]
    )SRC")
        .shouldVerified(false);

    const auto& rpt = getReport();
    ASSERT_TRUE(rpt.inErr());
}
