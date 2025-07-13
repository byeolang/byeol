#include "test/namuTest.hpp"

using namespace nm;

namespace {
    struct myNode: public node {
        NM(CLASS(myNode, node))

    public:
        myNode(int n): num(n) {}

        scope& subs() override { return dumScope::singleton(); }

        priorType prioritize(const args& a) const override { return NO_MATCH; }

        str run(const args& a) override { return str(); }

        int num;
    };
}

struct frameTest: public namuTest {
    frames& getFrames() { return (frames&) thread::get().getFrames(); }

    void SetUp() override {
        namuTest::SetUp();
        getFrames().add(new frame());
    }

    void TearDown() override {
        getFrames().rel();
        namuTest::TearDown();
    }

    scope& getScopeStack(frame& fr) { return fr.subs(); }
};

TEST_F(frameTest, testAccessFrame) { getFrames().add(new frame); }

TEST_F(frameTest, testFrameManipulateChainObjNegative) {
    frame& fr = getFrames()[getFrames().len() - 1];
    ASSERT_FALSE(getScopeStack(fr).isSub<dumScope>());

    scope local;
    local.add("myNode1", new myNode(1));
    local.add("myNode2", new myNode(2));
    fr.add(scope::wrap<scope>(local));
    ASSERT_FALSE(getScopeStack(fr).isSub<dumScope>());

    scope shares;
    shares.add("myNode4", new myNode(4));
    scope owns;
    owns.add("myNode3", new myNode(3));
    ASSERT_EQ(owns.len(), 1);
    auto lambda = [](const std::string& name, const myNode& elem) {
        return true;
    };
    ASSERT_TRUE(owns.get<myNode>(lambda));

    origin obj1(typeMaker::make<obj>(""));
    obj1.subs().add(owns);
    obj1.subs().link(shares);
    ASSERT_EQ(obj1.subs().len(), 2);

    ASSERT_EQ(fr.subAll<myNode>(lambda).len(), 2);
    ASSERT_EQ(obj1.subs().getAll<myNode>(lambda).len(), 2);
    ASSERT_EQ(obj1.subs().getContainer().getAll<myNode>(lambda).len(), 1);
}
