#include "test/byeolTest.hpp"

using namespace by;
using namespace std;

struct objTest: public byeolTest {};

namespace {
    class originObj: public obj {
        BY(CLASS(originObj, obj))

    public:
        originObj(): super(), _subs(*new scope()) {}

        originObj(const scope& subs): super(), _subs(subs) {}

        originObj(const me& rhs): super(rhs), _subs(rhs._subs) {}

        const baseObj& getOrigin() const override { return *this; }

        me& operator=(const me& rhs) {
            if(this == &rhs) return *this;

            _subs = rhs._subs;
            return *this;
        }

        using super::subs;

        scope& subs() override { return *_subs; }

    private:
        tstr<scope> _subs;
    };
}

TEST_F(objTest, testMakeOriginObj) {
    const nchar* o2Name = "o2";
    originObj o1, o2(*new scope());
    ASSERT_NE(o1.getId(), o2.getId());

    scope* newSubs = new scope();
    originObj o3(*newSubs);

    ASSERT_EQ(o3.subs().len(), 0);
    newSubs->add("o1", o1);
    newSubs->add(o2Name, o2);
    ASSERT_EQ(o3.subs().len(), 2);

    ASSERT_FALSE(o3.sub(""));
    ASSERT_TRUE(o3.sub("o1"));
    node& found = o3.sub(o2Name) OR_ASSERT(found);
    ASSERT_EQ(&found, &o2);
}

TEST_F(objTest, testCloneOriginObj) {
    const nchar* o2Name = "wow";
    originObj o1, o2(*new scope());
    scope* newSubs = new scope();
    originObj o3(*newSubs);

    tstr<originObj> clone((originObj*) o3.clone());
    ASSERT_TRUE(clone.get());

    ASSERT_EQ(clone->subs().len(), 0);
    ASSERT_EQ(o3.subs().len(), 0);
    newSubs->add("o1", o1);
    newSubs->add(o2Name, o2);
    ASSERT_EQ(o3.subs().len(), 2);
    ASSERT_EQ(clone->subs().len(), 2);

    ASSERT_FALSE(clone->sub("o3"));
    node& found = clone->sub(o2Name) OR_ASSERT(found);
    ASSERT_EQ(&found, &o2);
}

TEST_F(objTest, cloneByRunFunc) {
    nInt a(5);
    nInt& a1 = (nInt&) *a.clone();
    ASSERT_NE(&a, &a1);
    ASSERT_EQ(*a.cast<int>(), *a1.cast<int>());
}

TEST_F(objTest, addElementIntoOwns) {
    origin my(typeMaker::make<obj>("my"));
    ASSERT_EQ(my.subs().len(), 0);

    my.subs().add("banana", new nInt(1));
    ASSERT_EQ(my.getShares().len(), 0);
    ASSERT_EQ(my.getOwns().len(), 1);
    ASSERT_EQ(*my.getOwns()["banana"].cast<int>(), 1);
}

TEST_F(objTest, objType) {
    originObj o1;
    const type& t = o1.getType();
    ASSERT_EQ(t.getName(), "originObj");
    ASSERT_TRUE(t.getSuper() == ttype<obj>());
}

TEST_F(objTest, whenAssignObjOnlyOwnsShouldBeCloned) {
    obj o1;
    ASSERT_EQ(o1.subs().chainLen(), 2);
    ASSERT_EQ(o1.subs().len(), 0);
    o1.getOwns().add("age", new nInt(38));
    o1.getShares().add("grade", new nFlt(4.0));
    ASSERT_EQ(o1.getOwns().len(), 1);
    ASSERT_EQ(*o1.getOwns()["age"].cast<nint>(), 38);
    ASSERT_EQ(o1.getShares().len(), 1);
    ASSERT_EQ(*o1.getShares()["grade"].cast<nflt>(), 4.0f);
    ASSERT_EQ(o1.subs().len(), 2);

    obj o2;
    o2 = o1;
    ASSERT_EQ(*o2.getOwns()["age"].cast<nint>(), 38);
    ASSERT_NE(&o2.getOwns()["age"], &o1.getOwns()["age"]);
    ASSERT_EQ(*o2.getShares()["grade"].cast<nflt>(), 4.0f);
    ASSERT_EQ(&o2.getShares()["grade"], &o1.getShares()["grade"]);
}
