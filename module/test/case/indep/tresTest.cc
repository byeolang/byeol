#include "test/common/dep.hpp"

using namespace by;

TEST(tresTest, defaultConstructor) {
    tres<int, std::string> result;
    ASSERT_FALSE(result.has());
}

TEST(tresTest, constructorWithValue) {
    tres<int, std::string> result(42);
    ASSERT_TRUE(result.has());
    ASSERT_EQ(*result, 42);
}

TEST(tresTest, constructorWithError) {
    tres<int, std::string> result(std::string("error message"));
    ASSERT_FALSE(result.has());
    ASSERT_STREQ(result.getErr().c_str(), "error message");
}

TEST(tresTest, getErr) {
    tres<int, std::string> result(std::string("test error"));
    ASSERT_FALSE(result.has());
    ASSERT_STREQ(result.getErr().c_str(), "test error");

    // Test non-const version
    result.getErr() = "modified error";
    ASSERT_STREQ(result.getErr().c_str(), "modified error");
}

TEST(tresTest, setErr) {
    tres<int, std::string> result(42);
    ASSERT_TRUE(result.has());

    result.setErr("new error");
    ASSERT_FALSE(result.has());
    ASSERT_STREQ(result.getErr().c_str(), "new error");
}

TEST(tresTest, setValue) {
    tres<int, std::string> result(std::string("error"));
    ASSERT_FALSE(result.has());

    result.set(100);
    ASSERT_TRUE(result.has());
    ASSERT_EQ(*result, 100);
}

TEST(tresTest, rel) {
    tres<int, std::string> result(42);
    ASSERT_TRUE(result.has());

    result.rel();
    ASSERT_FALSE(result.has());
}

TEST(tresTest, relClearsError) {
    tres<int, std::string> result(std::string("error"));
    ASSERT_FALSE(result.has());

    result.rel();
    ASSERT_FALSE(result.has());
}

TEST(tresTest, setClearsError) {
    tres<int, std::string> result(std::string("error"));
    result.set(50);
    ASSERT_TRUE(result.has());
    ASSERT_EQ(*result, 50);
}

TEST(tresTest, setErrClearsValue) {
    tres<int, std::string> result(99);
    result.setErr("cleared value");
    ASSERT_FALSE(result.has());
    ASSERT_STREQ(result.getErr().c_str(), "cleared value");
}

TEST(tresTest, withDifferentErrorType) {
    tres<std::string, int> result(42);
    ASSERT_FALSE(result.has());
    ASSERT_EQ(result.getErr(), 42);

    result.set("success");
    ASSERT_TRUE(result.has());
    ASSERT_STREQ(result.get()->c_str(), "success");
}

TEST(tresTest, complexType) {
    struct CustomError {
        int code;
        std::string msg;
        bool operator==(const CustomError& rhs) const {
            return code == rhs.code && msg == rhs.msg;
        }
    };

    tres<int, CustomError> result(CustomError{404, "not found"});
    ASSERT_FALSE(result.has());
    ASSERT_EQ(result.getErr().code, 404);
    ASSERT_STREQ(result.getErr().msg.c_str(), "not found");
}
