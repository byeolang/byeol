#include "test/namuTest.hpp"

using namespace nm;
using namespace std;

struct nseqTest: public namuTest {};

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
