#include <vector>

#include "../../common/dep.hpp"

using namespace nm;
using namespace std;

namespace {
    void addIntegrityTest(chunk& chk, int cnt) {
        for(int n = 0; n < cnt; n++) {
            int* val = (int*) chk.new1();
            *val = n;
        }

        for(int n = 0; n < cnt; n++) {
            int* actual = (int*) chk[n];
            EXPECT_EQ(*actual, n);
        }
    }
}

TEST(chunkTest, memoryHaverTest) {
    chunk chk(4);

    EXPECT_EQ(chk.len(), 0);
    EXPECT_EQ(chk.size(), chunk::MIN_SZ);
    EXPECT_EQ(chk[0], chk.get(0));
    EXPECT_TRUE(chk[1]);
    EXPECT_FALSE(chk[-1]);
    EXPECT_FALSE(chk.isFull());
    EXPECT_TRUE(chk.isCapable());
    EXPECT_FALSE(chk.has(nulOf<instance>()));
}

TEST(chunkTest, allocatorTest) {
    chunk chk(4);

    EXPECT_EQ(chk.getBlkSize(), 4);
    EXPECT_TRUE(chk.rel());
    EXPECT_EQ(chk.len(), 0);
    EXPECT_EQ(chk.size(), 0);
}

TEST(chunkTest, chunkListTest) {
    class heap {
    public:
        nbool new1(chunk& chk, ncnt cnt) {
            for(int n = 0; n < cnt; n++) [[maybe_unused]]
                void* ptr = chk.new1() OR_RET false;
            return true;
        }
    } heap;

    chunk chk(4, 1112);

    EXPECT_TRUE(heap.new1(chk, 1));
    EXPECT_EQ(chk.len(), 1);
    EXPECT_EQ(chk.size(), 1112);
    EXPECT_TRUE(chk[0]);
    EXPECT_FALSE(chk[-1]);

    EXPECT_TRUE(heap.new1(chk, 1));
    EXPECT_EQ(chk.len(), 2);
    EXPECT_GE(chk.size(), chk.len());
    EXPECT_TRUE(chk[1]);
    EXPECT_TRUE(chk.has(*(instance*) chk[1]));
    EXPECT_EQ(chk.getBlkSize(), 4);

    EXPECT_EQ(chk.len(), 2);
    EXPECT_EQ(chk.size(), 1112);
    EXPECT_TRUE(chk[0]);
    EXPECT_EQ(chk.getBlkSize(), 4);

    EXPECT_TRUE(heap.new1(chk, 10));
    EXPECT_EQ(chk.len(), 12);

    EXPECT_TRUE(heap.new1(chk, 100));
    EXPECT_EQ(chk.len(), 112);
    EXPECT_EQ(chk.size(), 1112);
    EXPECT_TRUE(chk[0]);
    EXPECT_TRUE(chk[100]);

    EXPECT_TRUE(heap.new1(chk, 1000));
    EXPECT_EQ(chk.len(), 1112);

    EXPECT_FALSE(heap.new1(chk, 1));
    EXPECT_NE(chk.len(), 1113);

    EXPECT_TRUE(chk.rel());
    EXPECT_EQ(chk.size(), 0);
    EXPECT_EQ(chk.getBlkSize(), 4);

    EXPECT_EQ(chk.len(), 0);
    EXPECT_FALSE(chk[4]);
    EXPECT_FALSE(chk[0]);
    EXPECT_TRUE(chk.isFull());
    EXPECT_FALSE(chk.isCapable());
}

TEST(chunkTest, addIntegrityTest100) {
    chunk chk(4, 100);
    addIntegrityTest(chk, 100);
}

TEST(chunkTest, addIntegrityTest10000) {
    chunk chk(4, 10000);
    addIntegrityTest(chk, 10000);
}
