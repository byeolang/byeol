#include "test/namuSyntaxTest.hpp"

using namespace nm;
using namespace std;

struct mgdObjTest: public namuSyntaxTest {};

namespace {
    struct myObj: public obj {
        NM(CLASS(myObj, obj))
    };
}

TEST_F(mgdObjTest, testGetOriginPointingThis) {
    myObj obj1;
    ASSERT_EQ(&obj1.getOrigin(), &obj1);

    tstr<obj> copied((obj*) obj1.clone());
    ASSERT_TRUE(copied);
    ASSERT_NE(copied.get(), &obj1);
}

TEST_F(mgdObjTest, testAccessCompleteObject) {
    make()
        .parse(R"SRC(
        def a
            age := 3
        main() int
            b := a()
            b.age = 7
            c := b()
            ret c.age
    )SRC")
        .shouldVerified(true);

    str ret = run();
    ASSERT_TRUE(ret);
    ASSERT_EQ(*ret.cast<nint>(), 3);
}

TEST_F(mgdObjTest, testAccessInCompleteObjectNegative) {
    make()
        .negative()
        .parse(R"SRC(
        def A
            age := 3
        main() int
            b := A
            b.age = 7
            c := b()
            ret c.age
    )SRC")
        .shouldVerified(false);
}

TEST_F(mgdObjTest, testAccessInCompleteObjectNegative2) {
    make()
        .negative()
        .parse(R"SRC(
        def a
            age := 3
        main() str
            b := a()
            b.age = 7
            ret b.age as str + a.age as str
    )SRC")
        .shouldVerified(false);
}

TEST_F(mgdObjTest, distinguishPackScopeAndObjScopeByItsOwner) {
    make()
        .parse(R"SRC(
        # there are {default} pack.@ctor()
        def a
            # 'a' also has @ctor.
            age := 0
    )SRC")
        .shouldVerified(true);

    obj& a = getSubPack() TO(template sub<obj>("a")) OR_ASSERT(a);
    auto pr = a.subAll<baseFunc>(baseObj::CTOR_NAME, args());
    ASSERT_EQ(pr.len(), 1); // 'a' obj's ctor.

    {
        // get all funcs regardless of argument.
        auto matches = a.getSubPack().subAll<baseFunc>(baseObj::CTOR_NAME, nullptr);
        ASSERT_EQ(matches.len(), 2);
    }

    {
        auto matches = a.getSubPack().subAll<baseFunc>(baseObj::CTOR_NAME, args());
        ASSERT_EQ(matches.len(), 1); // default-pack's ctor.
        ASSERT_EQ(matches.len(), 1);
        ASSERT_TRUE(matches.get());
    }
}

TEST_F(mgdObjTest, clonedObjDoesntCloneSharesDeeply) {
    make()
        .parse(R"SRC(
        def b
            grade := 3.5
            foo() void
                print("hello")
        def a
            age := 22
            b1 b
        o1 a
        o2 a
        main() void
            print("ok")
    )SRC")
        .shouldVerified(true);

    obj& o1 = getSubPack()->sub<obj>("o1") OR_ASSERT(o1);
    obj& o2 = getSubPack()->sub<obj>("o2") OR_ASSERT(o2);

    ASSERT_EQ(o1.getOwns().len(), 2);
    ASSERT_EQ(*o1.getOwns()["age"].cast<nint>(), 22);
    obj& o1b1 = o1.sub<obj>("b1") OR_ASSERT(o1b1);
    ASSERT_EQ(o1b1.getOwns().len(), 1);
    ASSERT_EQ(*o1b1.getOwns()["grade"].cast<nflt>(), 3.5f);

    const func& b1foo = o1b1.getShares().get<func>("foo") OR_ASSERT(b1foo);
    ASSERT_EQ(b1foo.getParams().len(), 0);
    ASSERT_TRUE(b1foo.getRet()->isSub<nVoid>());


    ASSERT_EQ(o2.getOwns().len(), 2);
    ASSERT_EQ(*o2.getOwns()["age"].cast<nint>(), 22);
    obj& o2b1 = o2.sub<obj>("b1") OR_ASSERT(o2b1);
    ASSERT_EQ(o2b1.getOwns().len(), 1);
    ASSERT_EQ(*o2b1.getOwns()["grade"].cast<nflt>(), 3.5f);

    const func& b2foo = o2b1.getShares().get<func>("foo") OR_ASSERT(b2foo);
    ASSERT_EQ(b2foo.getParams().len(), 0);
    ASSERT_TRUE(b2foo.getRet()->isSub<nVoid>());

    ASSERT_NE(&o1b1.getOwns()["grade"], &o2b1.getOwns()["grade"]);
    ASSERT_EQ(&b1foo, &b2foo);
}
