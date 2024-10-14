#include "../../../namuSyntaxTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct genericsTest: public namuSyntaxTest {};
}

TEST_F(genericsTest, simpleDefineGenerics) {
    // as, defObj
    make()
        .parse(R"SRC(
        print(msg str) str: msg
        def object<T>
            foo() str
                ret print(1 as T)

        main() int
            a := object<str>()
            ret a.foo() == "1"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(genericsTest, defineGenerics) {
    // defProp, func
    make()
        .parse(R"SRC(
        def object<e>
            age e
            foo(num e) e
                ret age + num as e

        main() int
            a := object<int>()
            ret a.foo(2) as str == "2"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(genericsTest, defineGenerics1) {
    make()
        .parse(R"SRC(
        print(msg str) str: msg
        def object<T>
            foo(a T) T
                age := T() + T()
                ret a + age + boo()
            boo() T
                ret T()

        main() int
            a := object<int>()
            print(a.foo(2) as str) == "2"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(genericsTest, genericTwice1) {
    make()
        .parse(R"SRC(
        print(msg str) str: msg

        def object<T>
            foo() void
                print("1")

        main() void
            a := object<str>()
            b := object<bool>()
            c := object<flt>() # run 'b.foo()' occurs F/C
    )SRC")
        .shouldVerified(true);
    run();
    node& object = getSubPack().sub("object");
    str a = object.run(args{nulOf<baseObj>(), *new nStr()});
    ASSERT_TRUE(a);
    str b = object.run(args{nulOf<baseObj>(), *new nFlt()});
    ASSERT_TRUE(b);
    ASSERT_NE(&a.get(), &b.get());

    node& aFoo = a->sub("foo");
    ASSERT_FALSE(nul(aFoo));
    node& bFoo = b->sub("foo");
    ASSERT_FALSE(nul(bFoo));
    ASSERT_NE(&aFoo, &bFoo);
}

TEST_F(genericsTest, genericTwice2Negative) {
    make()
        .negative()
        .parse(R"SRC(
        def object<T>
            foo(val T) T
                age := T()
                print((val + age) as T)
                ret val + age

        main() int
            a := object<int>()
            b := object<str>()
            ret b.foo("3.5") == "3.5"
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(genericsTest, genericTwice2) {
    make()
        .parse(R"SRC(
        print(msg str) str: msg
        def object<T>
            foo(val T) T
                age := T()
                print((val + age) as str)
                ret val + age

        main() int
            a := object<int>()
            b := object<str>()
            ret b.foo("3.5") == "3.5"
    )SRC")
        .shouldVerified(true);
    str ret = run();
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret->cast<nint>(), 1);
}

TEST_F(genericsTest, simpleUseGenerics) {
    make()
        .parse(R"SRC(
        print(msg str) str: msg
        def object<T>
            foo(msg T) str
                print(msg)
                ret msg

        main() int
            o := object<str>()
            res := o.foo("hello generics!\n")
            ret res == "hello generics!\n"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res->cast<nint>(), 1);
}

TEST_F(genericsTest, implicitReturnTest) {
    make()
        .parse(R"SRC(
        def pakuman<T>
            foo() T
                "hello " + "world"

        main() int
            p := pakuman<str>()
            p.foo() == "hello world"
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res->cast<nint>(), 1);
}

TEST_F(genericsTest, simpleUseGenerics2) {
    make()
        .parse(R"SRC(
        print(msg str) str: msg
        def object<T>
            foo(val T) T
                age := T()
                print((val + age) as T)
                ret val + age

        main() int
            b := object<str>()
            ret b.foo("3.5") == "3.5"
    )SRC")
        .shouldVerified(true);

    str ret = run();
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret->cast<nint>(), 1);
}

TEST_F(genericsTest, genericNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def object<T>
            foo(val T) T
                age := T()
                print((val + age) as T)
                ret val + age

        main() int
            b := object<int>() # error at print(int)
            ret b.foo(3.5)
    )SRC")
        .shouldVerified(false);
}

TEST_F(genericsTest, genericParameter2) {
    make()
        .parse(R"SRC(
        def obj<T1, T2>
            foo(v1 T1, v2 T2) str
                ret "v1=" + v1 as str + ", v2=" + v2 as str

        main() int
            ret obj<int, str>().foo(3, "hello") == "v1=3, v2=hello"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(genericsTest, assignDifferentParameterizedTypeNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def A<T>
            age T
        main() void
            a A<int>
            a = A<flt>()
    )SRC")
        .shouldVerified(false);
}

TEST_F(genericsTest, defAssignForPreCtorShouldWork) {
    make()
        .parse(R"SRC(
        def A<T>
            a := T()
        main() int
            a1 A<str>
            a1.a = "hello"
            a1.a == "hello"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(genericsTest, 2DArrayWithGenerics) {
    make()
        .parse(R"SRC(
        def A<T>
            a := {{T()}}
            say() str
                for arr in a
                    for e in arr
                        ret e
        main() int
            a1 A<str>
            a1.a[0][0] = "hello"
            a1.say() == "hello"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(genericsTest, makeGenericOnPreCtor) {
    make()
        .parse(R"SRC(
        def A<T>
            age := 1
        def B<E>
            grade := A<E>().age + 4
        main() int
            B<int>().grade
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 5);
}

TEST_F(genericsTest, make2GenericsOnPreCtor) {
    make()
        .parse(R"SRC(
        def B<E>
            age := E() + 2
        def C<T>
            grade := 4.5
        def A<T>
            age := B<T>().age + 1
            grade := C<T>().grade + 1

        main() int
            A<int>().grade
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 5);
}

TEST_F(genericsTest, make2GenericsOnPreCtor2) {
    make()
        .parse(R"SRC(
        def B<E>
            age := E() + 2
        def C<T>
            grade := 4.5
        def A<T>
            age := B<T>().age + 1 + C<T>().grade + 1

        main() int
            A<int>().age
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 8); // <== (4 + 4.5) as int
}

TEST_F(genericsTest, make2GenericsOnPreCtor3) {
    make()
        .parse(R"SRC(
        def B<E>
            age := E() + 2
        def C<T>
            grade := 4.5
        def A<T>
            age := B<T>().age + 1 + C<flt>().grade + 1

        main() int
            A<int>().age
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 8); // <== (4 + 4.5) as int
}

TEST_F(genericsTest, genericsWillBeVerifiedWhenItIsUsed) {
    make()
        .parse(R"SRC(
        def A<E>
            age := B<E>().age + grade
            grade := age
        def B<T>
            grade := 0
        main() void
            ret
    )SRC")
        .shouldVerified(true);
}

TEST_F(genericsTest, genericsWillBeVerifiedWhenItIsUsedNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def A<E>
            age := B<E>().grade + grade # err: grade is circular dependency
            grade := age
        def B<T>
            grade := 0
        main() void
            A<int>().age
    )SRC")
        .shouldVerified(false);
}

TEST_F(genericsTest, simpleCompleteObj) {
    make()
        .parse(R"SRC(
        def person<E>
            grade E
            age := 33
        main() int
            person<flt>.age + person<flt>.grade
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 33);
}

TEST_F(genericsTest, simpleCompleteObjNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def person<E>
            age := 33
            ctor(n E): age = n
        main() int
            person<int>.age
    )SRC")
        .shouldVerified(false);
}

TEST_F(genericsTest, constToOriginObjNotAllowedNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def PERSON<E>
            age E
        main() int
            PERSON<int>().age
    )SRC")
        .shouldVerified(false);
}

TEST_F(genericsTest, ifAtLeastOneLetterIsLowerCaseThenItIsNotConst) {
    make()
        .parse(R"SRC(
        def PERSOn<E>
            age E
        main() int
            PERSOn<flt>().age
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 0);
}

TEST_F(genericsTest, ifFirstLetterBeginsWithLowerCaseThenItIsComplete) {
    make()
        .parse(R"SRC(
        def pERSON<E>
            age E
            say(value E) E
                age + value
        main() int
            pERSON<flt>.say(4) == 4
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(genericsTest, generalizedObjShouldRemoveExpandFunc) {
    make().parse(R"SRC(
        def Person<E>
            age E
            foo(rhs Person<E>) E
                ret rhs.age
        main() int
            p := Person<int>()
            p.age = 22
            ret Person<int>().foo(p)
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 22);

    str generic = getSubPack() THEN(sub("Person")) THEN(run(args{narr{nInt()}}));
    ASSERT_TRUE(generic);
    ASSERT_TRUE(nul(generic->sub(baseObj::EXPAND_NAME)));
}
