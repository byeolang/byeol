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
    ASSERT_TRUE(nextArr.in("foo"));
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
        def Person
            name := "default"

        foo(p Person) str
            p.name

        main() int
            foo(Person).len()
    )SRC")
        .shouldVerified(false);
}

TEST_F(defObjExprTest, incompleteObjNegative2) {
    make()
        .negative()
        .parse(R"SRC(
        def Person
            name := "default"

        foo(p Person) str
            p.name

        main() int
            ret Person.name.len()
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

TEST_F(defObjExprTest, expandReversedSequence) {
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
    // namu should return 22 in this scenario.
    // because `boo()` belongs to this `pack` obj.
    // so when `boo()` get called it should refer variable `age` belonged to `pack`.
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

TEST_F(defObjExprTest, defPropNotAllowedIfThereIsNoProperCtorNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def a
            age int
            ctor(n int)
                age = n
        main() int
            a1 a
            a1.age
    )SRC")
        .shouldVerified(false);
}

TEST_F(defObjExprTest, simpleCompleteObj) {
    make()
        .parse(R"SRC(
        def person # complete object
            age := 33
        main() int
            person.age
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 33);
}

TEST_F(defObjExprTest, simpleCompleteObjNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def person
            age := 33
            ctor(n int): age = n
        main() int
            person.age
    )SRC")
        .shouldVerified(false);
}

TEST_F(defObjExprTest, constToOriginObjNotAllowedNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def PERSON
            age := 33
        main() int
            PERSON().age
    )SRC")
        .shouldVerified(false);
}

TEST_F(defObjExprTest, ifAtLeastOneLetterIsLowerCaseThenItIsNotConst) {
    make()
        .parse(R"SRC(
        def PERSOn
            age := 33
        main() int
            PERSOn().age
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 33);
}

TEST_F(defObjExprTest, ifFirstLetterBeginsWithLowerCaseThenItIsComplete) {
    make()
        .parse(R"SRC(
        def pERSON
            age := 33
        main() int
            pERSON.age
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 33);
}

TEST_F(defObjExprTest, isStateOfDefObjVerified) {
    make()
        // if I set LOG_STRUCTURE flag, graphVisitor will all Person.subs(),
        // and it lets the object set to LINKED state.
        .delFlag(interpreter::LOG_STRUCTURE)
        .parse(R"SRC(
        def Person
            age int
        main() int: 0
    )SRC")
        .shouldVerified(true);

    obj& subpack = getSubPack().cast<obj>();
    ASSERT_TRUE(subpack.getState() >= VERIFIED);
    obj& person = subpack.sub<obj>("Person");
    ASSERT_FALSE(nul(person));
    ASSERT_EQ(person.getState(), VERIFIED);

    person.subs();
    ASSERT_EQ(person.getState(), VERIFIED); // it's not work because there is no frame.

    threadUse thr1;
    {
        frameInteract f1(person);
        person.subs();
    }
    ASSERT_EQ(person.getState(), LINKED);
}

TEST_F(defObjExprTest, verifierShouldVerifyCallCompleteNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def person("unknown") # <-- complete obj. and this'll be looking for ctor(str).
            name str
            ctor(): name = "no"
            ctor(newName flt): name = newName # <-- but look! it's not str, but flt.
        main() int: 0
    )SRC")
        .shouldVerified(false);
}

TEST_F(defObjExprTest, callCompleteForDefaultCtor) {
    make()
        .parse(R"SRC(
        def person
            name str
            ctor(): name = "kniz"
        main() int: person.name == "kniz"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(defObjExprTest, CallCompleteMakeIncompleteToCompleteType) {
    make()
        .parse(R"SRC(
        def person("unkown")
            name str
            ctor(): ;
            ctor(newName str): name = newName
        main() int: person.name.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 6);
}

TEST_F(defObjExprTest, NoCallCompleteForIncompleteNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def Person("unkown")
            name str
            ctor(): ;
            ctor(newName str): name = newName
        main() int: Person().name.len()
    )SRC")
        .shouldVerified(false);
}

TEST_F(defObjExprTest, doesItHaveCommonCtor) {
    make()
        .parse(R"SRC(
        def person
            name str
            ctor(): name = "kniz"
    )SRC")
        .shouldVerified(true);
}

TEST_F(defObjExprTest, memberVariableShouldBeNotMockNode) {
    make()
        .parse(R"SRC(
        def person
            age int # <-- if this is mockNode, age is origin of int.
        def fish
            age int # <-- if so, this age would be the just same instance of person's.
        main() int
            fish.age = 17
            person.age
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 0);
}

TEST_F(defObjExprTest, memberVariableShouldBeNotMockNode2) {
    make()
        .parse(R"SRC(
        def person
            age int # <-- if this is mockNode, age is origin of int.
        def fish
            p1 person
        main() int
            person.age = 17
            fish.p1.age
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 0);
}

TEST_F(defObjExprTest, simpleModifier) {
    make()
        .parse(R"SRC(
        def person
            _age := 23
            say() int: age
        main() int
            person.say()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 23);
}

TEST_F(defObjExprTest, simpleModifierNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def person
            _age := 23
        main() int
            person.age
    )SRC")
        .shouldVerified(false);
}

TEST_F(defObjExprTest, clonedObjModifierNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def person
            _age := 23
        main() int
            p person
            p.age
    )SRC")
        .shouldVerified(false);
}

TEST_F(defObjExprTest, clonedObjModifierNegative2) {
    make()
        .negative()
        .parse(R"SRC(
        def person
            _age := 23
        main() int
            p := person()
            p.age
    )SRC")
        .shouldVerified(false);
}

TEST_F(defObjExprTest, modifierForAnotherObjScope) {
    make()
        .parse(R"SRC(
        def person
            _age := 22
            say() int
                person.age
        main() int
            person.say()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 22);
}

TEST_F(defObjExprTest, modifierForAnotherObjScope2) {
    make()
        .parse(R"SRC(
        age := 33
        def person
            _age := 22
        main() int
            age
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 33);
}

TEST_F(defObjExprTest, simpleModifierForFuncNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def person
            _age := 23
            _say() int: age
        main() int
            person.say()
    )SRC")
        .shouldVerified(false);
}

TEST_F(defObjExprTest, simpleModifierForFunc) {
    make()
        .parse(R"SRC(
        def person
            _age := 23
            _say() int: age + 1
            boo() int: say() + 1
        main() int
            person.boo()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 25);
}

TEST_F(defObjExprTest, clonedObjModifierForFuncNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def person
            _age := 23
            _say() int: age + 1
        main() int
            p := person()
            p._say()
    )SRC")
        .shouldVerified(false);
}

TEST_F(defObjExprTest, modifierForFuncAndAnotherObjScope) {
    make()
        .parse(R"SRC(
        def person
            ctor(): ;
            ctor(newAge int): age = newAge
            age := 22
            _say() int: age
            boo() int: person.say()
        main() int
            p := person(38)
            ret p.boo()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 22);
}

TEST_F(defObjExprTest, modifierForFuncAndAnotherObjScope2) {
    make()
        .parse(R"SRC(
        foo() int: 3
        def person
            _foo() int: 2
            say() int: foo()
        main() int: person.say()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 2);
}

TEST_F(defObjExprTest, nested) {
    make()
        .parse(R"SRC(
        def a
            def b
                year := 2024
                def c
                    age := 38

        main() int: a.b.year + a.b.c.age
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 2062);
}

TEST_F(defObjExprTest, iterateObj) {
    make()
        .parse(R"SRC(
        def Person
            _age int
            _name str := "Unknown"

            ctor(newAge age): age = newAge
            ctor(newAge age, newName name): age = newAge; setName(newName)

            setName(newName str) void
                name = newName

            say() str: "$name: I'm $age years old.\n"

        main() int
            bookmarks str
            for p in {Person(22), Person(38, "kniz"), Person(17, "Highscool")}
                bookmarks += p.say()
            bookmarks == "Unknown: I'm 22 years old.\nkniz: I'm 38 years old.\nHighscool: I'm 17 years old.\n"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res->cast<nint>(), 1);
}

TEST_F(defObjExprTest, passNestedFuncAsCompleteCallParam) {
    make()
        .parse(R"SRC(
        onHandle(n int) int
        def handler((n int) int: n + 3)
            value int
            ctor(onHandle'): value = onHandle(-3) # -3 + 3 = 0

        main() int
            handler.value
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 0);
}

TEST_F(defObjExprTest, assignClosureShouldWork) {
    make()
        .parse(R"SRC(
        def handler
            onHandle(n int) int: 0
            listener onHandle
            setHandler(onHandle') void: listener = onHandle
            handle(n int) int: listener(n)

        main() int
            handler.setHandler((n int) int: n + 1)
            handler.handle(1)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 2);
}

TEST_F(defObjExprTest, originShouldNotOpenAccessToPackScopeNegative) {
    make()
        .negative()
        .parse(R"SRC(
        age := 12 # in pack scope
        def a
            foo() int: age
        main() int
            a().age
    )SRC")
        .shouldVerified(false);
}
