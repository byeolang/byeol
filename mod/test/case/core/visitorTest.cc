#include "test/namuSyntaxTest.hpp"

using namespace nm;
using namespace std;

struct visitorTest: public namuSyntaxTest {};

TEST_F(visitorTest, iterateManuallyConstructedNodes) {
    obj o;
    o.subs().add("foo1", new func(*new modifier(), typeMaker::make<func>(params(), *new nInt())));
    o.subs().add("val1", new nInt());

    obj o2;
    o2.subs().add("foo2", new func(*new modifier(), typeMaker::make<func>(params(), *new nFlt())));
    o2.subs().add("val2", new nFlt());
    o2.subs().add("o", o);

    struct myVisitor: public visitor {
        nbool metFoo1;
        nbool metVal1;
        nbool metFoo2;
        nbool metVal2;

        myVisitor(): metFoo1(false), metVal1(false), metFoo2(false), metVal2(false) {}

        nbool onVisit(const visitInfo& i, func& fun, nbool) override {
            if(i.name == "foo1") metFoo1 = true;
            if(i.name == "foo2") {
                metFoo2 = true;
                EXPECT_TRUE(fun.getRet().isSub<nFlt>());
            }
            return true;
        }

        nbool onVisit(const visitInfo& i, nInt& o, nbool) override {
            if(i.name == "val1") metVal1 = true;
            return true;
        }

        nbool onVisit(const visitInfo& i, nFlt& o, nbool) override {
            if(i.name == "val2") metVal2 = true;
            return true;
        }
    };

    myVisitor v;
    ASSERT_FALSE(v.metFoo1);
    ASSERT_FALSE(v.metFoo2);
    ASSERT_FALSE(v.metVal1);
    ASSERT_FALSE(v.metVal2);

    v.setTask(o2).work();

    ASSERT_TRUE(v.metFoo1);
    ASSERT_TRUE(v.metFoo2);
    ASSERT_TRUE(v.metVal1);
    ASSERT_TRUE(v.metVal2);
}

TEST_F(visitorTest, visitComplexExpressions) {
    make()
        .parse(R"SRC(
        def obj
            foo() flt
                5.0

        main() int
            o := obj()
            print(o.foo() as str)
            ret o.foo() as int
    )SRC")
        .shouldVerified(true);

    node& root = getSubPack();
    ASSERT_FALSE(nul(root));

    struct myVisitor: public visitor {
        myVisitor(): metO(0), metAsFlt(0), metFlt5(false) {}

        using visitor::onVisit;

        nbool onVisit(const visitInfo& i, getExpr& got, nbool) override {
            NM_DI("subname=%s", got.getName());
            if(got.getName() == "o") metO++;
            return true;
        }

        nbool onVisit(const visitInfo& i, asExpr& as, nbool) override {
            if(as.getAs().as<node>()->isSub<nInt>()) metAsFlt++;
            return true;
        }

        nbool onVisit(const visitInfo& i, nFlt& f, nbool) override {
            if(f.get() == 5.0f) metFlt5 = true;
            return true;
        }

        nint metO;
        nint metAsFlt;
        nbool metFlt5;
    };

    myVisitor v;
    v.setTask(root).work();
    ASSERT_EQ(v.metO, 2);
    ASSERT_EQ(v.metAsFlt, 1);
    ASSERT_TRUE(v.metFlt5);
}

TEST_F(visitorTest, visitComplexExpressions2Negative) {
    make()
        .negative()
        .parse(R"SRC(
        def obj
            foo(a int) int
                ret 5.0 + a

        main() int
            o := obj()
            res := o.foo() as int
            res = res * 2
            ret res
    )SRC")
        .shouldVerified(false);
}

TEST_F(visitorTest, visitComplexExpressions2) {
    make()
        .parse(R"SRC(
        def obj
            foo(a int) int
                ret 5.0 + a

        main() int
            o := obj()
            res := o.foo(5) as int
            res = res * 2
            ret res
    )SRC")
        .shouldVerified(true);

    node& root = getSubPack();
    ASSERT_FALSE(nul(root));

    struct myVisitor: public visitor {
        myVisitor(): metInt2(false), metRet(false) {}

        using visitor::onVisit;

        nbool onVisit(const visitInfo& i, FBOExpr& fao, nbool) override {
            tstr<nInt> num2 = ((node&) fao.getRight()).as<nInt>() OR.ret(true);

            if(num2->cast<nint>() == 2) metInt2 = true;
            return true;
        }

        nbool onVisit(const visitInfo& i, assignExpr& a, nbool) override {
            getExpr& leftGet = ((node&) a.getLeft()).cast<getExpr>() OR.ret(true);

            if(leftGet.getName() != "res") return true;
            metRet = true;
            return true;
        }

        nbool metInt2;
        nbool metRet;
    };

    myVisitor v;
    v.setTask(root).work();
    ASSERT_TRUE(v.metInt2);
    ASSERT_TRUE(v.metRet);
}
