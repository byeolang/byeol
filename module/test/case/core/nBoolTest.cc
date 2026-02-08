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

    b.set(true);
    ASSERT_TRUE(b.get());

    b.set(false);
    ASSERT_FALSE(b.get());
}

TEST_F(nBoolTest, testLogicalAnd) {
    nBool t(true);
    nBool f(false);

    tstr<scalar> result1 = t._logicalAnd(t) ? new nBool(true) : new nBool(false);
    ASSERT_TRUE(result1.cast<nBool>()->get());

    ASSERT_FALSE(t._logicalAnd(f));
    ASSERT_FALSE(f._logicalAnd(t));
    ASSERT_FALSE(f._logicalAnd(f));
}

TEST_F(nBoolTest, testLogicalOr) {
    nBool t(true);
    nBool f(false);

    ASSERT_TRUE(t._logicalOr(t));
    ASSERT_TRUE(t._logicalOr(f));
    ASSERT_TRUE(f._logicalOr(t));
    ASSERT_FALSE(f._logicalOr(f));
}

TEST_F(nBoolTest, testEquality) {
    nBool t1(true);
    nBool t2(true);
    nBool f1(false);
    nBool f2(false);

    ASSERT_TRUE(t1._eq(t2));
    ASSERT_TRUE(f1._eq(f2));
    ASSERT_FALSE(t1._eq(f1));
    ASSERT_FALSE(f1._eq(t1));
}

TEST_F(nBoolTest, testInequality) {
    nBool t(true);
    nBool f(false);

    ASSERT_TRUE(t._ne(f));
    ASSERT_TRUE(f._ne(t));
    ASSERT_FALSE(t._ne(t));
    ASSERT_FALSE(f._ne(f));
}

TEST_F(nBoolTest, testAddition) {
    nBool t(true);
    nBool f(false);

    tstr<scalar> result1 = t._add(t, false);
    ASSERT_TRUE(result1.cast<nBool>()->get());

    tstr<scalar> result2 = t._add(f, false);
    ASSERT_TRUE(result2.cast<nBool>()->get());

    tstr<scalar> result3 = f._add(f, false);
    ASSERT_FALSE(result3.cast<nBool>()->get());
}

TEST_F(nBoolTest, testSubtraction) {
    nBool t(true);
    nBool f(false);

    tstr<scalar> result1 = t._sub(f, false);
    ASSERT_TRUE(result1.cast<nBool>()->get());

    tstr<scalar> result2 = t._sub(t, false);
    ASSERT_FALSE(result2.cast<nBool>()->get());

    tstr<scalar> result3 = f._sub(t, false);
    ASSERT_TRUE(result3.cast<nBool>()->get()); // false - true wraps to true in bool
}

TEST_F(nBoolTest, testMultiplication) {
    nBool t(true);
    nBool f(false);

    tstr<scalar> result1 = t._mul(t, false);
    ASSERT_TRUE(result1.cast<nBool>()->get());

    tstr<scalar> result2 = t._mul(f, false);
    ASSERT_FALSE(result2.cast<nBool>()->get());

    tstr<scalar> result3 = f._mul(f, false);
    ASSERT_FALSE(result3.cast<nBool>()->get());
}

TEST_F(nBoolTest, testBitwiseOperations) {
    nBool t(true);
    nBool f(false);

    // Bitwise AND
    tstr<scalar> andResult1 = t._bitwiseAnd(t, false);
    ASSERT_TRUE(andResult1.cast<nBool>()->get());

    tstr<scalar> andResult2 = t._bitwiseAnd(f, false);
    ASSERT_FALSE(andResult2.cast<nBool>()->get());

    // Bitwise OR
    tstr<scalar> orResult1 = t._bitwiseOr(f, false);
    ASSERT_TRUE(orResult1.cast<nBool>()->get());

    tstr<scalar> orResult2 = f._bitwiseOr(f, false);
    ASSERT_FALSE(orResult2.cast<nBool>()->get());

    // Bitwise XOR
    tstr<scalar> xorResult1 = t._bitwiseXor(f, false);
    ASSERT_TRUE(xorResult1.cast<nBool>()->get());

    tstr<scalar> xorResult2 = t._bitwiseXor(t, false);
    ASSERT_FALSE(xorResult2.cast<nBool>()->get());
}

TEST_F(nBoolTest, testComparisons) {
    nBool t(true);
    nBool f(false);

    ASSERT_TRUE(t._gt(f));
    ASSERT_FALSE(f._gt(t));
    ASSERT_FALSE(t._gt(t));

    ASSERT_TRUE(f._lt(t));
    ASSERT_FALSE(t._lt(f));
    ASSERT_FALSE(f._lt(f));

    ASSERT_TRUE(t._ge(t));
    ASSERT_TRUE(t._ge(f));
    ASSERT_FALSE(f._ge(t));

    ASSERT_TRUE(f._le(f));
    ASSERT_TRUE(f._le(t));
    ASSERT_FALSE(t._le(f));
}

TEST_F(nBoolTest, testBitwiseNot) {
    nBool t(true);
    nBool f(false);

    tstr<scalar> notTrue = t.bitwiseNot();
    ASSERT_EQ(notTrue.cast<nInt>()->get(), 0);

    tstr<scalar> notFalse = f.bitwiseNot();
    ASSERT_EQ(notFalse.cast<nInt>()->get(), 1);
}

TEST_F(nBoolTest, testShiftOperations) {
    nBool t(true);
    nBool f(false);

    tstr<scalar> lshiftResult = t._lshift(t, false);
    ASSERT_FALSE(lshiftResult.cast<nBool>()->get()); // 1 << 1 = 2, which is != 0, but stored as bool

    tstr<scalar> rshiftResult = t._rshift(t, false);
    ASSERT_FALSE(rshiftResult.cast<nBool>()->get()); // 1 >> 1 = 0
}

TEST_F(nBoolTest, testMovAssignment) {
    nBool b1(false);
    nBool b2(true);

    b1._mov(b2);
    ASSERT_TRUE(b1.get());
}
