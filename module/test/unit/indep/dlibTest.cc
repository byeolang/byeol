#include "test/common/dep.hpp"

using namespace by;

TEST(dlibTest, defaultConstructor) {
    dlib lib;
    ASSERT_STREQ(lib.getPath().c_str(), "");
    ASSERT_FALSE(lib.isLoaded());
}

TEST(dlibTest, constructorWithCString) {
    dlib lib("/path/to/lib.so");
    ASSERT_STREQ(lib.getPath().c_str(), "/path/to/lib.so");
    ASSERT_FALSE(lib.isLoaded());
}

TEST(dlibTest, constructorWithString) {
    std::string path = "/path/to/lib.dylib";
    dlib lib(path);
    ASSERT_STREQ(lib.getPath().c_str(), "/path/to/lib.dylib");
    ASSERT_FALSE(lib.isLoaded());
}

TEST(dlibTest, copyConstructor) {
    dlib lib1("/original/path.so");
    dlib lib2(lib1);

    ASSERT_STREQ(lib2.getPath().c_str(), "/original/path.so");
    ASSERT_STREQ(lib1.getPath().c_str(), "/original/path.so");
}

TEST(dlibTest, assignmentOperator) {
    dlib lib1("/path1.so");
    dlib lib2("/path2.so");

    lib2 = lib1;
    ASSERT_STREQ(lib2.getPath().c_str(), "/path1.so");
}

TEST(dlibTest, selfAssignment) {
    dlib lib("/test.so");
    dlib lib2;
    lib2 = lib;
    ASSERT_STREQ(lib2.getPath().c_str(), "/test.so");
}

TEST(dlibTest, setPath) {
    dlib lib;
    lib.setPath("/new/path.so");
    ASSERT_STREQ(lib.getPath().c_str(), "/new/path.so");
}

TEST(dlibTest, setPathWithPointer) {
    dlib lib;
    std::string path = "/pointer/path.so";
    lib.setPath(&path);
    ASSERT_STREQ(lib.getPath().c_str(), "/pointer/path.so");
}

TEST(dlibTest, loadNonExistentLibrary) {
    dlib lib("/nonexistent/library.so");
    auto result = lib.load();

    // Should have error
    ASSERT_TRUE(result.has());
    ASSERT_FALSE(lib.isLoaded());
}

TEST(dlibTest, accessFuncWithoutLoading) {
    dlib lib("/some/path.so");
    auto result = lib.accessFunc<void*>("someFunction");

    // Should return error because library is not loaded
    ASSERT_FALSE(result.has());
    ASSERT_STRNE(result.getErr().c_str(), "");
}

TEST(dlibTest, accessFuncWithStringPointer) {
    dlib lib;
    std::string funcName = "testFunc";
    auto result = lib.accessFunc<void*>(&funcName);

    // Should fail because library not loaded
    ASSERT_FALSE(result.has());
}

TEST(dlibTest, relOnNonLoadedLibrary) {
    dlib lib;
    // Should not crash
    lib.rel();
    ASSERT_FALSE(lib.isLoaded());
}

TEST(dlibTest, multipleCopiesShareHandle) {
    dlib lib1("/test.so");
    dlib lib2 = lib1;

    // After copy, original should have null handle (move semantics)
    ASSERT_FALSE(lib1.isLoaded());
    ASSERT_FALSE(lib2.isLoaded());
}
