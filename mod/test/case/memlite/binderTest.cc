#include "../../common/dep.hpp"

using namespace wrd;

namespace {
    struct A : public instance {
        A(): age(0) {}
        A(int newAge): age(newAge) {}

        int age;

        const type& getType() const override {
            return ttype<A>::get();
        }
    };

    struct B : public instance {
        B() { ++get(); }
        B(const B&) { ++get(); }
        ~B() { --get(); }

        float grade;

        static wcnt& get() {
            static wcnt n = 0;
            return n;
        }

        const type& getType() const override {
            return ttype<B>::get();
        }
    };

    void integrity(int cnt) {
        std::vector<tstr<A>> tray;
        std::vector<id> ids;
        for(int n=0; n < cnt; n++) {
            A* new1 = new A(n);
            tray.push_back(tstr<A>(new1));
            ids.push_back(new1->getId());
        }

        const watcher& watcher = instancer::get().getWatcher();
        for(int n=0; n < cnt;n++) {
            id i = ids[n];
            ASSERT_EQ(tray[n]->getId(), i);

            const bindTag& tag = watcher[i].blk;
            ASSERT_EQ(i, tag.getId());
        }
    }

}

TEST(bindTest, defaultBehaviorTest) {
    tstr<A> b1(new A());
    ASSERT_FALSE(nul(*b1));
    ASSERT_FALSE(nul(b1->getType()));

    const bindTag& tag = b1->getBindTag();
    ASSERT_FALSE(nul(tag));
    ASSERT_TRUE(b1->isHeap());

    id i = tag.getId();
    ASSERT_GT(i.serial, 0);
    ASSERT_GE(i.tagN,  0);
}

TEST(bindTest, shouldBindTagInaccessibleAfterInstanceTermination) {
    id i;
    const watcher& watcher = instancer::get().getWatcher();
    const bindTag* tag;
    {
        B b1;
        i = b1.getId();
        tag = &watcher[i].blk;
        ASSERT_FALSE(nul(tag));
        ASSERT_EQ(i, tag->getId());
    }

    ASSERT_TRUE(nul(watcher[i]));
}

TEST(bindTest, supportLocalBindingTest) {
    tstr<B> bind;
    ASSERT_FALSE(bind);

    {
        B b;
        bind.bind(b);
        ASSERT_TRUE(bind);
        ASSERT_EQ(&(*bind), &b);
    }

    ASSERT_FALSE(bind);
    ASSERT_TRUE(nul(*bind));
}

TEST(bindTest, bindSameInstanceFewTimesTest) {
    ASSERT_EQ(B::get(), 0);

    {
        B b1;
        ASSERT_EQ(B::get(), 1);
        ASSERT_EQ(b1.getBindTag().getStrongCnt(), 0);

        tstr<B> bb1(b1),
                bb2(tstr<B>(new B(b1))),
                bb3;
        ASSERT_EQ(B::get(), 2);
        ASSERT_TRUE(bb1.isBind());
        ASSERT_FALSE(nul(bb1->getBindTag()));
        ASSERT_EQ(bb1->getBindTag().getStrongCnt(), 0);

        ASSERT_TRUE(bb2.isBind());
        ASSERT_FALSE(nul(bb2->getBindTag()));
        ASSERT_EQ(bb2->getBindTag().getStrongCnt(), 1);

        ASSERT_FALSE(bb3.isBind());
        bb3 = bb2;
        ASSERT_EQ(B::get(), 2);
        ASSERT_TRUE(bb3.isBind());
        ASSERT_FALSE(nul(bb3->getBindTag()));
        ASSERT_EQ(bb3->getBindTag().getStrongCnt(), 2);
        bb2.rel();
        ASSERT_FALSE(bb2.isBind());
        ASSERT_TRUE(nul(bb2.get()));
        ASSERT_EQ(bb3->getBindTag().getStrongCnt(), 1);
        ASSERT_EQ(B::get(), 2);
        bb3.rel();
        bb1.rel();
    }

    ASSERT_EQ(B::get(), 0);
}

TEST(bindTest, StrongAndWeakTest) {
    tstr<A> strA(new A());
    ASSERT_TRUE(strA.isBind());

    const bindTag& tag = strA->getBindTag();
    ASSERT_FALSE(nul(tag));
    ASSERT_EQ(tag.getStrongCnt(), 1);

    tweak<A> weakA(*strA);
    const bindTag& tagWeak = weakA->getBindTag();
    ASSERT_FALSE(nul(tagWeak));
    ASSERT_EQ(&tagWeak, &tag);
    ASSERT_EQ(tagWeak.getStrongCnt(), 1);
}

TEST(bindTest, bindByValueTest) {
    tstr<A> strA(new A());
    const bindTag& tag = strA->getBindTag();
    ASSERT_FALSE(nul(tag));
    ASSERT_EQ(tag.getStrongCnt(), 1);

    binder bindA(strA);
    ASSERT_EQ(&strA.get(), &bindA.get());
    ASSERT_EQ(tag.getStrongCnt(), 2);

    strA.rel();
    ASSERT_EQ(tag.getStrongCnt(), 1);

    {
        binder bindA2(bindA);
        ASSERT_EQ(tag.getStrongCnt(), 2);
    }

    ASSERT_EQ(tag.getStrongCnt(), 1);
    bindA.rel();

    ASSERT_EQ(tag.getStrongCnt(), 0);
}

TEST(bindTest, assignTest) {
    tstr<A> strA(new A());
    tstr<A> strA1;

    ASSERT_TRUE(strA.getItsId().isValid());
    ASSERT_TRUE(strA.getItsId().isValid());
    ASSERT_FALSE(strA1.getItsId().isValid());
    strA = strA1;
    ASSERT_FALSE(strA.getItsId().isValid());
}

TEST(bindTest, bindMultiplTimesIntegrityTest) {
    integrity(10);
    integrity(100);
    integrity(1000);
}

TEST(bindTest, WeakBindButInstanceGoneTest) {
    tstr<A> strA(new A());
    tweak<A> weakA(*strA);

    const bindTag& tag = weakA->getBindTag();
    ASSERT_FALSE(nul(tag));
    ASSERT_EQ(tag.getStrongCnt(), 1);

    strA.rel();
    ASSERT_FALSE(weakA.isBind());
    ASSERT_TRUE(nul(*weakA));
}

TEST(bindTest, bindNullShouldUnbindPrevious) {
    tstr<A> strA(new A(18));
    ASSERT_TRUE(strA);
    ASSERT_EQ(strA->age, 18);

    strA.bind(new A(22));
    ASSERT_TRUE(strA);
    ASSERT_EQ(strA->age, 22);

    strA.bind(nullptr);
    ASSERT_FALSE(strA.isBind());
}

// static variable could exists longer than watcher. in this situtation, trying to get
// watchcell info would be failed by returning null reference.
// we will verify that this memlite module can handle the situation properly.
TEST(bindTest, bindStaticVariable) {
    class myInstance : public instance {
        WRD(ME(myInstance, instance))

    public:
        const type& getType() const override {
            return ttype<myInstance>::get();
        };
    };

    // this static object of an instance will trigger release of watchcell on its
    // destructor.
    static myInstance variable;
    tstr<myInstance> binder;
    binder.bind(variable); // this statement makes watchell of 'variable'.
}

TEST(binderTest, testTacticIsImmutable) {
    tstr<A> strA; // has strTactic
    ASSERT_EQ(strA._tactic, &strTactic::singletone);
    tweak<A> weakA;
    ASSERT_EQ(weakA._tactic, &weakTactic::singletone);
    tstr<A> strA1(weakA);
    ASSERT_EQ(strA1._tactic, &strTactic::singletone);
    strA1.bind(*weakA);
    ASSERT_EQ(strA._tactic, &strTactic::singletone);
    strA1 = weakA;
    ASSERT_EQ(strA._tactic, &strTactic::singletone);
}
