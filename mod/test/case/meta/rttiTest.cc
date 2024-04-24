#include "../../namuTest.hpp"

namespace {
    struct noSuper {
        virtual void abstract() = 0;
        static void staticMethod() {}
    };
    struct hasSuper : public noSuper {
        typedef noSuper super;
        void abstract() {}
    };
}

using namespace namu;

struct rttiTest : public namuTest {};

TEST_F(rttiTest, metaTypeDefTest) {
    ASSERT_FALSE(tifHasSuperTypedef<noSuper>::is);
    ASSERT_TRUE(tifHasSuperTypedef<hasSuper>::is);

    ASSERT_EQ(ttype<tadaptiveSuper<noSuper>::super>(), ttype<adam>());
    ASSERT_EQ(ttype<tadaptiveSuper<hasSuper>::super>(), ttype<noSuper>::get());

    ASSERT_FALSE((tifSub<noSuper, hasSuper>::is));
    ASSERT_TRUE((tifSub<hasSuper, noSuper>::is));

    void* instance = tinstanceMaker<noSuper>::make();
    NAMU_I("instance=%x", instance);
    ASSERT_FALSE(instance);
    instance = tinstanceMaker<hasSuper>::make();
    NAMU_I("instance=%x", instance);
    ASSERT_TRUE(instance);

    ASSERT_FALSE(tifHasSuperTypedef<noSuper>::is);
    ASSERT_TRUE(tifHasSuperTypedef<hasSuper>::is);

    ASSERT_EQ(typeid(tadaptiveSuper<noSuper>::super), typeid(adam));
    ASSERT_EQ(typeid(tadaptiveSuper<hasSuper>::super), typeid(noSuper));
}

TEST_F(rttiTest, superTypeDefTest) {
    struct A {};
    struct customA {
        typedef struct myType : type {
            int foo() {
                return 22;
            }
        } metaType;
    };

    ASSERT_EQ(typeid(tmetaTypeDef<A>::is), typeid(type));
    ASSERT_EQ(typeid(tmetaTypeDef<customA>::is), typeid(customA::myType));
}
