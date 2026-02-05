#include "test/common/dep.hpp"

using namespace by;

TEST(platformAPITest, createNowTime) {
    std::string time = platformAPI::createNowTime("%Y-%m-%d");
    // Should return non-empty formatted time
    ASSERT_FALSE(time.empty());
    // Should contain year digits
    ASSERT_TRUE(time.find("20") != std::string::npos || time.find("19") != std::string::npos);
}

TEST(platformAPITest, demangle) {
    // Test with mangled name
    std::string demangled = platformAPI::demangle("_ZN2by6tnarrI5paramE4initEv");
    ASSERT_FALSE(demangled.empty());

    // Test with unmangled name
    std::string plain = platformAPI::demangle("plainFunction");
    ASSERT_STREQ(plain.c_str(), "plainFunction");
}

TEST(platformAPITest, filterDemangle) {
    std::string filtered = platformAPI::filterDemangle("_ZN2by6tnarrI5paramE4initEv");
    ASSERT_FALSE(filtered.empty());
}

TEST(platformAPITest, toAddrId) {
    int dummy = 42;
    void* ptr = &dummy;
    std::string addrId = platformAPI::toAddrId(ptr);

    // Should return 4-character hex string
    ASSERT_EQ(addrId.length(), 4);

    // All characters should be valid hex digits
    for(char c: addrId)
        ASSERT_TRUE((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

TEST(platformAPITest, format) {
    std::string result = platformAPI::format("Hello %s, number %d", "world", 42);
    ASSERT_STREQ(result.c_str(), "Hello world, number 42");
}

TEST(platformAPITest, formatWithNumbers) {
    std::string result = platformAPI::format("%d + %d = %d", 1, 2, 3);
    ASSERT_STREQ(result.c_str(), "1 + 2 = 3");
}

TEST(platformAPITest, formatWithFloat) {
    std::string result = platformAPI::format("Pi is %.2f", 3.14159);
    ASSERT_STREQ(result.c_str(), "Pi is 3.14");
}

TEST(platformAPITest, getExecPath) {
    nbool notWindows = buildFeature::platform::get() != by::buildFeature::WINDOW;
    std::string path = platformAPI::getExecPath();
    // Path may be empty on Windows, but should be valid on POSIX
    ASSERT_TRUE(notWindows && path.size() > 0);
}

TEST(platformAPITest, getNowMs) {
    nuint64 now1 = platformAPI::getNowMs();
    ASSERT_GT(now1, 0);

    // Wait a tiny bit and check it increases
    for(volatile int i = 0; i < 10000; i++)
        ;
    nuint64 now2 = platformAPI::getNowMs();
    ASSERT_GE(now2, now1);
}

TEST(platformAPITest, logString) {
    // Should not crash
    platformAPI::log("test message");
    platformAPI::log(std::string("another test"));
}

TEST(platformAPITest, logStringPointer) {
    std::string msg = "pointer test";
    platformAPI::log(&msg);
}

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
