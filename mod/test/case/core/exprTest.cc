#include "../../namuTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct helloWorld {
        void main(string msg) { // NOLINT
            isRun = true;
        }

        static inline nbool isRun = false;
    };
}

struct exprTest : public namuTest {
    void SetUp() override;
    void TearDown() override;

    tstr<obj> bridge;

    static void setLine(expr& exp, ncnt row, ncnt col) {
        exp._pos = {row, col};
    }

    static frames& getFrames() {
        return nm::thread::get()._getFrames();
    }
};

void exprTest::SetUp() {
    namuTest::SetUp();

    bridge.bind(tbridger<helloWorld>()
        .ctor().ctor<helloWorld>()
        .func("main", &helloWorld::main).make(new helloWorld()));
}

void exprTest::TearDown() {
    namuTest::TearDown();

    helloWorld::isRun = false;
}

TEST_F(exprTest, standbyHelloWorldBridgeObj) {
    ASSERT_TRUE(bridge.isBind());

    tstr<nStr> msg(new nStr());
    args a(*bridge, narr {msg.get()});

    node& mainFunc = bridge->sub("main", a);
    ASSERT_FALSE(nul(mainFunc));
    ASSERT_TRUE(mainFunc.canRun(a));
    ASSERT_FALSE(mainFunc.canRun(args(*bridge, narr {*bridge, *msg})));
    ASSERT_FALSE(helloWorld::isRun);
    str res = mainFunc.run(a);
    ASSERT_TRUE(res.isBind());
    res = bridge->run("main", a);
    ASSERT_TRUE(res.isBind());
    res = bridge->run("main", args(narr{*msg}));
    ASSERT_TRUE(res.isBind());
    ASSERT_TRUE(res.getType() == ttype<node>::get());
    ASSERT_TRUE(res->getType() == ttype<nVoid>::get());
    ASSERT_TRUE(helloWorld::isRun);
}

TEST_F(exprTest, simpleGetExpr) {
    getExpr exp(bridge.get(), "main", narr(*new nStr()));
    errReport rep;
    verifier veri;
    veri.setReport(rep).setTask(exp).work();
    /*TODO: uncomment this
    ASSERT_TRUE(rep); // should have some errors.*/
    setLine(exp, 1, 1);

    rep.rel();
    ASSERT_FALSE(rep);

    baseFunc& f = bridge->sub<baseFunc>("main");
    ASSERT_FALSE(nul(f));
    veri.setTask(exp).work();
    ASSERT_FALSE(rep);

    str res = exp.run();
    ASSERT_TRUE(res.isBind());
    ASSERT_TRUE(res.getType() == ttype<node>::get());
    ASSERT_TRUE(res->isSub<baseFunc>());
}

TEST_F(exprTest, simpleGetExprNegative) {
    getExpr exp(bridge.get(), "main?", narr(*new nStr()));
    setLine(exp, 1, 1);
    errReport rep;
    verifier veri;
    veri.setReport(rep).setFlag(0).setTask(exp).work();
    ASSERT_TRUE(rep); // should have some errs.

    getExpr exp2(bridge.get(), "main", narr(*new nStr()));
    setLine(exp2, 1, 1);
    rep.rel();
    veri.setTask(exp).work();
    ASSERT_TRUE(rep);
}

TEST_F(exprTest, simpleRunExprWithoutMeObjNegative) {
    runExpr exp1(nulOf<node>(), bridge->sub("main"), narr(*new nStr("kniz!")));
    errReport rep;
    verifier veri;
    veri.setReport(rep).setFlag(0).setTask(exp1).work();
    ASSERT_TRUE(rep);

    setLine(exp1, 1, 1);
    rep.rel();
    veri.setReport(rep).setTask(exp1).work();
    ASSERT_TRUE(rep);
}

TEST_F(exprTest, simpleRunExpr) {
    runExpr exp1(*bridge, *new getExpr("main"), narr(*new nStr("kniz!")));

    frame fr;
    fr.add(*new scope());
    fr.setMe(*bridge);
    getFrames().add(fr);

    errReport rep;
    verifier veri;
    veri.setReport(rep).setTask(exp1).work();

    setLine(exp1, 1, 1);
    rep.rel();
    veri.setReport(rep).setTask(exp1).work();
    ASSERT_FALSE(rep);
    ASSERT_FALSE(helloWorld::isRun);

    str res = exp1.run();
    ASSERT_TRUE(res.isBind());
    ASSERT_TRUE(res.getType() == ttype<node>::get());
    ASSERT_TRUE(res->getType() == ttype<nVoid>::get());

    ASSERT_TRUE(helloWorld::isRun);
    getFrames().del();
}

TEST_F(exprTest, simpleRunExprNegative) {
    runExpr exp1(bridge->sub("main"), narr());
    setLine(exp1, 1, 1);
    errReport rep;
    verifier veri;
    veri.setReport(rep).setTask(exp1).work();
    ASSERT_TRUE(rep);

    ASSERT_FALSE(helloWorld::isRun);

    str res = exp1.run();
    ASSERT_FALSE(res.isBind());
    ASSERT_TRUE(res.getType() == ttype<node>::get());
    ASSERT_FALSE(helloWorld::isRun);
}

TEST_F(exprTest, constructExprInManual) {
    runExpr r(*bridge, *new getExpr("main"), narr(*new nStr("kniz!")));
    setLine(r, 1, 1);

    frame fr;
    fr.add(*new scope());
    fr.setMe(*bridge);
    getFrames().add(fr);

    str res = r.run();
    ASSERT_TRUE(res);
    ASSERT_TRUE(res.getType() == ttype<node>::get());
    ASSERT_TRUE(res->getType() == ttype<nVoid>::get());

    getFrames().del();
}
