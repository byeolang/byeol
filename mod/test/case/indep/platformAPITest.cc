#include "../../common/dep.hpp"

using namespace nm;

TEST(platformAPITest, iterateCodepoints) {
    std::string val1 = "🏁🎌☃";
    cpIter e1(val1);
    ASSERT_EQ(*e1++, "🏁");
    ASSERT_EQ(e1.remainLen(), 2);
    ASSERT_EQ(*e1++, "🎌");
    ASSERT_EQ(*e1++, "☃");
    ASSERT_TRUE(e1.isEnd());
    e1.next(10);
    ASSERT_FALSE(e1);

    cpIter e2("ä");
    ASSERT_EQ(e2.remainLen(), 1);
    ASSERT_EQ(*e2, "ä");
    ASSERT_EQ(*(++e2), std::string(""));

    cpIter e3("அம்மா");
    ASSERT_EQ(e3.remainLen(), 5);
}

TEST(platformAPITest, iterateCodepointsMixedString) {
    cpIter e4("abcd🏁efg");
    e4.next(2); // 'a' -> 'c'
    ASSERT_EQ(*e4++, "c"); // 'c' -> 'd'
    ASSERT_EQ(*e4++, "d"); // 'd' -> '🏁'
    ASSERT_EQ(*e4, "🏁");
    e4.next(2); // '🏁' -> 'f'
    ASSERT_EQ(*e4++, "f"); // 'f' -> 'g'
    ASSERT_EQ(*e4++, "g"); // 'g' -> ""
    ASSERT_EQ(*e4, "");
    e4.next(1);
    ASSERT_EQ(*e4, "");
}
