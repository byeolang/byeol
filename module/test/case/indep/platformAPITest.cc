#include "test/common/dep.hpp"

using namespace by;

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

    std::string val2 = "ä";
    cpIter e2(val2);
    ASSERT_EQ(e2.remainLen(), 1);
    ASSERT_EQ(*e2, "ä");
    ASSERT_EQ(*(++e2), std::string(""));

    std::string val3 = "அம்மா";
    cpIter e3(val3);
    ASSERT_EQ(e3.remainLen(), 5);
}

TEST(platformAPITest, iterateCodepointsMixedString) {
    std::string src = "abcd🏁efg";
    cpIter e4(src);
    e4.next(2);            // 'a' -> 'c'
    ASSERT_EQ(*e4++, "c"); // 'c' -> 'd'
    ASSERT_EQ(*e4++, "d"); // 'd' -> '🏁'
    ASSERT_EQ(*e4, "🏁");
    e4.next(2);            // '🏁' -> 'f'
    ASSERT_EQ(*e4++, "f"); // 'f' -> 'g'
    ASSERT_EQ(*e4++, "g"); // 'g' -> ""
    ASSERT_EQ(*e4, "");
    e4.next(1);
    ASSERT_EQ(*e4, "");
}

TEST(platformAPITest, reverseCodepointIterator) {
    std::string src = "abcd🏁efg";
    cpIter e4(src.c_str() + src.size(), src.c_str()); // reversed.
    e4.next(2);                                       // <end> -> 'f'
    ASSERT_EQ(*e4++, "f");                            // 'f' -> 'e'
    ASSERT_EQ(*e4++, "e");                            // 'e' -> '🏁'
    ASSERT_EQ(*e4, "🏁");
    e4.next(2);            // '🏁' -> 'c'
    ASSERT_EQ(*e4++, "c"); // 'c' -> 'b'
    ASSERT_EQ(*e4++, "b"); // 'b' -> 'a'
    ASSERT_EQ(*e4++, "a"); // 'a' -> ""
    ASSERT_EQ(*e4, "");
    e4.next(1);
    ASSERT_EQ(*e4, "");
}

TEST(platformAPITest, reverseCodepointIterator2) {
    std::string src = "abcd🏁efg";
    cpIter e4(src, true);  // reversed.
    e4.next(2);            // <end> -> 'f'
    ASSERT_EQ(*e4++, "f"); // 'f' -> 'e'
    ASSERT_EQ(*e4++, "e"); // 'e' -> '🏁'
    ASSERT_EQ(*e4, "🏁");
    e4.next(2);            // '🏁' -> 'c'
    ASSERT_EQ(*e4++, "c"); // 'c' -> 'b'
    ASSERT_EQ(*e4++, "b"); // 'b' -> 'a'
    ASSERT_EQ(*e4++, "a"); // 'a' -> ""
    ASSERT_EQ(*e4, "");
    e4.next(1);
    ASSERT_EQ(*e4, "");
}

TEST(platformAPITest, reverseCPIteratorToBackward) {
    std::string src = "abcd🏁efg";
    cpIter e4(src, true); // reversed.
    while(*e4 != "")
        ++e4;
    ASSERT_EQ(*e4, "");

    std::string expects[] = {"g", "f", "e", "🏁", "d", "c", "b", "a"};
    for(int n = 0; n < 8; n++) {
        e4.stepBackward(1);
        ASSERT_EQ(*e4, expects[n]);
    }
}
