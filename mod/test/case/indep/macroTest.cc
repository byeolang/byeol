#include "../../common/dep.hpp"

using namespace nm;

TEST(macroTest, nul) {
    nint a = 5;
    nint* aPtr = &a;
    nint* aNul = nullptr;
    nint& refNul = nulOf<nint>();
    nint& refA = *aPtr;

    ASSERT_TRUE(nul(refNul));
    ASSERT_FALSE(nul(refA));
    ASSERT_TRUE(nul(aNul));
    ASSERT_FALSE(nul(aPtr));
}

TEST(macroTest, NM_GETtest) {
    struct A {};

    struct B {
        A& getA() { return a; }

        A& getNul() { return nulOf<A>(); }

        A a;
    } b;

    struct C {
        B& getB() { return b; }

        B& getNul() { return nulOf<B>(); }

        B b;
    } c;

    struct D {
        C& getC() { return c; }

        C& getNul() { return nulOf<C>(); }

        C c;
    } d;

    A& a = safeGet(b, getA());
    ASSERT_FALSE(nul(a));
    A* aNul = &safeGet(b, getNul());
    ASSERT_TRUE(nul(aNul));

    A& a0 = safeGet(b.getNul());
    ASSERT_TRUE(nul(a0));

    A& a2 = safeGet(c, getB(), getA());
    ASSERT_FALSE(nul(a2));
    A* a2Nul = &safeGet(c, getNul(), getA());
    ASSERT_TRUE(nul(a2Nul));

    A& a3 = safeGet(d, getC(), getB(), getA());
    ASSERT_FALSE(nul(a3));
    A* a3Nul = &safeGet(d, getNul(), getB(), getA());
    ASSERT_TRUE(nul(a3Nul));

    A& a4 = safeGet(d.getC().getB(), getA());
    ASSERT_FALSE(nul(a4));
    A* a4Nul = &safeGet(d.getC().getB(), getNul());
    ASSERT_TRUE(nul(a4Nul));
}

TEST(macroTest, UnconstCalltest) {
    struct A {
        NM_ME(A)

    public:
        nbool foo() { return true; }
        nbool foo() const NM_CONST_FUNC(foo())
    } a;

    const A& aConst = a;
    ASSERT_TRUE(aConst.foo());
}
