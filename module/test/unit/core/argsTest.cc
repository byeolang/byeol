#include "test/byeolTest.hpp"

using namespace by;
using namespace std;

struct argsTest: public byeolTest {};

TEST_F(argsTest, testDefaultConstructor) {
    args a;

    ASSERT_EQ(a.len(), 0);
    ASSERT_EQ(a.getMe(), nullptr);
}

TEST_F(argsTest, testConstructorWithNarr) {
    narr arr;
    arr.add(new nInt(1));
    arr.add(new nInt(2));
    arr.add(new nInt(3));

    args a(arr);

    ASSERT_EQ(a.len(), 3);
    ASSERT_EQ(a[0].cast<nInt>()->get(), 1);
    ASSERT_EQ(a[1].cast<nInt>()->get(), 2);
    ASSERT_EQ(a[2].cast<nInt>()->get(), 3);
}

TEST_F(argsTest, testConstructorWithMeObject) {
    nInt meObj(42);
    args a(&meObj);

    ASSERT_EQ(a.len(), 0);
    ASSERT_TRUE(a.getMe() != nullptr);
    ASSERT_EQ(a.getMe(), &meObj);
}

TEST_F(argsTest, testConstructorWithMeAndNarr) {
    nStr meObj("context");
    narr arr;
    arr.add(new nInt(10));
    arr.add(new nInt(20));

    args a(&meObj, arr);

    ASSERT_EQ(a.len(), 2);
    ASSERT_EQ(a.getMe(), &meObj);
    ASSERT_EQ(a[0].cast<nInt>()->get(), 10);
    ASSERT_EQ(a[1].cast<nInt>()->get(), 20);
}

TEST_F(argsTest, testCopyConstructor) {
    nInt meObj(99);
    narr arr;
    arr.add(new nInt(5));

    args a1(&meObj, arr);
    args a2(a1);

    ASSERT_EQ(a2.len(), a1.len());
    ASSERT_EQ(a2.getMe(), a1.getMe());
    ASSERT_EQ(a2[0].cast<nInt>()->get(), 5);
}

TEST_F(argsTest, testSetMeWithNode) {
    args a;
    nInt context(123);

    ASSERT_EQ(a.getMe(), nullptr);

    a.setMe(context);
    ASSERT_TRUE(a.getMe() != nullptr);
    ASSERT_EQ(a.getMe(), &context);
}

TEST_F(argsTest, testSetMeWithPointer) {
    args a;
    nInt context(456);

    a.setMe(&context);
    ASSERT_TRUE(a.getMe() != nullptr);
    ASSERT_EQ(a.getMe(), &context);
}

TEST_F(argsTest, testSetMeReturnsThis) {
    args a;
    nInt context(789);

    const args& result = a.setMe(context);
    ASSERT_EQ(&result, &a);
}

TEST_F(argsTest, testSetMeOnConstArgs) {
    narr arr;
    arr.add(new nInt(1));
    const args a(arr);

    nInt context(100);
    a.setMe(context);

    ASSERT_EQ(a.getMe(), &context);
}

TEST_F(argsTest, testAddArgumentsAfterConstruction) {
    args a;

    a.add(new nInt(1));
    a.add(new nInt(2));
    a.add(new nInt(3));

    ASSERT_EQ(a.len(), 3);
    ASSERT_EQ(a[0].cast<nInt>()->get(), 1);
    ASSERT_EQ(a[1].cast<nInt>()->get(), 2);
    ASSERT_EQ(a[2].cast<nInt>()->get(), 3);
}

TEST_F(argsTest, testMixedArgumentTypes) {
    args a;

    a.add(new nInt(42));
    a.add(new nStr("hello"));
    a.add(new nFlt(3.14f));
    a.add(new nBool(true));

    ASSERT_EQ(a.len(), 4);
    ASSERT_EQ(a[0].cast<nInt>()->get(), 42);
    ASSERT_EQ(a[1].cast<nStr>()->get(), "hello");
    ASSERT_FLOAT_EQ(a[2].cast<nFlt>()->get(), 3.14f);
    ASSERT_EQ(a[3].cast<nBool>()->get(), true);
}

TEST_F(argsTest, testSetMeMultipleTimes) {
    args a;
    nInt context1(10);
    nInt context2(20);
    nInt context3(30);

    a.setMe(context1);
    ASSERT_EQ(a.getMe(), &context1);

    a.setMe(context2);
    ASSERT_EQ(a.getMe(), &context2);

    a.setMe(context3);
    ASSERT_EQ(a.getMe(), &context3);
}

TEST_F(argsTest, testEmptyArgsWithContext) {
    nVoid context;
    args a(&context);

    ASSERT_EQ(a.len(), 0);
    ASSERT_TRUE(a.getMe() != nullptr);
    ASSERT_EQ(a.getMe(), &context);
}
