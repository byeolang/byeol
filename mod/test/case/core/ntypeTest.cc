#include "../../common/dep.hpp"

using namespace nm;
using namespace std;

namespace {
    class myObj: public obj {
        NM(CLASS(myObj, obj))

    public:
        const baseObj& getOrigin() const override { return *this; }

        using super::subs;

        scope& subs() override { return *_subs; }

    private:
        tstr<scope> _subs;
    };
}

TEST(ntypeTest, basicWTypeAPITest) {
    nInt intObj;
    myObj myO;
    const ntype& myType = myO.getType();
    const ntype& intType = intObj.getType();
    const ntype& inttType = ttype<nInt>::get();

    ASSERT_EQ(&intType, &inttType);
    ASSERT_EQ(intType, inttType);

    ASSERT_FALSE(myType.isImmutable());
    ASSERT_TRUE(intType.isImmutable());

    ASSERT_FALSE(intType.isImpli<nStr>());
    ASSERT_FALSE(intObj.isImpli<nStr>());
    ASSERT_TRUE(intType.isImpli<nFlt>());
    ASSERT_TRUE(intObj.isImpli<nFlt>());
    ASSERT_FALSE(inttType.isImpli<myObj>());
    ASSERT_FALSE(myO.isImpli<nInt>());

    ASSERT_TRUE(intType.is<nBool>());
    ASSERT_TRUE(intObj.is<nBool>());
    ASSERT_FALSE(inttType.is<myObj>());
    ASSERT_FALSE(myO.getType().is<nInt>());
}

TEST(ntypeTest, makeTest) {
    tstr<nInt> new1 = ttype<nInt>().makeAs<nInt>();
    ASSERT_TRUE(new1);
    ASSERT_EQ(new1->get(), 0);
}

TEST(ntypeTest, allImplicitCastingForPrimitives) {
    nInt i;
    nFlt f;
    nBool b;
    nByte by;
    nStr s;

    // int:
    ASSERT_TRUE(i.isImpli<nInt>());
    ASSERT_TRUE(i.isImpli<nFlt>());
    ASSERT_TRUE(i.isImpli<nBool>());
    ASSERT_TRUE(i.isImpli<nByte>());
    ASSERT_FALSE(i.isImpli<nStr>());

    // float:
    ASSERT_TRUE(f.isImpli<nInt>());
    ASSERT_TRUE(f.isImpli<nFlt>());
    ASSERT_TRUE(f.isImpli<nBool>());
    ASSERT_TRUE(f.isImpli<nByte>());
    ASSERT_FALSE(f.isImpli<nStr>());

    // bool:
    ASSERT_TRUE(b.isImpli<nInt>());
    ASSERT_TRUE(b.isImpli<nFlt>());
    ASSERT_TRUE(b.isImpli<nBool>());
    ASSERT_TRUE(b.isImpli<nByte>());
    ASSERT_FALSE(b.isImpli<nStr>());

    // byte:
    ASSERT_TRUE(by.isImpli<nInt>());
    ASSERT_FALSE(by.isImpli<nFlt>());
    ASSERT_TRUE(by.isImpli<nBool>());
    ASSERT_TRUE(by.isImpli<nByte>());
    ASSERT_FALSE(by.isImpli<nStr>());

    // str:
    ASSERT_FALSE(s.isImpli<nInt>());
    ASSERT_FALSE(s.isImpli<nFlt>());
    ASSERT_FALSE(s.isImpli<nBool>());
    ASSERT_FALSE(s.isImpli<nByte>());
    ASSERT_TRUE(s.isImpli<nStr>());
}

TEST(ntypeTest, funcAndClosureIsImplicitCastable) {
    nm::thread th; // declare namespace because there is a `thread` class in some version of macos
    nm::thread::set(th);
    obj dummy;
    frameInteract f1(dummy);

    params leftPs;
    leftPs.add(new param("a", *new nInt()));
    leftPs.add(new param("b", *new nFlt()));
    str lhs = new func(*new modifier(), typeMaker::make<func>("left", leftPs, *new nInt()));
    ASSERT_TRUE(lhs);
    tstr<closure> rhs = closure::make(*lhs);
    ASSERT_TRUE(rhs);

    ASSERT_TRUE(lhs->isSub<baseFunc>());
    ASSERT_TRUE(rhs->isSub<closure>());
    ASSERT_FALSE(nul(rhs->getFunc()));

    ASSERT_TRUE(lhs->isImpli(*rhs));
    ASSERT_TRUE(rhs->isImpli(*lhs));

    nm::thread::set();
}

TEST(ntypeTest, funcAndClosureIsImplicitCastableWithDifferentSameFunc) {
    nm::thread th;
    nm::thread::set(th);
    obj dummy;
    frameInteract f1(dummy);

    params leftPs;
    leftPs.add(new param("a", *new nInt()));
    leftPs.add(new param("b", *new nFlt()));
    str lhs = new func(*new modifier(), typeMaker::make<func>("left", leftPs, *new nInt()));
    ASSERT_TRUE(lhs);

    str boo = new func(*new modifier(), typeMaker::make<func>("boo", leftPs, *new nInt()));
    tstr<closure> rhs = closure::make(*boo);
    ASSERT_TRUE(rhs);

    ASSERT_TRUE(lhs->isSub<baseFunc>());
    ASSERT_TRUE(rhs->isSub<closure>());
    ASSERT_FALSE(nul(rhs->getFunc()));

    ASSERT_TRUE(lhs->isImpli(*rhs));
    ASSERT_TRUE(rhs->isImpli(*lhs));

    nm::thread::set();
}

TEST(ntypeTest, funcAndClosureIsNotCastableReturnTypeIsDifferentNegative) {
    nm::thread th;
    nm::thread::set(th);
    obj dummy;
    frameInteract f1(dummy);

    params leftPs;
    leftPs.add(new param("a", *new nInt()));
    leftPs.add(new param("b", *new nFlt()));
    str foo = new func(*new modifier(), typeMaker::make<func>("foo", leftPs, *new nInt()));
    ASSERT_TRUE(foo);

    // almost same to 'foo' except return type:
    //  but you may think that this is implicit castable between int and byte when you try.
    params booPs;
    booPs.add(new param("r", *new nInt())); // it's okay to have different param name.
    booPs.add(new param("x", *new nFlt()));
    str boo = new func(*new modifier(), typeMaker::make<func>("boo", booPs, *new nByte()));
    tstr<closure> rhs = closure::make(*boo);
    ASSERT_TRUE(rhs);

    ASSERT_TRUE(foo->isSub<baseFunc>());
    ASSERT_TRUE(rhs->isSub<closure>());
    ASSERT_FALSE(nul(rhs->getFunc()));

    //  but truth is, it's not allowed to cast between func/closure if its return type isn't
    //  exactly same.
    ASSERT_FALSE(foo->isImpli(*rhs));
    ASSERT_FALSE(rhs->isImpli(*foo));

    nm::thread::set();
}

TEST(ntypeTest, funcAndClosureIsNotCastableSomeParamIsDifferentNegative) {
    nm::thread th;
    nm::thread::set(th);
    obj dummy;
    frameInteract f1(dummy);

    params leftPs;
    leftPs.add(new param("a", *new nInt()));
    leftPs.add(new param("b", *new nFlt()));
    str foo = new func(*new modifier(), typeMaker::make<func>("foo", leftPs, *new nInt()));
    ASSERT_TRUE(foo);

    // almost same to 'foo' except return type:
    params booPs;
    booPs.add(new param("r", *new nByte())); // <-- it's byte, not int!
    booPs.add(new param("x", *new nFlt()));
    //  but you may still think that this is implicit castable between int and byte when you
    //  try.
    str boo = new func(*new modifier(), typeMaker::make<func>("boo", booPs, *new nByte()));
    tstr<closure> rhs = closure::make(*boo);
    ASSERT_TRUE(rhs);

    ASSERT_TRUE(foo->isSub<baseFunc>());
    ASSERT_TRUE(rhs->isSub<closure>());
    ASSERT_FALSE(nul(rhs->getFunc()));

    //  but truth is, it's not allowed to cast between func/closure if its any param type isn't
    //  exactly same.
    ASSERT_FALSE(foo->isImpli(*rhs));
    ASSERT_FALSE(rhs->isImpli(*foo));

    nm::thread::set();
}
