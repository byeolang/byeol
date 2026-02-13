#include "test/byeolTest.hpp"

using namespace by;
using namespace std;

struct visitorTest: public byeolTest {};

TEST_F(visitorTest, iterateManuallyConstructedNodes) {
    obj o;
    o.subs().add("foo1", new func(*new modifier(), typeMaker::make<func>(params(), new nInt())));
    o.subs().add("val1", new nInt());

    obj o2;
    o2.subs().add("foo2", new func(*new modifier(), typeMaker::make<func>(params(), new nFlt())));
    o2.subs().add("val2", new nFlt());
    o2.subs().add("o", o);

    struct myVisitor: public visitor {
        nbool metFoo1;
        nbool metVal1;
        nbool metFoo2;
        nbool metVal2;

        myVisitor(): metFoo1(false), metVal1(false), metFoo2(false), metVal2(false) {}

        nbool onVisit(const visitInfo& i, func& fun, nbool) override {
            if(i.name == "foo1") metFoo1 = true;
            if(i.name == "foo2") {
                metFoo2 = true;
                EXPECT_TRUE(fun.getRet()->isSub<nFlt>());
            }
            return true;
        }

        nbool onVisit(const visitInfo& i, nInt& o, nbool) override {
            if(i.name == "val1") metVal1 = true;
            return true;
        }

        nbool onVisit(const visitInfo& i, nFlt& o, nbool) override {
            if(i.name == "val2") metVal2 = true;
            return true;
        }
    };

    myVisitor v;
    ASSERT_FALSE(v.metFoo1);
    ASSERT_FALSE(v.metFoo2);
    ASSERT_FALSE(v.metVal1);
    ASSERT_FALSE(v.metVal2);

    v.setTask(o2).work();

    ASSERT_TRUE(v.metFoo1);
    ASSERT_TRUE(v.metFoo2);
    ASSERT_TRUE(v.metVal1);
    ASSERT_TRUE(v.metVal2);
}

