#include "test/byeolTest.hpp"

using namespace by;
using namespace std;

struct nBoolTest: public byeolTest {};

TEST_F(nBoolTest, testDefaultConstructor) {
    nBool b;

    // Default constructed nBool should have value false
    ASSERT_FALSE(b.get());
}

TEST_F(nBoolTest, testConstructorWithTrue) {
    nBool b(true);

    ASSERT_TRUE(b.get());
}

TEST_F(nBoolTest, testConstructorWithFalse) {
    nBool b(false);

    ASSERT_FALSE(b.get());
}

TEST_F(nBoolTest, testSet) {
    nBool b(false);

    b.get() = true;
    ASSERT_TRUE(b.get());

    b.get() = false;
    ASSERT_FALSE(b.get());
}

TEST_F(nBoolTest, testLogicalAnd) {
    nBool t(true);
    nBool f(false);

    tstr<scalar> result1 = t.logicalAnd(t) ? new nBool(true) : new nBool(false);
    ASSERT_TRUE(result1.cast<nBool>()->get());

    ASSERT_FALSE(t.logicalAnd(f));
    ASSERT_FALSE(f.logicalAnd(t));
    ASSERT_FALSE(f.logicalAnd(f));
}

TEST_F(nBoolTest, testLogicalOr) {
    nBool t(true);
    nBool f(false);

    ASSERT_TRUE(t.logicalOr(t));
    ASSERT_TRUE(t.logicalOr(f));
    ASSERT_TRUE(f.logicalOr(t));
    ASSERT_FALSE(f.logicalOr(f));
}

TEST_F(nBoolTest, testEquality) {
    nBool t1(true);
    nBool t2(true);
    nBool f1(false);
    nBool f2(false);

    ASSERT_TRUE(t1.eq(t2));
    ASSERT_TRUE(f1.eq(f2));
    ASSERT_FALSE(t1.eq(f1));
    ASSERT_FALSE(f1.eq(t1));
}

TEST_F(nBoolTest, testInequality) {
    nBool t(true);
    nBool f(false);

    ASSERT_TRUE(t.ne(f));
    ASSERT_TRUE(f.ne(t));
    ASSERT_FALSE(t.ne(t));
    ASSERT_FALSE(f.ne(f));
}

TEST_F(nBoolTest, testAddition) {
    nBool t(true);
    nBool f(false);

    tstr<scalar> result1 = t.add(t);
    ASSERT_TRUE(result1.cast<nBool>()->get());

    tstr<scalar> result2 = t.add(f);
    ASSERT_TRUE(result2.cast<nBool>()->get());

    tstr<scalar> result3 = f.add(f);
    ASSERT_FALSE(result3.cast<nBool>()->get());
}

TEST_F(nBoolTest, testSubtraction) {
    nBool t(true);
    nBool f(false);

    tstr<scalar> result1 = t.sub(f);
    ASSERT_TRUE(result1.cast<nBool>()->get());

    tstr<scalar> result2 = t.sub(t);
    ASSERT_FALSE(result2.cast<nBool>()->get());

    tstr<scalar> result3 = f.sub(t);
    ASSERT_TRUE(result3.cast<nBool>()->get()); // false - true wraps to true in bool
}

TEST_F(nBoolTest, testMultiplication) {
    nBool t(true);
    nBool f(false);

    tstr<scalar> result1 = t.mul(t);
    ASSERT_TRUE(result1.cast<nBool>()->get());

    tstr<scalar> result2 = t.mul(f);
    ASSERT_FALSE(result2.cast<nBool>()->get());

    tstr<scalar> result3 = f.mul(f);
    ASSERT_FALSE(result3.cast<nBool>()->get());
}

TEST_F(nBoolTest, testBitwiseOperations) {
    nBool t(true);
    nBool f(false);

    // Bitwise AND
    tstr<scalar> andResult1 = t.bitwiseAnd(t);
    ASSERT_TRUE(andResult1.cast<nBool>()->get());

    tstr<scalar> andResult2 = t.bitwiseAnd(f);
    ASSERT_FALSE(andResult2.cast<nBool>()->get());

    // Bitwise OR
    tstr<scalar> orResult1 = t.bitwiseOr(f);
    ASSERT_TRUE(orResult1.cast<nBool>()->get());

    tstr<scalar> orResult2 = f.bitwiseOr(f);
    ASSERT_FALSE(orResult2.cast<nBool>()->get());

    // Bitwise XOR
    tstr<scalar> xorResult1 = t.bitwiseXor(f);
    ASSERT_TRUE(xorResult1.cast<nBool>()->get());

    tstr<scalar> xorResult2 = t.bitwiseXor(t);
    ASSERT_FALSE(xorResult2.cast<nBool>()->get());
}

TEST_F(nBoolTest, testComparisons) {
    nBool t(true);
    nBool f(false);

    ASSERT_TRUE(t.gt(f));
    ASSERT_FALSE(f.gt(t));
    ASSERT_FALSE(t.gt(t));

    ASSERT_TRUE(f.lt(t));
    ASSERT_FALSE(t.lt(f));
    ASSERT_FALSE(f.lt(f));

    ASSERT_TRUE(t.ge(t));
    ASSERT_TRUE(t.ge(f));
    ASSERT_FALSE(f.ge(t));

    ASSERT_TRUE(f.le(f));
    ASSERT_TRUE(f.le(t));
    ASSERT_FALSE(t.le(f));
}

TEST_F(nBoolTest, testBitwiseNot) {
    nBool t(true);
    nBool f(false);

    tstr<scalar> notTrue = t.bitwiseNot();
    ASSERT_EQ(*notTrue.cast<nbool>(), false);

    tstr<scalar> notFalse = f.bitwiseNot();
    ASSERT_EQ(*notFalse.cast<nbool>(), true);
}

TEST_F(nBoolTest, testShiftOperations) {
    nBool t(true);
    nBool f(false);

    tstr<scalar> lshiftResult = t.lshift(t);
    ASSERT_TRUE(lshiftResult.cast<nBool>()->get()); // 1 << 1 = 2, which is != 0

    tstr<scalar> rshiftResult = t.rshift(t);
    ASSERT_FALSE(rshiftResult.cast<nBool>()->get()); // 1 >> 1 = 0
}

TEST_F(nBoolTest, testMovAssignment) {
    nBool b1(false);
    nBool b2(true);

    b1.mov(b2);
    ASSERT_TRUE(b1.get());
}
