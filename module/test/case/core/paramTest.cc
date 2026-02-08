#include "test/byeolTest.hpp"

using namespace by;
using namespace std;

struct paramTest: public byeolTest {};

TEST_F(paramTest, testConstructorWithNameAndNodeReference) {
    nInt origin(42);
    param p("testParam", origin);

    ASSERT_EQ(p.getName(), "testParam");
    ASSERT_EQ(&p.getOrigin(), &origin);
}

TEST_F(paramTest, testConstructorWithNameAndNodePointer) {
    nInt* origin = new nInt(100);
    param p("myParam", origin);

    ASSERT_EQ(p.getName(), "myParam");
    ASSERT_EQ(&p.getOrigin(), origin);

    delete origin;
}

TEST_F(paramTest, testSetAndGetName) {
    nVoid origin;
    param p("initialName", origin);

    ASSERT_EQ(p.getName(), "initialName");

    p.setName("newName");
    ASSERT_EQ(p.getName(), "newName");

    p.setName("anotherName");
    ASSERT_EQ(p.getName(), "anotherName");
}

TEST_F(paramTest, testSetOrigin) {
    nInt int1(1);
    nInt int2(2);
    param p("param", int1);

    ASSERT_EQ(&p.getOrigin(), &int1);

    p.setOrigin(int2);
    ASSERT_EQ(&p.getOrigin(), &int2);
}

TEST_F(paramTest, testEqualityOperatorWithSameType) {
    nInt origin1(10);
    nInt origin2(20);
    param p1("param1", origin1);
    param p2("param2", origin2);

    // params with same type should be equal regardless of different names
    ASSERT_TRUE(p1 == p2);
    ASSERT_FALSE(p1 != p2);
}

TEST_F(paramTest, testEqualityOperatorWithDifferentTypes) {
    nInt intOrigin(42);
    nStr strOrigin("test");
    param p1("param1", intOrigin);
    param p2("param2", strOrigin);

    // params with different types should not be equal
    ASSERT_FALSE(p1 == p2);
    ASSERT_TRUE(p1 != p2);
}

TEST_F(paramTest, testCloneDeep) {
    nInt origin(99);
    param p1("originalParam", origin);

    param* p2 = (param*) p1.cloneDeep();
    ASSERT_TRUE(p2 != nullptr);
    ASSERT_EQ(p1.getName(), p2->getName());

    // After deep clone, origins should be different instances
    ASSERT_NE(&p1.getOrigin(), &p2->getOrigin());

    // But should have the same type
    ASSERT_EQ(p1.getOrigin().getType(), p2->getOrigin().getType());

    delete p2;
}

TEST_F(paramTest, testSetNameWithDifferentOverloads) {
    nInt origin(1);
    param p("initial", origin);

    // Test setName with std::string&
    string newName = "stringName";
    p.setName(newName);
    ASSERT_EQ(p.getName(), "stringName");

    // Test setName with const char*
    p.setName("charPtrName");
    ASSERT_EQ(p.getName(), "charPtrName");
}

TEST_F(paramTest, testMultipleParamsWithSameOrigin) {
    nInt sharedOrigin(50);
    param p1("first", sharedOrigin);
    param p2("second", sharedOrigin);
    param p3("third", sharedOrigin);

    ASSERT_EQ(&p1.getOrigin(), &sharedOrigin);
    ASSERT_EQ(&p2.getOrigin(), &sharedOrigin);
    ASSERT_EQ(&p3.getOrigin(), &sharedOrigin);

    ASSERT_TRUE(p1 == p2);
    ASSERT_TRUE(p2 == p3);
    ASSERT_TRUE(p1 == p3);
}
