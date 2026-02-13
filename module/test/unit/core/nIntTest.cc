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

    n.get() = 20;
    ASSERT_EQ(n.get(), 20);

    n.get() = -50;
    ASSERT_EQ(n.get(), -50);

    n.get() = 0;
    ASSERT_EQ(n.get(), 0);
}

TEST_F(nIntTest, testAddition) {
    nInt a(10);
    nInt b(20);
    nInt c(-5);

    tstr<scalar> result1 = a.add(b);
    ASSERT_EQ(result1.cast<nInt>()->get(), 30);

    tstr<scalar> result2 = a.add(c);
    ASSERT_EQ(result2.cast<nInt>()->get(), 5);

    tstr<scalar> result3 = c.add(c);
    ASSERT_EQ(result3.cast<nInt>()->get(), -10);
}

TEST_F(nIntTest, testSubtraction) {
    nInt a(50);
    nInt b(30);
    nInt c(-10);

    tstr<scalar> result1 = a.sub(b);
    ASSERT_EQ(result1.cast<nInt>()->get(), 20);

    tstr<scalar> result2 = a.sub(c);
    ASSERT_EQ(result2.cast<nInt>()->get(), 60);

    tstr<scalar> result3 = b.sub(a);
    ASSERT_EQ(result3.cast<nInt>()->get(), -20);
}

TEST_F(nIntTest, testMultiplication) {
    nInt a(5);
    nInt b(7);
    nInt c(-3);
    nInt zero(0);

    tstr<scalar> result1 = a.mul(b);
    ASSERT_EQ(result1.cast<nInt>()->get(), 35);

    tstr<scalar> result2 = a.mul(c);
    ASSERT_EQ(result2.cast<nInt>()->get(), -15);

    tstr<scalar> result3 = b.mul(zero);
    ASSERT_EQ(result3.cast<nInt>()->get(), 0);
}

TEST_F(nIntTest, testDivision) {
    nInt a(20);
    nInt b(5);
    nInt c(-4);

    tstr<scalar> result1 = a.div(b);
    ASSERT_EQ(result1.cast<nInt>()->get(), 4);

    tstr<scalar> result2 = a.div(c);
    ASSERT_EQ(result2.cast<nInt>()->get(), -5);

    nInt d(17);
    nInt e(5);
    tstr<scalar> result3 = d.div(e);
    ASSERT_EQ(result3.cast<nInt>()->get(), 3);
}

TEST_F(nIntTest, testModulo) {
    nInt a(17);
    nInt b(5);
    nInt c(20);
    nInt d(4);

    tstr<scalar> result1 = a.mod(b);
    ASSERT_EQ(result1.cast<nInt>()->get(), 2);

    tstr<scalar> result2 = c.mod(d);
    ASSERT_EQ(result2.cast<nInt>()->get(), 0);
}

TEST_F(nIntTest, testBitwiseAnd) {
    nInt a(12); // 1100 in binary
    nInt b(10); // 1010 in binary

    tstr<scalar> result = a.bitwiseAnd(b);
    ASSERT_EQ(result.cast<nInt>()->get(), 8); // 1000 in binary
}

TEST_F(nIntTest, testBitwiseOr) {
    nInt a(12); // 1100 in binary
    nInt b(10); // 1010 in binary

    tstr<scalar> result = a.bitwiseOr(b);
    ASSERT_EQ(result.cast<nInt>()->get(), 14); // 1110 in binary
}

TEST_F(nIntTest, testBitwiseXor) {
    nInt a(12); // 1100 in binary
    nInt b(10); // 1010 in binary

    tstr<scalar> result = a.bitwiseXor(b);
    ASSERT_EQ(result.cast<nInt>()->get(), 6); // 0110 in binary
}

TEST_F(nIntTest, testBitwiseNot) {
    nInt a(5);

    tstr<scalar> result = a.bitwiseNot();
    ASSERT_EQ(result.cast<nInt>()->get(), ~5);
}

TEST_F(nIntTest, testLeftShift) {
    nInt a(5);
    nInt b(2);

    tstr<scalar> result = a.lshift(b);
    ASSERT_EQ(result.cast<nInt>()->get(), 20); // 5 << 2 = 20
}

TEST_F(nIntTest, testRightShift) {
    nInt a(20);
    nInt b(2);

    tstr<scalar> result = a.rshift(b);
    ASSERT_EQ(result.cast<nInt>()->get(), 5); // 20 >> 2 = 5
}

TEST_F(nIntTest, testEquality) {
    nInt a(42);
    nInt b(42);
    nInt c(100);

    ASSERT_TRUE(a.eq(b));
    ASSERT_FALSE(a.eq(c));
}

TEST_F(nIntTest, testInequality) {
    nInt a(42);
    nInt b(100);

    ASSERT_TRUE(a.ne(b));
    ASSERT_FALSE(a.ne(a));
}

TEST_F(nIntTest, testGreaterThan) {
    nInt a(100);
    nInt b(50);
    nInt c(100);

    ASSERT_TRUE(a.gt(b));
    ASSERT_FALSE(b.gt(a));
    ASSERT_FALSE(a.gt(c));
}

TEST_F(nIntTest, testLessThan) {
    nInt a(50);
    nInt b(100);
    nInt c(50);

    ASSERT_TRUE(a.lt(b));
    ASSERT_FALSE(b.lt(a));
    ASSERT_FALSE(a.lt(c));
}

TEST_F(nIntTest, testGreaterOrEqual) {
    nInt a(100);
    nInt b(50);
    nInt c(100);

    ASSERT_TRUE(a.ge(b));
    ASSERT_TRUE(a.ge(c));
    ASSERT_FALSE(b.ge(a));
}

TEST_F(nIntTest, testLessOrEqual) {
    nInt a(50);
    nInt b(100);
    nInt c(50);

    ASSERT_TRUE(a.le(b));
    ASSERT_TRUE(a.le(c));
    ASSERT_FALSE(b.le(a));
}

TEST_F(nIntTest, testLogicalAnd) {
    nInt a(5);
    nInt b(10);
    nInt zero(0);

    ASSERT_TRUE(a.logicalAnd(b));
    ASSERT_FALSE(a.logicalAnd(zero));
    ASSERT_FALSE(zero.logicalAnd(b));
}

TEST_F(nIntTest, testLogicalOr) {
    nInt a(5);
    nInt zero1(0);
    nInt zero2(0);

    ASSERT_TRUE(a.logicalOr(zero1));
    ASSERT_TRUE(zero1.logicalOr(a));
    ASSERT_FALSE(zero1.logicalOr(zero2));
}

TEST_F(nIntTest, testMovAssignment) {
    nInt a(10);
    nInt b(20);

    a.mov(b);
    ASSERT_EQ(a.get(), 20);
}

TEST_F(nIntTest, testNegativeNumbers) {
    nInt a(-10);
    nInt b(-20);

    tstr<scalar> result1 = a.add(b);
    ASSERT_EQ(result1.cast<nInt>()->get(), -30);

    tstr<scalar> result2 = a.mul(b);
    ASSERT_EQ(result2.cast<nInt>()->get(), 200);
}

TEST_F(nIntTest, testZero) {
    nInt zero(0);
    nInt a(42);

    tstr<scalar> result1 = zero.add(a);
    ASSERT_EQ(result1.cast<nInt>()->get(), 42);

    tstr<scalar> result2 = zero.mul(a);
    ASSERT_EQ(result2.cast<nInt>()->get(), 0);
}
