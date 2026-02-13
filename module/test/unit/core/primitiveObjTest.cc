#include "test/byeolTest.hpp"

using namespace by;
using namespace std;

struct tscalarTest: public byeolTest {};

TEST_F(tscalarTest, testCloneWIntInMgd) {
    nInt origin(1);

    tstr<nInt> inst((nInt*) origin.clone());
    ASSERT_TRUE(inst);
    ASSERT_EQ(origin.get(), inst->get());
    ASSERT_NE(&origin, &(*inst));
}

TEST_F(tscalarTest, subsIsNotDummy) {
    nInt val(2);
    int initial = val.subs().len();

    bicontainable& subs = val.subs();
    subs.add("not work", new nInt(3));
    ASSERT_EQ(subs.len(), initial + 1);
    ASSERT_TRUE(subs.in("not work"));

    subs.del("not work");
    ASSERT_EQ(subs.len(), initial);
}

TEST_F(tscalarTest, operatorExceptAddingToStrTypeShouldNotWork) {
    nStr s1("123");
    nStr s2("3");

    tstr<nStr> res = s1.sub(s2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), s1.get());

    res = s1.mul(s2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), s1.get());

    res = s1.div(s2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), s1.get());

    res = s1.bitwiseAnd(s2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), s1.get());

    res = s1.bitwiseXor(s2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), s1.get());

    res = s1.bitwiseOr(s2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), s1.get());

    res = s1.lshift(s2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), s1.get());

    res = s1.rshift(s2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), s1.get());

    ASSERT_EQ(s1.gt(s2), s1.get() > s2.get());
    ASSERT_EQ(s1.ge(s2), s1.get() >= s2.get());
    ASSERT_EQ(s1.lt(s2), s1.get() < s2.get());
    ASSERT_EQ(s1.le(s2), s1.get() <= s2.get());
    ASSERT_FALSE(s1.logicalAnd(s2));
    ASSERT_FALSE(s1.logicalOr(s2));
}

TEST_F(tscalarTest, voidDoesntSupportAnyOperator) {
    nVoid v1;
    nVoid v2;
    const nVoid& expect = nVoid::singleton();

    tstr<nVoid> res = v1.sub(v2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res.get(), &expect);

    res = expect.mul(v2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res.get(), &expect);

    res = expect.div(v2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res.get(), &expect);

    res = expect.bitwiseAnd(v2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res.get(), &expect);

    res = expect.bitwiseXor(v2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res.get(), &expect);

    res = expect.bitwiseOr(v2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res.get(), &expect);

    res = expect.lshift(v2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res.get(), &expect);

    res = expect.rshift(v2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res.get(), &expect);

    ASSERT_FALSE(expect.gt(v2));
    ASSERT_FALSE(expect.ge(v2));
    ASSERT_FALSE(expect.lt(v2));
    ASSERT_FALSE(expect.le(v2));
    ASSERT_FALSE(expect.logicalAnd(v2));
    ASSERT_FALSE(expect.logicalOr(v2));
}

TEST_F(tscalarTest, byteOperators) {
    nByte b1(123);
    nByte b2(3);

    tstr<nByte> res = b1.sub(b2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), 123 - 3);

    res = b1.add(b2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), 123 + 3);

    res = b1.mul(b2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), (nuchar) (123 * 3));

    res = b1.div(b2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), (nuchar) (123 / 3));

    res = b1.bitwiseAnd(b2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), 123 & 3);

    res = b1.bitwiseXor(b2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), (nuchar) (123 ^ 3));

    res = b1.bitwiseOr(b2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), 123 | 3);

    res = b1.lshift(b2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), (nuchar) (123 << 3));

    res = b1.rshift(b2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), (nuchar) (123 >> 3));

    ASSERT_EQ(b1.gt(b2), b1.get() > b2.get());
    ASSERT_EQ(b1.ge(b2), b1.get() >= b2.get());
    ASSERT_EQ(b1.lt(b2), b1.get() < b2.get());
    ASSERT_EQ(b1.le(b2), b1.get() <= b2.get());
    ASSERT_EQ(b1.logicalAnd(b2), b1.get() && b2.get());
    ASSERT_EQ(b1.logicalOr(b2), b1.get() || b2.get());
}

TEST_F(tscalarTest, boolOperators) {
    nBool b1(true);
    nBool b2(false);

    tstr<nBool> res = b1.sub(b2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), (nbool) (1 - 0));

    res = b1.add(b2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), (nbool) (true + false));

    res = b1.mul(b2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), (nbool) (true && false));

    res = b2.div(b1);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), (nbool) (false && true));

    res = b1.bitwiseAnd(b2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), (nbool) (true & false));

    res = b1.bitwiseXor(b2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), (nbool) (true ^ false));

    res = b1.bitwiseOr(b2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), (nbool) (true | false));

    res = b1.lshift(b2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), true);

    res = b1.rshift(b2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), true);

    ASSERT_EQ(b1.gt(b2), b1.get() > b2.get());
    ASSERT_EQ(b1.ge(b2), b1.get() >= b2.get());
    ASSERT_EQ(b1.lt(b2), b1.get() < b2.get());
    ASSERT_EQ(b1.le(b2), b1.get() <= b2.get());
    ASSERT_EQ(b1.logicalAnd(b2), b1.get() && b2.get());
    ASSERT_EQ(b1.logicalOr(b2), b1.get() || b2.get());
}

TEST_F(tscalarTest, fltOperators) {
    nFlt f1(12.25f);
    nFlt f2(0.25f);

    tstr<nFlt> res = f1.sub(f2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), (nflt) (12.25f - 0.25f));

    res = f1.add(f2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), (nflt) (12.25f + 0.25f));

    res = f1.mul(f2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), (nflt) (12.25f * 0.25f));

    res = f2.div(f1);
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), (nflt) (0.25f / 12.25f));

    res = f1.bitwiseAnd(f2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res.get(), &f1);

    res = f1.bitwiseXor(f2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res.get(), &f1);

    res = f1.bitwiseOr(f2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res.get(), &f1);

    res = f1.lshift(f2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res.get(), &f1);

    res = f1.rshift(f2);
    ASSERT_TRUE(res);
    ASSERT_EQ(res.get(), &f1);

    ASSERT_EQ(f1.gt(f2), f1.get() > f2.get());
    ASSERT_EQ(f1.ge(f2), f1.get() >= f2.get());
    ASSERT_EQ(f1.lt(f2), f1.get() < f2.get());
    ASSERT_EQ(f1.le(f2), f1.get() <= f2.get());
    ASSERT_EQ(f1.logicalAnd(f2), f1.get() && f2.get());
    ASSERT_EQ(f1.logicalOr(f2), f1.get() || f2.get());
}
