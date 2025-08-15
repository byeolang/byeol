#include "test/byeolTest.hpp"

using namespace by;
using namespace std;

struct nseqTest: public byeolTest {};

TEST_F(nseqTest, simpleIterator) {
    nseq s(nInt(1), nInt(4));
    auto e = s.begin();

    int expects[] = {1, 2, 3};
    for(int n = 0; n < 3; n++, ++e)
        ASSERT_EQ((*e).get(), expects[n]);

    ASSERT_TRUE(e.isEnd());
}

TEST_F(nseqTest, simpleReversedIterator) {
    nseq s(nInt(1), nInt(4));
    auto re = s.rbegin();
    ASSERT_TRUE(re.isReversed());

    int expects[] = {3, 2, 1};
    for(int n = 0; n < 3; n++, ++re)
        ASSERT_EQ((*re).get(), expects[n]);
}

TEST_F(nseqTest, stepForwardReversedIterator) {
    nseq s(nInt(1), nInt(4));
    auto re = s.rbegin();

    ASSERT_TRUE(re.isReversed());
    ASSERT_EQ((*re).get(), 3);

    ++re; // 2
    ++re; // 1
    ASSERT_EQ((*re).get(), 1);

    re.stepForward(2); // 3
    ASSERT_EQ((*re).get(), 3);

    re.stepBackward(1); // 2
    ASSERT_EQ((*re).get(), 2);

    ASSERT_EQ(re.next(6), 1);
    ASSERT_TRUE(re.isEnd());
    ASSERT_EQ(re, s.rend());
}

struct dummyUcontainer : public tnucontainer<nInt, nInt, nInt> {
    typedef tnucontainer<nInt, nInt, nInt> _super11;
    BY(CLASS(dummyUcontainer, _super11))

public:
    ncnt len() const override { return 0; }
    nbool set(const iter& at, const nInt& new1) override { return true; }
    nbool add(const iter& at, const nInt& new1) override { return true; }
    void add(const iter& here, const iter& from, const iter& to) override {}
    nbool del(const iter& it) override { return true; }
    nbool del(const iter& from, const iter& end) override { return true; }
    void rel() override {};
    iteration* _onMakeIteration(ncnt step, nbool isReversed) const override { return nullptr; }
};

TEST_F(nseqTest, ucontainableTest) {
    dummyUcontainer c;
    // why is there no assert?:
    //  this is actually code to prevent lazy template instantiation in gcc/clang.
    //  as long as it compiles properly, the goal is already achieved.
    c.get([&](const nInt& it) { return it.get() > 0; });

    c.getAll([&](const nInt& it) { return it.get() > 0; });

    int sum = 0;
    c.each([&](const nInt& it) {
        sum += it.get();
        return true;
    });
    ASSERT_EQ(sum, 0);

    c.in([&](const nInt& it) {
        return it.get() > 0;
    });

    nInt n1(1);
    c.iterate(n1);
    c.riterate(n1);
}
