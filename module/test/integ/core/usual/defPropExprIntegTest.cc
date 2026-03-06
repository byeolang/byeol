#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct defPropExprIntegTest: public byeolIntegTest {};
}

TEST_F(defPropExprIntegTest, simpleDefineVariable) {
    if(make()
            .parse(R"SRC(
        main() void
            age int
            ret
    )SRC")
            .shouldVerified(true)) {
        node& res = getSubPack() OR_ASSERT(res);
        const func& f = res.sub<func>("main", narr()) OR_ASSERT(f);

        const narr& stmts = f.getBlock().getStmts();
        ASSERT_EQ(stmts.len(), 2);

        const defPropExpr& defProp = stmts[0].cast<defPropExpr>() OR_ASSERT(defProp);
        ASSERT_EQ(defProp.getName(), "age");
        ASSERT_EQ(defProp.getRight()->getType(), ttype<nInt>());
    }
}

TEST_F(defPropExprIntegTest, definePackVariableNegative) {
    make()
        .negative()
        .parse(R"SRC(
        name str
        age int
        grade flt
        main() void
            age int
            ret age
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defPropExprIntegTest, definePackVariable2) {
    make()
        .parse(R"SRC(
        name str
        age int
        grade flt
        main() int
            age int
            ret age
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);
    slot& s = getSlot() OR_ASSERT(s);
    ASSERT_EQ(s.getManifest().name, manifest::DEFAULT_NAME);

    scope::super& owns = (scope*) (getSlot() TO(subs().getContainer())) OR_ASSERT(owns);
    scope::super& shares = (scope*) (getSlot() TO(subs().getNext()) TO(getContainer())) OR_ASSERT(shares);
    ASSERT_EQ(owns.len(), 3);
    ASSERT_EQ(shares.len(), 3);
    ASSERT_EQ(owns.getAll<baseObj>().len(), 3);
    ASSERT_EQ(s.subAll<func>().len(), 1);

    nStr& name = s.sub<nStr>("name") OR_ASSERT(name);
    ASSERT_EQ(name.get(), "");

    nInt& age = s.sub<nInt>("age") OR_ASSERT(age);
    ASSERT_EQ(age.get(), 0); // 0 is default value.

    nFlt& grade = s.sub<nFlt>("grade") OR_ASSERT(grade);
    ASSERT_EQ(grade.get(), 0.0f);
}

TEST_F(defPropExprIntegTest, defPropVoidNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a void
            ret a
    )SRC")
        .shouldVerified(false);
}

TEST_F(defPropExprIntegTest, passingVoidIsOk) {
    make()
        .parse(R"SRC(
        foo() void
            ret
        main() void
            ret foo() # <-- ok
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->isSub<nVoid>());
}

TEST_F(defPropExprIntegTest, defVoidContainerNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a void[]
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(defPropExprIntegTest, defPropWithObj) {
    make()
        .parse(R"SRC(
        def a
            age := 22
        a1 a
        a2 a
        main() int
            a2.age
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 22);
}

TEST_F(defPropExprIntegTest, defPropWithAccess) {
    make()
        .parse(R"SRC(
        def a
            def b
                def c
                    name := "chales"
                    age := 38
        main() int
            age a.b.c.age
            age = a.b.c.age
            age
    )SRC")
        .shouldVerified(true);
}

TEST_F(defPropExprIntegTest, shortDefinition) {
    make()
        .parse(R"SRC(
        def person
            age := 2

        foo() int
            person`
            person.age = 3 # person is local variable.
            ret person.age

        main() int
            ret foo() + person.age # this is for origin obj
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 5);
}

TEST_F(defPropExprIntegTest, shortDefinitionOnParam) {
    make()
        .parse(R"SRC(
        def person
            age := 0
            ctor(person`)
                age = person.age + 1
        foo(person`) int
            person.age + 1

        main() int
            p1 := person(person) # p1.age == 1
            foo(p1)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 2);
}

TEST_F(defPropExprIntegTest, shortDefinitionNotAllowPrimitiveNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo(int`) int: 1
        main() int
            foo(2)
    )SRC")
        .shouldVerified(false);
}

TEST_F(defPropExprIntegTest, shortDefinitionNotAllowContainerNegative) {
    make()
        .negative()
        .parse(R"SRC(
        foo(int[]') int: 1
        main() int
            foo(2)
    )SRC")
        .shouldVerified(false);
}

TEST_F(defPropExprIntegTest, shortDefinitionNotAllowContainerNegative2) {
    make()
        .negative()
        .parse(R"SRC(
        def Person
            age int
        foo(Person[]') int: 1
        main() int
            foo({Person()})
    )SRC")
        .shouldVerified(false);
}

TEST_F(defPropExprIntegTest, shortDefinitionWithGenerics) {
    make()
        .parse(R"SRC(
        def Person<E>
            value E
        foo(Person<int>') int
            person.value + 1 # short-definition always replaces to define lowercased name.
        main() int
            p1 Person<int>
            p1.value = 33
            foo(p1)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 34);
}

TEST_F(defPropExprIntegTest, userShouldBeAbleToDefineProtectedProperty) {
    make()
        .parse(R"SRC(
        def Person
            _phone Phone
            ctor(b str): phone.brand = b
            hello() str: "my name is ${phone.brand}"
        def Phone
            brand str
            ctor(): ;
            ctor(b brand): brand = b
        main() int
            Person("kniz").hello() == "my name is kniz"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(defPropExprIntegTest, cantDefineFuncTypePropertyNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def handler
            onHandle(n int) int
            plus1(n int) int: n + 1
            listener onHandle
        int main()
            handler.listener(1)
    )SRC")
        .shouldVerified(false);
}

TEST_F(defPropExprIntegTest, canDefineFuncTypePropertyIfItsNotAbstract) {
    make()
        .parse(R"SRC(
        def handler
            plus1(n int) int: n + 1
            listener plus1
        main() int
            handler.listener(1)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 2);
}

TEST_F(defPropExprIntegTest, canDefAssignFuncTypeProperty) {
    make()
        .parse(R"SRC(
        def handler
            onHandle(n int) int
            plus1(n int) int: n + 1
            listener onHandle := plus1
        main() int
            handler.listener(1)
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 2);
}
