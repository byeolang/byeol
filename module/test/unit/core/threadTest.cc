#include "test/byeolTest.hpp"

using namespace by;

struct threadTest: public byeolTest {
    thread* thr = nullptr;

    void SetUp() override {
        thr = new thread();
        thread::set(thr);
    }

    void TearDown() override {
        thread::set(nullptr);
        if(thr) delete thr;
        thr = nullptr;
    }

    nbool didExceptionOccurs() const { return getExceptions().inErr(); }

    const errReport& getExceptions() const { return thread::get().getEx(); }
};

TEST_F(threadTest, testExceptionOccurs) {
    errReport rpt(false);
    threadUse thr(rpt);
    narr a;
    a.add(new mockNode());
    a.add(new mockNode());
    ASSERT_FALSE(didExceptionOccurs());

    a[1];
    ASSERT_FALSE(didExceptionOccurs());

    a[2];
    ASSERT_TRUE(didExceptionOccurs());

    const auto& ex = getExceptions();
    ASSERT_TRUE(ex.len() > 0);
    ex[0].dump();
}
