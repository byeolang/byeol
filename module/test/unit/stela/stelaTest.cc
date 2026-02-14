#include "test/byeolTest.hpp"

using namespace std;
using namespace by;

struct stelaTest: public byeolTest {};

TEST_F(stelaTest, checkDefaultNulStela) {
    const stela& nul = nulStela::get();
    ASSERT_TRUE(&nul);
    ASSERT_EQ(nul.asChar(), '\0');
    ASSERT_EQ(nul.asInt(), 0);
    ASSERT_EQ(nul.asBool(), false);
    ASSERT_EQ(nul.asStr(), "");
    ASSERT_TRUE(&nul.getType());
}

TEST_F(stelaTest, checkVerStela) {
    verStela s(1, 2, 3);
    ASSERT_EQ(s.asMajor(), 1);
    ASSERT_EQ(s.asMinor(), 2);
    ASSERT_EQ(s.asFix(), 3);

    verStela s2("1.2.3");
    ASSERT_EQ(s2.asMajor(), 1);
    ASSERT_EQ(s2.asMinor(), 2);
    ASSERT_EQ(s2.asFix(), 3);

    ASSERT_TRUE(s == s2);

    verStela s3(2, 0, 0);
    ASSERT_TRUE(s < s3);
}
