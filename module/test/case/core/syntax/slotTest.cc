#include <utility>

#include "test/namuSyntaxTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct slotTest: public namuSyntaxTest {};

    class myfunc: public func {
        NM(ME(myfunc, func), CLONE(myfunc))

        class myBlock: public blockExpr {
            NM(CLASS(myBlock, blockExpr))

        public:
            str run(const args& a) override {
                NM_I("hello world!");
                _executed = true;

                if(_lambda) _res = _lambda(a, (frames&) nm::thread::get().getFrames());
                return str();
            }

            void setLambda(function<nbool(const ucontainable&, const frames&)> lambda) {
                _lambda = std::move(lambda);
            }

            function<nbool(const ucontainable&, const frames&)> _lambda;
            nbool _res;
            nbool _executed;
        };

    public:
        myfunc():
            super(*new modifier(),
                funcMgdType("myfunc", ttype<me>::get(), params(), false, *new nVoid()),
                *new myBlock()) {
            NM_I("myfunc(%s) new", this);
        }

        ~myfunc() override { NM_I("myfunc(%s) delete", this); }

        nbool isRun() const { return getBlock().cast<myBlock>()._executed; }

        void setLambda(function<nbool(const ucontainable&, const frames&)> lambda) {
            getBlock().cast<myBlock>()._lambda = std::move(lambda);
        }

        nbool isSuccess() const { return getBlock().cast<myBlock>()._res; }
    };

    class nativeFunc: public baseFunc {
        NM(ME(nativeFunc, baseFunc), CLONE(nativeFunc))

    public:
        nativeFunc(): super(*new modifier) {}

    public:
        nbool isRun() const { return _executed; }

        void setLambda(function<nbool(const ucontainable&, const frames&)> lambda) {
            _lambda = std::move(lambda);
        }

        const ntype& getType() const override {
            static mgdType inner = typeMaker::make<me>(params(), nVoid::singleton());
            return inner;
        }

        nbool isSuccess() const { return _res; }

        const baseObj& getOrigin() const override {
            static obj inner(scope{
                {"nativeFunc", new nativeFunc()}
            });
            return inner;
        }

        str run(const args& a) override {
            _executed = true;
            _res = _lambda(a, (frames&) nm::thread::get().getFrames());
            return str();
        }

    private:
        function<nbool(const ucontainable&, const frames&)> _lambda;
        nbool _res;
        nbool _executed;
    };
} // namespace

TEST_F(slotTest, parsePackTest) {
    make(manifest("demo"))
        .parse(R"SRC(
pack demo
    )SRC")
        .shouldParsed(true);
    ASSERT_FALSE(nul(getSubPack()));
    ASSERT_FALSE(nul(getSlot().subs()));
    auto& shares = (scope::super&) getSlot().subs().getNext().getContainer();
    ASSERT_FALSE(nul(shares));
    ASSERT_EQ(shares.len(), 2);
    ASSERT_EQ(getSlot().getManifest().name, "demo");
}

TEST_F(slotTest, slotIsInFrameWhenCallMgdFunc) {
    // check whether pack's subnodes registered into frame when it calls:
    slot testSlot(manifest("demo"));
    myfunc f1;

    params& ps = f1.getParams();
    ps.add(new param("age", new nInt()));
    ps.add(new param("grade", new nFlt()));
    f1.setLambda([](const auto& contain, const auto& sf) {
        const frame& fr = sf[sf.len() - 1] OR.ret(false);

        // checks slot is in frame:
        const myfunc& cast = fr.sub<myfunc>("foo", narr(*new nInt(), *new nFlt())) OR.ret(false);
        const params& ps = cast.getParams() OR.ret(false);
        if(ps.len() != 2) return false;
        if(ps[0].getOrigin().getType() != ttype<nInt>()) return false;
        if(ps[1].getName() != "grade") return false;

        // checks args of funcs is in frame:
        const nInt& age = fr.sub<nInt>("age") OR.ret(false);
        if(age.cast<int>() != 1) return false;

        const nFlt& grade = fr.sub("grade").cast<nFlt>() OR.ret(false);
        if(grade.get() < 3.4f || grade.get() > 3.6f) return false;

        return true;
    });

    testSlot.subs().add("foo", f1);
    testSlot.run("foo", narr(*new nInt(1), *new nFlt(3.5f)));
    ASSERT_TRUE(f1.isRun());
    ASSERT_TRUE(f1.isSuccess());
}

/* Concept changed: now, native call also make a frame instance.
 * TEST_F(slotTest, slotIsNotInFrameWhenCallNativeFunc) {
    // check whether slot's subnodes not registered into frame when it calls:
    slot testPack(manifest("demo"), packLoadings());
    nativeFunc f1;
    params& ps = f1.getParams();
    ps.add(new param("age", ttype<nInt>::get()));
    ps.add(new param("grade", ttype<nFlt>::get()));
    f1.setLambda([](const auto& contain, const auto& sf) {
        const frame& fr = sf[sf.len() - 1];
        if(!nul(fr)) return NM_E("fr == null"), false;

        return true;
    });
    testPack.subs().add("foo", f1);

    testPack.run("foo", *new narr(nInt(1), *new nFlt(3.5f)));
    ASSERT_TRUE(f1.isRun());
    ASSERT_TRUE(f1.isSuccess());
}*/
