#include "test/common/dep.hpp"

using namespace nm;

TEST(macroTest, nul) {
    nint a = 5;
    nint* aPtr = &a;
    nint* aNul = nullptr;

    ASSERT_TRUE(nul(aNul));
    ASSERT_FALSE(nul(aPtr));
}

TEST(macroTest, NM_GETtest) {
    struct A {};

    struct B {
        A* getA() { return &a; }

        A* getNul() { return nullptr; }

        A a;
    } b;

    struct C {
        B* getB() { return &b; }

        B* getNul() { return nullptr; }

        B b;
    } c;

    struct D {
        C* getC() { return &c; }

        C* getNul() { return nullptr; }

        C c;
    } d;

    A* a = b TO(getA());
    ASSERT_TRUE(a);
    A* aNul = b TO(getNul());
    ASSERT_FALSE(aNul);

    A* a0 = b.getNul();
    ASSERT_FALSE(a0);

    A* a2 = c TO(getB()) TO(getA());
    ASSERT_TRUE(a2);
    A* a2Nul = c TO(getNul()) TO(getA());
    ASSERT_FALSE(a2Nul);

    A* a3 = d TO(getC()) TO(getB()) TO(getA());
    ASSERT_TRUE(a3);
    A* a3Nul = d TO(getNul()) TO(getB()) TO(getA());
    ASSERT_FALSE(a3Nul);

    A* a4 = d.getC()->getB() TO(getA());
    ASSERT_TRUE(a4);
    A* a4Nul = d.getC()->getB() TO(getNul());
    ASSERT_FALSE(a4Nul);
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
