#include "test/byeolTest.hpp"

using namespace by;

TEST(instanceTest, idDefaultCopyCtor) {
    id id1(-1, -1, 1999999);
    id id2 = id1;

    ASSERT_EQ(id1, id2);
}

TEST(instanceTest, idDefaultCopyCtor2) {
    id id1(1048575, -1, 0);
    id1.serial--;
    id1.tagN++;
    id id2 = id1;

    ASSERT_EQ(id1, id2);
}
