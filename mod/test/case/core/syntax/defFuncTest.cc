#include "../../../namuSyntaxTest.hpp"

using namespace namu;
using namespace std;

namespace {
    struct defFuncTest : public namuSyntaxTest {};
}

TEST_F(defFuncTest, distinguishDefineFuncOrCall) {
    if(make().parse(R"SRC(
        foo(x int, y int) void
            ret

        main() void
            a := 22
            foo(a, 22)
    )SRC").shouldParsed(true)) {
        node& res = getSubPack();
        ASSERT_FALSE(nul(res));

        const baseFunc& f = res.sub<baseFunc>("main", narr());
        ASSERT_FALSE(nul(f));
        ASSERT_EQ(f.getParams().len(), 0);
        ASSERT_EQ(f.getRet()->getType(), ttype<nVoid>());
    }

    if(make().parse(R"SRC(
        foo(x int, y int) void
            ret

        main(argc int, argv str) void
            foo(argc, 22)
    )SRC").shouldParsed(true)) {
        node& res = getSubPack();
        ASSERT_FALSE(nul(res));

        const baseFunc& fwrong = res.sub<baseFunc>("main", narr());
        ASSERT_TRUE(nul(fwrong));
        const baseFunc& f = res.sub<baseFunc>("main", narr(nInt(), nStr()));
        ASSERT_FALSE(nul(f));
        ASSERT_EQ(f.getParams().len(), 2);
        ASSERT_EQ(f.getRet()->getType(), ttype<nVoid>());

        const params& ps = f.getParams();
        ASSERT_FALSE(nul(ps));
        ASSERT_EQ(ps.len(), 2);

        const param& arg1 = ps[0];
        ASSERT_EQ(arg1.getName(), "argc");
        ASSERT_EQ(arg1.getOrigin().getType(), ttype<nInt>());
        const param& arg2 = ps[1];
        ASSERT_EQ(arg2.getName(), "argv");
        ASSERT_EQ(arg2.getOrigin().getType(), ttype<nStr>());
    }
}

TEST_F(defFuncTest, distinguishDefineFuncOrCall2) {
    make().negative().parse(R"SRC(
        main() void
            foo(a, 22) void
                a.doSomething(22)
            foo(a, 22)
    )SRC").shouldParsed(false);
}

TEST_F(defFuncTest, distinguishDefineLambdaOrCall) {
    make().negative().parse(R"SRC(
        main() void
            (a, 22) // this is not lambda
    )SRC").shouldParsed(false);
}

TEST_F(defFuncTest, distinguishDefineLambdaOrCallNegative) {
    make().negative().parse(R"SRC(
        main() void
            (a, 22) // lambda should be declared in func call.
                a.doSomething(22)
            foo(a, 22)
    )SRC").shouldParsed(false);
}

TEST_F(defFuncTest, lambda1) {
    make().negative().parse(R"SRC(
        main() void
            a.sendPacket((packet)
                doSomething()
            ))SRC").shouldParsed(true);
}

TEST_F(defFuncTest, lambda2) {
    make().negative().parse(R"SRC(
        main() void
            a.sendPacket((packet)
                doSomething()
            ))SRC").shouldParsed(true);
}

TEST_F(defFuncTest, lambda3) {
    make().negative().parse(R"SRC(
        main() void
            a.sendPacket((packet))
            (packet)
            a + 5
    )SRC").shouldParsed(true);
}

TEST_F(defFuncTest, lambda4) {
    make().negative().parse(R"SRC(
        main() void
            a.sendPacket(() void
                doSomething()
            ))SRC").shouldParsed(true);
}

TEST_F(defFuncTest, lambda5Negative) {
    make().negative().parse(R"SRC(
        main() void
            a.sendPacket(((a int, b) void // no paranthesis for lambda
                a.doSomething()
            ))
    )SRC").shouldParsed(false);
}

TEST_F(defFuncTest, lambda6) {
    make().negative().parse(R"SRC(
        main() void
            a.sendPacket((a int, b) void
                a.doSomething()
            )
    )SRC").shouldParsed(false);
}

TEST_F(defFuncTest, noBodyNegative) {
    negative().make().parse(R"SRC(
        main() void)SRC").shouldParsed(true);
    shouldVerified(false);

    make().parse(R"SRC(
        main(n int) void
    )SRC").shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defFuncTest, wrongParamNegative) {
    negative().make().parse(R"SRC(
        main(a) void
            22
    )SRC").shouldParsed(false);

    make().negative().parse(R"SRC(
        main(age int()) void
            22
    )SRC").shouldParsed(false);

    make().negative().parse(R"SRC(
        main(age + 22, age int) void
            22
    )SRC").shouldParsed(false);

    make().negative().parse(R"SRC(
        main(age + 22, age int) void
            22
    )SRC").shouldParsed(false);

    make().negative().parse(R"SRC(
        main(aka int -> i) void
            22
    )SRC").shouldParsed(false);
}

TEST_F(defFuncTest, nameLikeStr) {
    make().parse(R"SRC(
        main(str_ str) int
            ret "wow".len()
    )SRC").shouldParsed(true);
    shouldVerified(true);
}

TEST_F(defFuncTest, defFuncAtSubPack) {
    make().parse(R"SRC(
        foo(msg str[]) str
            print(msg[0])
            msg[0]

        main() int
            msgs := {"hello", "world"}
            foo(msgs) == "hello"
    )SRC").shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(defFuncTest, defFuncReturnClass) {
    make().parse(R"SRC(
        foo() A
            A()
        def A
            age := 0
        main() int
            a := 'r'
            foo().age
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 0);
}

TEST_F(defFuncTest, defFuncDuplicateNegative) {
    make().negative().parse(R"SRC(
        a(n int) void
            ret
        a(n int) int
            ret n
        main() void
            ret
    )SRC").shouldVerified(false);
}

TEST_F(defFuncTest, defFuncDuplicate) {
    make().parse(R"SRC(
        a() void
            ret
        a(n int) int
            ret n
        main() void
            ret
    )SRC").shouldVerified(true);
}

TEST_F(defFuncTest, funcHasSameNameToFieldNegative) {
    make().negative().parse(R"SRC(
        def A
            a() void

            a := 0

        main() void
            ret
    )SRC").shouldVerified(false);
}

TEST_F(defFuncTest, funcButNoStmts) {
    make().parse(R"SRC(
        foo() void:;
        main() void
            foo()
    )SRC").shouldVerified(true);
}

TEST_F(defFuncTest, overloadingDifferentParameters) {
    make().parse(R"SRC(
        def a
            foo() int: 0
            foo(n int) int: 1
    )SRC").shouldVerified(true);

    node& pak = getSubPack();
    obj& a = pak.sub<obj>("a");
    ASSERT_FALSE(nul(a));

    {
        auto subs = a.subAll<func>("foo");
        ASSERT_EQ(subs.len(), 2);
    }

    {
        args args1(narr{nInt()});
        auto subs = a.subAll<func>("foo", args1);
        ASSERT_EQ(subs.len(), 1);
        ASSERT_EQ(subs[0].lv, EXACT_MATCH);
        const params& ps = subs[0]->getParams();
        ASSERT_EQ(ps.len(), 1);
        ASSERT_EQ(ps[0].getOrigin().getType(), ttype<nInt>());
        str res = a.run("foo", args1);
        ASSERT_TRUE(res);
        ASSERT_EQ(res->cast<nint>(), 1);
    }
}

TEST_F(defFuncTest, overloadingSimilarParameters) {
    make().parse(R"SRC(
        def a
            foo(b bool, n bool, s str) int: 0
            foo(b bool, n int, s str) int: 1
    )SRC").shouldVerified(true);

    obj& a = getSubPack().sub<obj>("a");
    ASSERT_FALSE(nul(a));

    {
        ASSERT_EQ(a.subAll<func>("foo", args(narr{nBool(), nChar()})).len(), 0);
    }

    {
        args args1(narr{*new nBool(), *new nChar(), *new nStr()});
        auto subs = a.subAll<func>("foo", args1);
        ASSERT_EQ(subs.len(), 2);
        ASSERT_EQ(subs[0].lv, subs[1].lv);
        ASSERT_EQ(subs[0].lv, IMPLICIT_MATCH);
        str res = a.run("foo", args1);
        ASSERT_FALSE(res);
    }

    {
        args args1(narr{*new nBool(), *new nByte(), *new nStr()}); // byte <--> int are overloading priority lv1. refers func.cpp
        auto subs = a.subAll<func>("foo", args1);
        ASSERT_EQ(subs.len(), 2);
        ASSERT_EQ(subs[0].lv, NUMERIC_MATCH);
        ASSERT_EQ(subs[1].lv, IMPLICIT_MATCH);
        str res = a.run("foo", args1);
        ASSERT_TRUE(res);
        ASSERT_EQ(res.cast<nint>(), 1);
    }
}

TEST_F(defFuncTest, overloadingAmbigiousNegative) {
    make().negative().parse(R"SRC(
        def a
            foo(b bool, c int) int: 1
            foo(b int, c bool) int: 2
        main() void
            foo('1', '2')
    )SRC").shouldParsed(true);
    shouldVerified(false);

    obj& a = getSubPack().sub<obj>("a");
    ASSERT_FALSE(nul(a));
    tpriorities<func> p = a.subAll<func>("foo", args(narr{nChar(), nChar()})).getAmbigious();
    ASSERT_EQ(p.len(), 2);
}

/* TODO: uncomment after implement isAbstract() on func/originObj
TEST_F(defFuncTest, funcButNoStmtsNegative) {
    make().negative().parse(R"SRC(
        foo() int:; // err: no return stmt
        main() void
            foo()
    )SRC").shouldVerified(false);
}*/
