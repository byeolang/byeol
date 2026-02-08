#include "test/byeolTest.hpp"

using namespace by;
using namespace std;

struct modifierTest: public byeolTest {};

TEST_F(modifierTest, testDefaultConstructor) {
    modifier m;

    ASSERT_TRUE(m.isPublic());
    ASSERT_FALSE(m.isExplicitOverride());
}

TEST_F(modifierTest, testConstructorWithPublicTrue) {
    modifier m(true, false);

    ASSERT_TRUE(m.isPublic());
    ASSERT_FALSE(m.isExplicitOverride());
}

TEST_F(modifierTest, testConstructorWithPublicFalse) {
    modifier m(false, false);

    ASSERT_FALSE(m.isPublic());
    ASSERT_FALSE(m.isExplicitOverride());
}

TEST_F(modifierTest, testConstructorWithOverrideTrue) {
    modifier m(true, true);

    ASSERT_TRUE(m.isPublic());
    ASSERT_TRUE(m.isExplicitOverride());
}

TEST_F(modifierTest, testConstructorWithBothTrue) {
    modifier m(true, true);

    ASSERT_TRUE(m.isPublic());
    ASSERT_TRUE(m.isExplicitOverride());
}

TEST_F(modifierTest, testConstructorWithBothFalse) {
    modifier m(false, false);

    ASSERT_FALSE(m.isPublic());
    ASSERT_FALSE(m.isExplicitOverride());
}

TEST_F(modifierTest, testSetPublicToTrue) {
    modifier m(false, false);

    ASSERT_FALSE(m.isPublic());

    m.setPublic(true);

    ASSERT_TRUE(m.isPublic());
}

TEST_F(modifierTest, testSetPublicToFalse) {
    modifier m(true, false);

    ASSERT_TRUE(m.isPublic());

    m.setPublic(false);

    ASSERT_FALSE(m.isPublic());
}

TEST_F(modifierTest, testSetExplicitOverrideToTrue) {
    modifier m(true, false);

    ASSERT_FALSE(m.isExplicitOverride());

    m.setExplicitOverride(true);

    ASSERT_TRUE(m.isExplicitOverride());
}

TEST_F(modifierTest, testSetExplicitOverrideToFalse) {
    modifier m(true, true);

    ASSERT_TRUE(m.isExplicitOverride());

    m.setExplicitOverride(false);

    ASSERT_FALSE(m.isExplicitOverride());
}

TEST_F(modifierTest, testTogglePublicMultipleTimes) {
    modifier m;

    m.setPublic(false);
    ASSERT_FALSE(m.isPublic());

    m.setPublic(true);
    ASSERT_TRUE(m.isPublic());

    m.setPublic(false);
    ASSERT_FALSE(m.isPublic());

    m.setPublic(true);
    ASSERT_TRUE(m.isPublic());
}

TEST_F(modifierTest, testToggleOverrideMultipleTimes) {
    modifier m;

    m.setExplicitOverride(true);
    ASSERT_TRUE(m.isExplicitOverride());

    m.setExplicitOverride(false);
    ASSERT_FALSE(m.isExplicitOverride());

    m.setExplicitOverride(true);
    ASSERT_TRUE(m.isExplicitOverride());
}

TEST_F(modifierTest, testIndependentProperties) {
    modifier m;

    m.setPublic(false);
    ASSERT_FALSE(m.isPublic());
    ASSERT_FALSE(m.isExplicitOverride());

    m.setExplicitOverride(true);
    ASSERT_FALSE(m.isPublic());
    ASSERT_TRUE(m.isExplicitOverride());

    m.setPublic(true);
    ASSERT_TRUE(m.isPublic());
    ASSERT_TRUE(m.isExplicitOverride());
}

TEST_F(modifierTest, testPublicOverrideCombination) {
    modifier m1(true, true);
    ASSERT_TRUE(m1.isPublic());
    ASSERT_TRUE(m1.isExplicitOverride());

    modifier m2(false, true);
    ASSERT_FALSE(m2.isPublic());
    ASSERT_TRUE(m2.isExplicitOverride());
}

TEST_F(modifierTest, testProtectedModifier) {
    modifier m(false, false);

    ASSERT_FALSE(m.isPublic());
    ASSERT_FALSE(m.isExplicitOverride());
}

TEST_F(modifierTest, testMultipleModifierInstances) {
    modifier m1;
    modifier m2(false, true);
    modifier m3(true, false);

    ASSERT_TRUE(m1.isPublic());
    ASSERT_FALSE(m1.isExplicitOverride());

    ASSERT_FALSE(m2.isPublic());
    ASSERT_TRUE(m2.isExplicitOverride());

    ASSERT_TRUE(m3.isPublic());
    ASSERT_FALSE(m3.isExplicitOverride());
}
