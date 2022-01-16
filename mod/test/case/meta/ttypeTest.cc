#include "../../common/dep.hpp"

using namespace wrd;

TEST(ttypeTest, initSystem) {}

class myClass {
    WRD_INIT_META(myClass);
};

struct myDerivedClass : public myClass {
    WRD_INIT_META(myDerivedClass);
    typedef myClass super;
};

TEST(ttypeTest, basicBehavior) {
    ASSERT_FALSE(ttype<myClass>().isTemplate());
    ASSERT_FALSE(ttype<myClass>().isAbstract());

    const type& type = ttype<myClass>();
    EXPECT_STREQ(type.getName().c_str(), "myClass");

    const types& subs = type.getSubs();
    ASSERT_EQ(subs.size(), 2);
    ASSERT_EQ(*subs[0], ttype<myDerivedClass>::get());

    ASSERT_STREQ(type.getSuper().getName().c_str(), "adam");
}

TEST(ttypeTest, customTypeInheritTest) {
    static const std::string trg = "injected";
    static const int fooRet = 22;

    struct A {};
    struct customA {
        typedef struct myType : type {
            int foo() const {
                return fooRet;
            }
            const std::string& getName() const {
                return trg;
            }
        } metaType;
    };

    EXPECT_NE(ttype<A>().getName(), ttype<customA>().getName());
    EXPECT_EQ(ttype<customA>::get().foo(), fooRet);
}


struct A {
    WRD_INIT_META(A);

    A(): value(true) {}
    wbool value;
};
TEST(ttypeTest, makeInstanceTest) {
    ttype<A> type;
    A* arr[] = {(A*) type.make(), type.makeAs<A>()};
    ASSERT_TRUE(arr[0]);
    ASSERT_TRUE(arr[1]);
    ASSERT_NE(arr[0], arr[1]);
}

struct B {
    WRD_INIT_META(B);

    B(wbool newValue): value(newValue) {}
    wbool value;
};
TEST(ttypeTest, makeInstanceNegativeTest) {
    ttype<B> type;
    ASSERT_FALSE(type.make());
}

struct myDerivedClass2 : public myClass {
    WRD_INIT_META(myDerivedClass2);
    typedef myClass super;
};

struct myDerivedClass3 : public myDerivedClass {
    WRD_INIT_META(myDerivedClass3);
    typedef myDerivedClass super;
};

TEST(ttypeTest, iterateLeafClassTest) {
    const type& typ = ttype<myClass>();
    ASSERT_EQ(typ.getSubs().size(), 2);

    const types& subs = typ.getLeafs();
    ASSERT_FALSE(nul(subs));

    // hierarchy:
    //  myClass -------> myDerivedClass ----> "myDerivedClass3"
    //             |---> "myDerivedClass2"
    //  we should get 2 classes here as leaf class.
    ASSERT_TRUE(*subs[0] == ttype<myDerivedClass3>());
    ASSERT_TRUE(*subs[1] == ttype<myDerivedClass2>());
}
