#include "../../../namuSyntaxTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct defObjExprTest: public namuSyntaxTest {};
}

TEST_F(defObjExprTest, simpleDefineObject) {
    make()
        .parse(R"SRC(
        def A
            foo() void
                print("hello")
        main() void
            a1 := A()
    )SRC")
        .shouldVerified(true);
    run();
}

TEST_F(defObjExprTest, objMakeScopeWithOwnsAndSharesButNotPackScope) {
    make()
        .parse(R"SRC(
        def a
            age := 22
            foo() int: age
        main() void
            a().foo()
    )SRC")
        .shouldVerified(true);

    node& root = getSubPack();
    ASSERT_FALSE(nul(root));

    node& a = root.sub("a");
    ASSERT_FALSE(nul(a));

    const scope& s = a.subs();
    ASSERT_FALSE(nul(s));
    ASSERT_TRUE(s.len() > 2);
    const auto& sArr = s.getContainer();
    ASSERT_EQ(sArr.len(), 1);
    ASSERT_EQ(sArr.begin().getKey(), "age");
    ASSERT_TRUE(sArr.begin()->isSub<nInt>());

    ASSERT_EQ(s.chainLen(), 2); // owns + shares
    const auto& next = s.getNext();
    ASSERT_FALSE(nul(next));
    ASSERT_EQ(next.len(), 3); // foo() + 2 x @ctor

    const auto& nextArr = next.getContainer();
    ASSERT_FALSE(nul(nextArr));
    ASSERT_EQ(nextArr.len(), 3);
    ASSERT_TRUE(nextArr.has("foo"));
}

TEST_F(defObjExprTest, simpleDefineObject2) {
    make()
        .parse(R"SRC(
        print(msg str) void: 1

        def A
            foo() void
                print("hello")
        main() void
            a A
            a.foo()
    )SRC")
        .shouldVerified(true);
    run();
}

TEST_F(defObjExprTest, simpleDefineObjectNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def A
            foo() void
                print("hello")
        main() void
            a B
            a.foo()
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defObjExprTest, assignMemberVariable) {
    make()
        .parse(R"SRC(
        print(msg str) void: 1

        def obj
            index := 5

        main() int
            o := obj()
            o.index = 2
            print(o.index as str)
            o.index
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 2);
}

TEST_F(defObjExprTest, make2Objects) {
    make()
        .parse(R"SRC(
        def obj
            name := ""
            age int

        main() int
            o1 := obj()
            o1.name = "Chales"
            o1.age = 36
            o2 := obj()
            o2.name = "kniz"
            o2.age = 22

            (o1.name + o2.name + o1.age as str + o2.age as str) == "Chaleskniz3622"
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(defObjExprTest, manipulate2Origin) {
    make()
        .parse(R"SRC(
        def obj
            name := ""
            age int

        main() int
            o1 obj
            o1.name = "Chales"
            o1.age = 36
            o2 obj
            o2.name = "kniz"
            o2.age = 22

            ret (o1.name + o2.name + o1.age as str + o2.age as str) == "Chaleskniz3622"
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(defObjExprTest, objAsParameter) {
    make()
        .parse(R"SRC(
        def person
            name := "default"

        foo(p person) str
            p.name

        main() int
            foo(person()) == "default"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(defObjExprTest, incompleteObjNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def person
            name := "default"

        foo(p person) str
            p.name

        main() int
            foo(person).len()
    )SRC")
        .shouldVerified(false);
}

TEST_F(defObjExprTest, incompleteObjNegative2) {
    make()
        .negative()
        .parse(R"SRC(
        def person
            name := "default"

        foo(p person) str
            p.name

        main() int
            ret person.name.len()
    )SRC")
        .shouldVerified(false);
}

TEST_F(defObjExprTest, defVarWithVoidNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def person
            n void
        main() void
            ret
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);

    errReport& rpt = getReport();
    ASSERT_TRUE(rpt);
    ASSERT_EQ(rpt[0].cast<nerr>().getErrCode(), errCode::VOID_CANT_DEFINED);
}

TEST_F(defObjExprTest, defSameObjAndFuncNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def foo
            age := 0
        foo(n int) void
            ret
        main() void
            ret
    )SRC")
        .shouldVerified(false);
}

TEST_F(defObjExprTest, defSameObjNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def A
            age := 0
        def A
            grade := 0.5
        main() void
            ret
    )SRC")
        .shouldVerified(false);
}

TEST_F(defObjExprTest, testPrector) {
    make()
        .parse(R"SRC(
        def A
            age := B().age + 1
        def B
            age := 0
        main() int
            A().age
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(defObjExprTest, testPrectorCircularNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def A
            age := B().age
        def B
            age := A().age
        main() void
            ret
    )SRC")
        .shouldVerified(false);
}

TEST_F(defObjExprTest, preCtorReversedSequence) {
    make()
        .parse(R"SRC(
        def A
            age := B().grade + grade
            grade := 1

        def B
            grade := age + 1
            age := 1

        main() int
            A().age
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 3);
}

TEST_F(defObjExprTest, variableDuplication) {
    make()
        .parse(R"SRC(
        age := 22
        def a
            age := 33
            foo() int
                ret age
        main() int
            ret a().foo()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 33);
}

TEST_F(defObjExprTest, frameNotCreatedWhenCallPackFunc) {
    make()
        .parse(R"SRC(
        age := 22
        boo() int
            ret age
        def a
            age := 33
            foo() int
                ret boo()
        main() int
            a().foo()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 22);
    // namu returns 22 in this scenario so far.
    // because it doesn't make a frame object and add to current thread when frame::inFrame() func
    // get called. so it reuses previous frame instance when boo() called.
}

TEST_F(defObjExprTest, defPropAllowedIfThereIsProperCtor) {
    make()
        .parse(R"SRC(
        def a
            age int
            ctor()
                age = 22
        main() int
            a1 a
            a1.age
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 22);
}
