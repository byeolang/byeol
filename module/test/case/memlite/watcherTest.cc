#include "test/namuTest.hpp"

using namespace std;
using namespace nm;

struct watcherTest: public namuTest {};

namespace {
    struct A: public instance {
        int age;

        const type& getType() const override { return ttype<A>::get(); }

        clonable* clone() const override { return new A(); }
    };

    struct B: public A {
        float grade;

        const type& getType() const override { return ttype<B>::get(); }

        clonable* clone() const override { return new B(); }
    };

    void addRemoveIntegrityTest(int cnt) {
        vector<A*> tray;
        vector<id> ids;

        for(int n = 0; n < cnt; n++) {
            A* new1 = new B();
            tray.push_back(new1);

            id newId = new1->getId();
            ASSERT_EQ(newId, new1->getBindTag()->getId());
            ids.push_back(newId);
        }

        for(int n = 0; n < cnt; n++)
            ASSERT_EQ(tray[n]->getId(), ids[n]);
    }

    void addIntegrityTest(int cnt) {
        vector<A*> tray;
        vector<id> ids;

        for(int n = 0; n < cnt; n++) {
            A* new1 = new B();
            tray.push_back(new1);

            id newId = new1->getId();
            ASSERT_EQ(newId, new1->getBindTag()->getId());
            ids.push_back(newId);
        }

        for(int n = 0; n < cnt; n++)
            ASSERT_EQ(tray[n]->getId(), ids[n]);
    }
} // namespace

TEST_F(watcherTest, localVariableBindTagTest) {
    A a;
    const bindTag& tag = a.getBindTag() OR_ASSERT(tag);
    ASSERT_TRUE(tag.getId().serial);

    ASSERT_FALSE(tag.getChunk());
}

TEST_F(watcherTest, NestedClassBindTest) {
    class inner: public instance {
    public:
        const type& getType() const override { return ttype<inner>::get(); }

        clonable* clone() const override { return new inner(); }
    };

    class outer: public instance {
    public:
        outer(inner* in): _inner(in) {}

        const type& getType() const override { return ttype<outer>::get(); }

        clonable* clone() const override { return new outer(_inner); }

        inner* _inner;
    };

    auto* b = new outer(new inner());
    ASSERT_TRUE(b->getId().isHeap());
    ASSERT_TRUE(b->_inner->getId().isHeap());
}

TEST_F(watcherTest, heapVariableBindTagTest) {
    vector<A*> arr;

    for(int n = 0; n < 10; n++) {
        A* new1 = new A();

        const bindTag& tag = new1->getBindTag() OR_ASSERT(tag);
        ASSERT_GT(tag.getId().serial, 0);
        ASSERT_TRUE(tag.isBind());
        ASSERT_EQ(tag.getBindable(), ttype<instance>());
        ASSERT_TRUE(tag.canBind(ttype<instance>()));

        id i = tag->getId();
        ASSERT_NE(i.tagN, NM_INDEX_ERROR);
        ASSERT_NE(i.chkN, NM_INDEX_ERROR);
        ASSERT_GT(i.serial, 0);
        ASSERT_TRUE(i.isHeap());

        const chunk& chk = tag.getChunk() OR_ASSERT(chk);
        ASSERT_TRUE(chk.has(*new1));
        ASSERT_GT(chk.len(), 0);
        ASSERT_GT(chk.size(), 0);
        new1->age = n;
        arr.push_back(new1);
    }

    for(A* e: arr)
        delete e;
    arr.clear();
}

TEST_F(watcherTest, addIntegrityTest100) { addIntegrityTest(100); }

TEST_F(watcherTest, addIntegrityTest10000) { addIntegrityTest(1000); }

TEST_F(watcherTest, addRemoveIntegrityTest100) { addRemoveIntegrityTest(100); }
