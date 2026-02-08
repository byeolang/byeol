#include "test/byeolTest.hpp"

using namespace by;
using namespace std;

struct nIntTest: public byeolTest {};

TEST_F(nIntTest, testDefaultConstructor) {
    nInt n;

    ASSERT_EQ(n.get(), 0);
}

TEST_F(nIntTest, testConstructorWithValue) {
    nInt n1(42);
    nInt n2(-100);
    nInt n3(0);

    ASSERT_EQ(n1.get(), 42);
    ASSERT_EQ(n2.get(), -100);
    ASSERT_EQ(n3.get(), 0);
}

TEST_F(nIntTest, testSet) {
    nInt n(10);

    n.set(20);
    ASSERT_EQ(n.get(), 20);

    n.set(-50);
    ASSERT_EQ(n.get(), -50);

    n.set(0);
    ASSERT_EQ(n.get(), 0);
}

TEST_F(nIntTest, testAddition) {
    nInt a(10);
    nInt b(20);
    nInt c(-5);

    tstr<scalar> result1 = a._add(b, false);
    ASSERT_EQ(result1.cast<nInt>()->get(), 30);

    tstr<scalar> result2 = a._add(c, false);
    ASSERT_EQ(result2.cast<nInt>()->get(), 5);

    tstr<scalar> result3 = c._add(c, false);
    ASSERT_EQ(result3.cast<nInt>()->get(), -10);
}

TEST_F(nIntTest, testSubtraction) {
    nInt a(50);
    nInt b(30);
    nInt c(-10);

    tstr<scalar> result1 = a._sub(b, false);
    ASSERT_EQ(result1.cast<nInt>()->get(), 20);

    tstr<scalar> result2 = a._sub(c, false);
    ASSERT_EQ(result2.cast<nInt>()->get(), 60);

    tstr<scalar> result3 = b._sub(a, false);
    ASSERT_EQ(result3.cast<nInt>()->get(), -20);
}

TEST_F(nIntTest, testMultiplication) {
    nInt a(5);
    nInt b(7);
    nInt c(-3);
    nInt zero(0);

    tstr<scalar> result1 = a._mul(b, false);
    ASSERT_EQ(result1.cast<nInt>()->get(), 35);

    tstr<scalar> result2 = a._mul(c, false);
    ASSERT_EQ(result2.cast<nInt>()->get(), -15);

    tstr<scalar> result3 = b._mul(zero, false);
    ASSERT_EQ(result3.cast<nInt>()->get(), 0);
}

TEST_F(nIntTest, testDivision) {
    nInt a(20);
    nInt b(5);
    nInt c(-4);

    tstr<scalar> result1 = a._div(b, false);
    ASSERT_EQ(result1.cast<nInt>()->get(), 4);

    tstr<scalar> result2 = a._div(c, false);
    ASSERT_EQ(result2.cast<nInt>()->get(), -5);

    nInt d(17);
    nInt e(5);
    tstr<scalar> result3 = d._div(e, false);
    ASSERT_EQ(result3.cast<nInt>()->get(), 3);
}

TEST_F(nIntTest, testModulo) {
    nInt a(17);
    nInt b(5);
    nInt c(20);
    nInt d(4);

    tstr<scalar> result1 = a._mod(b, false);
    ASSERT_EQ(result1.cast<nInt>()->get(), 2);

    tstr<scalar> result2 = c._mod(d, false);
    ASSERT_EQ(result2.cast<nInt>()->get(), 0);
}

TEST_F(nIntTest, testBitwiseAnd) {
    nInt a(12);  // 1100 in binary
    nInt b(10);  // 1010 in binary

    tstr<scalar> result = a._bitwiseAnd(b, false);
    ASSERT_EQ(result.cast<nInt>()->get(), 8);  // 1000 in binary
}

TEST_F(nIntTest, testBitwiseOr) {
    nInt a(12);  // 1100 in binary
    nInt b(10);  // 1010 in binary

    tstr<scalar> result = a._bitwiseOr(b, false);
    ASSERT_EQ(result.cast<nInt>()->get(), 14);  // 1110 in binary
}

TEST_F(nIntTest, testBitwiseXor) {
    nInt a(12);  // 1100 in binary
    nInt b(10);  // 1010 in binary

    tstr<scalar> result = a._bitwiseXor(b, false);
    ASSERT_EQ(result.cast<nInt>()->get(), 6);  // 0110 in binary
}

TEST_F(nIntTest, testBitwiseNot) {
    nInt a(5);

    tstr<scalar> result = a.bitwiseNot();
    ASSERT_EQ(result.cast<nInt>()->get(), ~5);
}

TEST_F(nIntTest, testLeftShift) {
    nInt a(5);
    nInt b(2);

    tstr<scalar> result = a._lshift(b, false);
    ASSERT_EQ(result.cast<nInt>()->get(), 20);  // 5 << 2 = 20
}

TEST_F(nIntTest, testRightShift) {
    nInt a(20);
    nInt b(2);

    tstr<scalar> result = a._rshift(b, false);
    ASSERT_EQ(result.cast<nInt>()->get(), 5);  // 20 >> 2 = 5
}

TEST_F(nIntTest, testEquality) {
    nInt a(42);
    nInt b(42);
    nInt c(100);

    ASSERT_TRUE(a._eq(b));
    ASSERT_FALSE(a._eq(c));
}

TEST_F(nIntTest, testInequality) {
    nInt a(42);
    nInt b(100);

    ASSERT_TRUE(a._ne(b));
    ASSERT_FALSE(a._ne(a));
}

TEST_F(nIntTest, testGreaterThan) {
    nInt a(100);
    nInt b(50);
    nInt c(100);

    ASSERT_TRUE(a._gt(b));
    ASSERT_FALSE(b._gt(a));
    ASSERT_FALSE(a._gt(c));
}

TEST_F(nIntTest, testLessThan) {
    nInt a(50);
    nInt b(100);
    nInt c(50);

    ASSERT_TRUE(a._lt(b));
    ASSERT_FALSE(b._lt(a));
    ASSERT_FALSE(a._lt(c));
}

TEST_F(nIntTest, testGreaterOrEqual) {
    nInt a(100);
    nInt b(50);
    nInt c(100);

    ASSERT_TRUE(a._ge(b));
    ASSERT_TRUE(a._ge(c));
    ASSERT_FALSE(b._ge(a));
}

TEST_F(nIntTest, testLessOrEqual) {
    nInt a(50);
    nInt b(100);
    nInt c(50);

    ASSERT_TRUE(a._le(b));
    ASSERT_TRUE(a._le(c));
    ASSERT_FALSE(b._le(a));
}

TEST_F(nIntTest, testLogicalAnd) {
    nInt a(5);
    nInt b(10);
    nInt zero(0);

    ASSERT_TRUE(a._logicalAnd(b));
    ASSERT_FALSE(a._logicalAnd(zero));
    ASSERT_FALSE(zero._logicalAnd(b));
}

TEST_F(nIntTest, testLogicalOr) {
    nInt a(5);
    nInt zero1(0);
    nInt zero2(0);

    ASSERT_TRUE(a._logicalOr(zero1));
    ASSERT_TRUE(zero1._logicalOr(a));
    ASSERT_FALSE(zero1._logicalOr(zero2));
}

TEST_F(nIntTest, testMovAssignment) {
    nInt a(10);
    nInt b(20);

    a._mov(b);
    ASSERT_EQ(a.get(), 20);
}

TEST_F(nIntTest, testNegativeNumbers) {
    nInt a(-10);
    nInt b(-20);

    tstr<scalar> result1 = a._add(b, false);
    ASSERT_EQ(result1.cast<nInt>()->get(), -30);

    tstr<scalar> result2 = a._mul(b, false);
    ASSERT_EQ(result2.cast<nInt>()->get(), 200);
}

TEST_F(nIntTest, testZero) {
    nInt zero(0);
    nInt a(42);

    tstr<scalar> result1 = zero._add(a, false);
    ASSERT_EQ(result1.cast<nInt>()->get(), 42);

    tstr<scalar> result2 = zero._mul(a, false);
    ASSERT_EQ(result2.cast<nInt>()->get(), 0);
}
