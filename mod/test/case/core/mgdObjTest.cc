#include "../../namuSyntaxTest.hpp"

using namespace namu;
using namespace std;

struct mgdObjTest : public namuSyntaxTest {};

namespace {
    struct myObj : public obj {
        NAMU(CLASS(myObj, obj))
    };
}

TEST_F(mgdObjTest, testGetOriginPointingThis) {
    myObj obj1;
    ASSERT_FALSE(nul(obj1.getOrigin()));
    ASSERT_EQ(&obj1.getOrigin(), &obj1);

    tstr<obj> copied((obj*) obj1.clone());
    ASSERT_TRUE(copied);
    ASSERT_NE(&copied.get(), &obj1);
}

TEST_F(mgdObjTest, testAccessCompleteObject) {
    make().parse(R"SRC(
        def a
            age := 3
        main() int
            b := a()
            b.age = 7
            c := b()
            ret c.age
    )SRC").shouldVerified(true);

    str ret = run();
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.cast<nint>(), 3);
}

TEST_F(mgdObjTest, testAccessInCompleteObjectNegative) {
    make().negative().parse(R"SRC(
        def a
            age := 3
        main() int
            b := a
            b.age = 7
            c := b()
            ret c.age
    )SRC").shouldVerified(false);
}

TEST_F(mgdObjTest, testAccessInCompleteObjectNegative2) {
    make().negative().parse(R"SRC(
        def a
            age := 3
        main() str
            b := a()
            b.age = 7
            ret b.age as str + a.age as str
    )SRC").shouldVerified(false);
}

TEST_F(mgdObjTest, distinguishPackScopeAndObjScopeByItsOwner) {
    make().parse(R"SRC(
        // there are {default} pack.@ctor()
        def a
            // 'a' also has @ctor.
            age := 0
    )SRC").shouldVerified(true);

    obj& a = getSubPack().sub<obj>("a");
    ASSERT_FALSE(nul(a));
    tpriorities<baseFunc> pr = a.subAll<baseFunc>(baseObj::CTOR_NAME, args(narr()));
    ASSERT_EQ(pr.len(), 1); // 'a' obj's ctor.
    auto pr2 = a.getSubPack().subAll<baseFunc>(baseObj::CTOR_NAME, args(narr()));
    ASSERT_EQ(pr2.len(), 1); // default-pack's ctor.
    auto matches = pr.getMatches();
    ASSERT_EQ(matches.len(), 1);
    ASSERT_TRUE(pr.getMatch());
}
