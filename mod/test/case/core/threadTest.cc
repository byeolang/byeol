#include "../../namuTest.hpp"

using namespace nm;

struct threadTest : public namuTest {
    thread* thr = nullptr;

    void SetUp() {
        thr = new thread();
        thread::set(thr);
    }

    void TearDown() {
        thread::set(nullptr);
        if(thr)
            delete thr;
        thr = nullptr;
    }

    nbool didExceptionOccurs() const {
        return getExceptions().hasErr();
    }

    const errReport& getExceptions() const {
        return thread::get().getEx();
    }
};

TEST_F(threadTest, testExceptionOccurs) {
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
    const err& e = ex[0];
    ASSERT_FALSE(nul(ex));
    ASSERT_EQ(e.code, errCode::OUT_OF_RANGE);

    e.dump();
}
