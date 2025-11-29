#include "test/byeolSyntaxTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct defFuncTest: public byeolSyntaxTest {};
}

TEST_F(defFuncTest, distinguishDefineFuncOrCall) {
    if(make()
            .parse(R"SRC(
        foo(x int, y int) void
            ret

        main() void
            a := 22
            foo(a, 22)
    )SRC")
            .shouldParsed(true)) {
        node& res = getSubPack() OR_ASSERT(res);

        const baseFunc& f = res.sub<baseFunc>("main", narr()) OR_ASSERT(f);
        ASSERT_EQ(f.getParams().len(), 0);
        ASSERT_EQ(f.getRet()->getType(), ttype<nVoid>());
    }

    if(make()
            .parse(R"SRC(
        foo(x int, y int) void
            ret

        main(argc int, argv str) void
            foo(argc, 22)
    )SRC")
            .shouldParsed(true)) {
        node& res = getSubPack() OR_ASSERT(res);

        ASSERT_FALSE(res.sub<baseFunc>("main", narr()));
        const baseFunc& f = res.sub<baseFunc>("main", narr(*new nInt(), *new nStr())) OR_ASSERT(f);
        ASSERT_EQ(f.getParams().len(), 2);
        ASSERT_EQ(f.getRet()->getType(), ttype<nVoid>());

        const params& ps = f.getParams();
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
    make()
        .negative()
        .parse(R"SRC(
        main() void
            foo(a, 22) void
                a.doSomething(22)
            foo(a, 22)
    )SRC")
        .shouldParsed(false);
}

TEST_F(defFuncTest, distinguishDefineLambdaOrCall) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            (a, 22) # this is not lambda
    )SRC")
        .shouldParsed(false);
}

TEST_F(defFuncTest, distinguishDefineLambdaOrCallNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            (a, 22) # lambda should be declared in func call.
                a.doSomething(22)
            foo(a, 22)
    )SRC")
        .shouldParsed(false);
}

TEST_F(defFuncTest, lambda1) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a.sendPacket((packet)
                doSomething()
            ))SRC")
        .shouldParsed(true);
}

TEST_F(defFuncTest, lambda2) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a.sendPacket((packet)
                doSomething()
            ))SRC")
        .shouldParsed(true);
}

TEST_F(defFuncTest, lambda3) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a.sendPacket((packet))
            (packet)
            a + 5
    )SRC")
        .shouldParsed(true);
}

TEST_F(defFuncTest, lambda4) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a.sendPacket(() void
                doSomething()
            ))SRC")
        .shouldParsed(true);
}

TEST_F(defFuncTest, lambda5Negative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a.sendPacket(((a int, b) void # no paranthesis for lambda
                a.doSomething()
            ))
    )SRC")
        .shouldParsed(false);
}

TEST_F(defFuncTest, lambda6) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a.sendPacket((a int, b) void
                a.doSomething()
            )
    )SRC")
        .shouldParsed(false);
}

TEST_F(defFuncTest, noBodyNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void)SRC")
        .shouldParsed(true);
    shouldVerified(false);

    make()
        .negative()
        .parse(R"SRC(
        main(n int) void
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defFuncTest, wrongParamNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main(a) void
            22
    )SRC")
        .shouldParsed(false);

    make()
        .negative()
        .parse(R"SRC(
        main(age int()) void
            22
    )SRC")
        .shouldParsed(false);

    make()
        .negative()
        .parse(R"SRC(
        main(age + 22, age int) void
            22
    )SRC")
        .shouldParsed(false);

    make()
        .negative()
        .parse(R"SRC(
        main(age + 22, age int) void
            22
    )SRC")
        .shouldParsed(false);

    make()
        .negative()
        .parse(R"SRC(
        main(aka int -> i) void
            22
    )SRC")
        .shouldParsed(false);
}

TEST_F(defFuncTest, nameLikeStr) {
    make()
        .parse(R"SRC(
        main(str_ str) int
            ret "wow".len()
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);
}

TEST_F(defFuncTest, defFuncAtSubPack) {
    make()
        .parse(R"SRC(
        print(msg str) void: 1

        foo(msg str[]) str
            print(msg[0])
            msg[0]

        main() int
            msgs := {"hello", "world"}
            foo(msgs) == "hello"
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(defFuncTest, defFuncReturnClass) {
    make()
        .parse(R"SRC(
        foo() A
            A()
        def A
            age := 0
        main() int
            foo().age
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 0);
}

TEST_F(defFuncTest, defFuncDuplicateNegative) {
    make()
        .negative()
        .parse(R"SRC(
        a(n int) void
            ret
        a(n int) int
            ret n
        main() void
            ret
    )SRC")
        .shouldVerified(false);
}

TEST_F(defFuncTest, defFuncDuplicate) {
    make()
        .parse(R"SRC(
        a() void
            ret
        a(n int) int
            ret n
        main() void
            ret
    )SRC")
        .shouldVerified(true);
}

TEST_F(defFuncTest, funcHasSameNameToFieldNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def A
            a() void

            a := 0

        main() void
            ret
    )SRC")
        .shouldVerified(false);
}

TEST_F(defFuncTest, funcButNoStmts) {
    make()
        .parse(R"SRC(
        foo() void:;
        main() void
            foo()
    )SRC")
        .shouldVerified(true);
}

TEST_F(defFuncTest, overloadingDifferentParameters) {
    make()
        .parse(R"SRC(
        def a
            foo() int: 0
            foo(n int) int: 1
    )SRC")
        .shouldVerified(true);

    node& a = getSubPack() TO(template sub<obj>("a")) OR_ASSERT(a);
    {
        auto subs = a.subAll<func>("foo");
        ASSERT_EQ(subs.len(), 2);
    }

    {
        threadUse th;
        args args1(narr{*new nInt()});
        auto subs = a.subAll<func>("foo", args1);
        ASSERT_EQ(subs.len(), 1);
        ASSERT_EQ(subs.getPriorType(), EXACT_MATCH);
        const params& ps = subs[0].getParams();
        ASSERT_EQ(ps.len(), 1);
        ASSERT_EQ(ps[0].getOrigin().getType(), ttype<nInt>());
        str res = a.run("foo", args1);
        ASSERT_TRUE(res);
        ASSERT_EQ(*res->cast<nint>(), 1);
    }
}

TEST_F(defFuncTest, overloadingSimilarParameters) {
    make()
        .parse(R"SRC(
        def a
            foo(b bool, n bool, s str) int: 0
            foo(b bool, n int, s str) int: 1
    )SRC")
        .shouldVerified(true);

    obj& a = getSubPack() TO(template sub<obj>("a")) OR_ASSERT(a);

    { ASSERT_EQ(a.subAll<func>("foo", args(nullptr, narr(nBool(), nInt()))).len(), 0); }

    {
        threadUse th;
        args args1(narr{*new nBool(), *new nFlt(), *new nStr()});
        auto subs = a.subAll<func>("foo", args1);
        ASSERT_EQ(subs.len(), 2);
        ASSERT_EQ(subs.getPriorType(), IMPLICIT_MATCH);
        str res = a.run("foo", args1);
        ASSERT_FALSE(res);
    }

    {
        threadUse th;
        args args1(narr{*new nBool(), *new nByte(), *new nStr()});
        auto subs = a.subAll<func>("foo", args1);
        ASSERT_EQ(subs.len(), 1); // there are 2 implicit match in above code but,
                                  // foo(bool, int, str) has higher priority compare to foo(bool,
                                  // bool, str) the reason is, byte <--> int casting is priority
                                  // lv1. @refers func.cpp so subs only contains 1 priority element.
        ASSERT_EQ(subs.getPriorType(), NUMERIC_MATCH);
        str res = a.run("foo", args1);
        ASSERT_TRUE(res);
        ASSERT_EQ(*res.cast<nint>(), 1);
    }
}

TEST_F(defFuncTest, overloadingAmbigiousNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def a
            foo(b bool, c int) int: 1
            foo(b int, c bool) int: 2
        main() void
            foo(1, 2)
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);

    obj& a = getSubPack() TO(template sub<obj>("a")) OR_ASSERT(a);
    auto p = a.subAll<func>("foo", args(nullptr, narr(nInt(), nInt())));
    ASSERT_FALSE(p.isMatched());
    ASSERT_EQ(p.len(), 2);
}

TEST_F(defFuncTest, accessMeShouldBeComplete) {
    make()
        .parse(R"SRC(
        def a
            age int
            say(n int) void
                me.age = n

        main() int
            a1 := a()
            a1.say(22)
            ret a1.age
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 22);
}

TEST_F(defFuncTest, simpleCtor) {
    make()
        .parse(R"SRC(
        def Person
            name str
            ctor(name str)
                me.name = name
        main() int
            p1 := Person("kniz")
            ret p1.name.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res->cast<int>(), 4);
}

TEST_F(defFuncTest, multipleCtor) {
    make()
        .parse(R"SRC(
        def person
            name str
            ctor(): me.name = "hello"
            ctor(name str)
                me.name = name
        main() int
            p1 := person("kniz")
            ret p1.name.len() + person().name.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res->cast<int>(), 9);
}

TEST_F(defFuncTest, simpleCtorNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def person
            name str
            ctor(name str)
                me.name = name
        main() int
            p1 := person()
            ret p1.name.len()
    )SRC")
        .shouldVerified(false);
}

TEST_F(defFuncTest, funcButNoStmtsNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo() int:; # err: no return stmt
        main() void
            foo()
    )SRC")
        .shouldVerified(false);
}

TEST_F(defFuncTest, mainFuncWithInlineBlock) {
    make()
        .parse(R"SRC(
        foo(n int, x int) int: n + x
        age := 2
        main() int: foo(age, 1))SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 3);
}

TEST_F(defFuncTest, mainFuncWithInlineBlock2) {
    make()
        .parse(R"SRC(
        age := 2
        main() int: foo(age, 1)
        foo(n int, x int) int: n + x
   )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 3);
}

TEST_F(defFuncTest, funcTakingFunc) {
    make()
        .parse(R"SRC(
        foo(n int) int: n + 1
        boo(f foo, n int) int
            f(n + 1)
        main() int
            boo(foo, 1)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 3);
}

TEST_F(defFuncTest, funcTakingClosure) {
    make()
        .parse(R"SRC(
        foo(n int) flt: n + 1.5
        boo(f foo, n int) flt: f(n + 1.5) # flt -> int is allowed to use a func type
        main() int: boo(foo, 1) + 0.5
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 4);
}

TEST_F(defFuncTest, closureDontHaveImplicitCasting) {
    make()
        .negative()
        .parse(R"SRC(
        foo1(n int) int: n + 1
        foo2(n flt) int: n + 1
        boo(f foo1, n int) int
            f(n + 1)
        main() int: boo(foo2, 1) # flt -> int implicit casting
                                 # doesn't work for a func type
    )SRC")
        .shouldVerified(false);
}

TEST_F(defFuncTest, simpleClosure) {
    make()
        .parse(R"SRC(
        def a
            foo(n int) int: n + 5
        def b
            foo(n int) int: n - 2
        foo(f a.foo, n int) int: f(n)
        main() int: foo(a.foo, 1) + foo(b.foo, 1) # 6 + -1
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 5);
}

TEST_F(defFuncTest, simpleConvergence) {
    make()
        .parse(R"SRC(
        def person
            age int
            foo(newAge age) age: age = newAge; newAge # assignment is not expr.
        main() int
            person.foo(5)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 5);
}

TEST_F(defFuncTest, complexConvergence) {
    make()
        .parse(R"SRC(
        def person
            age := name.len() + 20    # 1)
            addAge(newAge age) void   # 2)
                age += newAge
            name := defaultName + "1" # 3)
            defaultName := "kniz"     # 4)

        main() int
            person.age # == 25
    )SRC")
        .shouldVerified(true);

    // what this test do?:
    //  this is to check whether type convergence works even in complex code that is not sequential
    //  and has step-by-step dependencies.
    //
    //  parser and expander read and evaluate codes from top to bottom.
    //  so they visit codes like '1 -> 2 -> 3' flow.
    //  however, when expander visit code #1, it can't infer type because name wasn't define yet.
    //  so when expander visit code #2, it also couldn't converge parameter type because age is not
    //  expanded. therefore, the appropriate expand sequence should be: 4 -> 3 -> 1 -> 2.

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 25);
}

TEST_F(defFuncTest, simpleNestedFunc) {
    make()
        .parse(R"SRC(
        main() int
            foo(n int) flt: n + 0.5
            value int := (foo(3) + 0.5)
            ret value
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 4);
}

TEST_F(defFuncTest, capturedNestedFunc) {
    make()
        .parse(R"SRC(
        main() int
            name := "chales"
            hello(msg str) str
                ret "$name: $msg"

            res := hello("hello kniz!")
            res == "chales: hello kniz!"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(defFuncTest, capturedVariablePreservedEvenWhenAssigning) {
    make()
        .parse(R"SRC(
        main() int
            name := "chales"
            hello(msg str) str
                ret "$name: $msg"

            name = "kniz" # did you expect variable `name` captured inside 'hello()' will change?
            res := hello("hello kniz!")
            res == "chales: hello kniz!" # nope. it shouldn't.
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(defFuncTest, capturedVariableCanChangeItsFields) {
    make()
        .parse(R"SRC(
        def Person
            ctor(newName str): name = newName
            name str

        main() int
            p := Person("chales")
            hello(msg str) str
                ret "${p.name}: $msg"

            p.name = "kniz" # did you expect that fields of variable `p` captured inside 'hello()' can change?
            res := hello("hello kniz!")
            res == "kniz: hello kniz!" # yes!
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(defFuncTest, variableInsideBlockCanBeCaptured) {
    make()
        .parse(R"SRC(
        main() int
            res := 0
            if true
                name := "chales"
                hello(msg str) str
                    ret "$name: $msg"
                res = "chales: hello kniz!" == hello("hello kniz!")
            res
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(defFuncTest, nestedFuncOnlyAvailableInBlockNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() int
            if true
                name := "chales"
                hello(msg str) str
                    ret "$name: $msg"
            ret "chales: hello kniz!" == hello("hello kniz!")
    )SRC")
        .shouldVerified(false);
}

TEST_F(defFuncTest, TheFuncShouldNotDefineReturnTypeAsNestedFuncItHas) {
    make()
        .negative()
        .parse(R"SRC(
        foo() nestedFoo # <--- error?
            ret nestedFoo(n int) int: n + 3
        main() int
            foo()(-3)
    )SRC")
        .shouldVerified(false);
}

TEST_F(defFuncTest, nestedFuncShouldBeAbleToCaptureArgument) {
    make()
        .parse(R"SRC(
        makeClosure(n int) int
            ret if n >= 0
                multiply(input int) int: input * n
                multiply(3)
            else
                subtract(input int) int: input + n
                subtract(-3)
        main() int
            ret makeClosure(3) + makeClosure(-3)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 9 - 6);
}

TEST_F(defFuncTest, nestedFuncClosure) {
    make()
        .parse(R"SRC(
        foo(input int) int: 0
        makeClosure(n int) foo
            ret if n >= 0
                multiply(input int) int: input * n
            else
                subtract(input int) int: input + n
        main() int
            multi := makeClosure(-5)
            sub := makeClosure(5)
            multi(3) + sub(3)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), -2 + 15);
}

TEST_F(defFuncTest, nestedFuncClosureWithAbstractFunc) {
    make()
        .parse(R"SRC(
        foo(input int) int
        makeClosure(n int) foo
            ret if n >= 0
                multiply(input int) int: input * n
            else
                subtract(input int) int: input + n
        main() int
            multi := makeClosure(-5)
            sub := makeClosure(5)
            multi(3) + sub(3)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), -2 + 15);
}

TEST_F(defFuncTest, nestedFuncClosureAndCallDirectly) {
    make()
        .parse(R"SRC(
        foo(input int) int
        makeClosure(n int) foo
            ret if n >= 0
                multiply(input int) int: input * n
            else
                subtract(input int) int: input + n
        main() int
            makeClosure(-5)(3) + makeClosure(5)(3)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), -2 + 15);
}

TEST_F(defFuncTest, closureShouldCanCaptureInMainFunc) {
    make()
        .parse(R"SRC(
        foo(inpux int) int
        runClosure(foo', input int) int
            foo(input) # interpreter misrecognize this as 'input()' builtin func

        main() int
            nestedAdd(n int) int: n + 3
            runClosure(nestedAdd, 2)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 5);
}

TEST_F(defFuncTest, simpleLambda) {
    make()
        .parse(R"SRC(
        foo(n int) int
        add(foo', input int) int
            ret foo(input)

        main() int
            ret add((n int) int # <-- lambda
                 n + 3
            , 10)
   )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 13);
}

TEST_F(defFuncTest, simpleLambdaInline) {
    make()
        .parse(R"SRC(
        foo(n int) int
        add(foo', input int) int: foo(input)
        main() int
            add((n int) int: n + 3, 10)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 13);
}

TEST_F(defFuncTest, voidParameterNotAllowedNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo(n void) int: ret 3
        main() int: 0
    )SRC")
        .shouldVerified(false);
}

TEST_F(defFuncTest, voidParameterNotAllowedInLambdaNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo() int
        callClosure(foo') int
            foo()
        main() int
            callClosure((n void) int: 6)
    )SRC")
        .shouldVerified(false);
}

/* TODO: uncomment this.
TEST_F(defFuncTest, complexLambda) {
    make()
        .parse(R"SRC(
        def Person
            name str
            ctor(n name): name = n

        def class
            def handler((Person') void: ;)
                onHandle(Person') void: ;
                listener := onHandle
                ctor(onHandle'): listener = onHandle
                handle(Person') void: listener(person)

            _hdlr := handler
            people Person[]

            addHandler(handler') void: hdlr = handler
            add(Person') void
                people.add(person)
                hdlr.handle(person)

        main() int
            sum := 0
            h := class.handler((p Person) void
                sum++
            )
            class.addHandler(h)
            class.add(Person("Chales"))
            class.add(Person("kniz"))
            class.add(Person("Kojima"))

            ret sum == class.people.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}*/
