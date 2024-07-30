#include <functional>
#include <utility>

#include "../../namuTest.hpp"

using namespace nm;
using namespace std;

struct funcTest: public namuTest {};

namespace {
    class myfunc: public func {
        NM(CLASS(myfunc, func))

        class myBlock: public blockExpr {
            NM(CLASS(myBlock, blockExpr))

        public:
            str run(const args& a) override {
                if(!canRun(a)) return str();
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
        myfunc(): super(params(), *new nVoid(), *new myBlock()) {
            NM_I("myfunc(%s) new", (void*) this);
        }

        ~myfunc() override { NM_I("myfunc(%s) delete", (void*) this); }

        void setUp() {
            myBlock& blk = getBlock().cast<myBlock>();
            blk._executed = false;
            blk._res = false;
        }

        nbool isRun() const { return getBlock().cast<myBlock>()._executed; }

        void setLambda(function<nbool(const ucontainable&, const frames&)> lambda) {
            getBlock().cast<myBlock>()._lambda = std::move(lambda);
        }

        nbool isSuccess() const { return getBlock().cast<myBlock>()._res; }

        str getRet() const override {
            static str inner(new nVoid());
            return inner;
        }

        const params& getParams() const override { return _params; }

        params& getParams() { return _params; }

    private:
        params _params;
    };

    nbool _isFrameLinkScope(const frame& fr, const scope& subs) {
        for(const scope* e = &fr.subs().cast<scope>(); e; e = (scope*) &e->getNext())
            if(e == &subs) return true;
        return false;
    }

    nbool checkFrameHasfuncAndObjScope(const frame& fr, const baseFunc& func,
        const std::string& name, const obj& obj, const char* funcNames[], int funcNameSize) {
        if(nul(fr)) return false;

        int n = 0;
        NM_I("fr.len=%d", fr.subs().len());
        for(auto e = fr.subs().begin(); e; e++)
            NM_I(" - func(\"%s\") calls: fr[%d]=%s", e.getKey(), n++, e.getVal());

        const scope& funcScope = fr.subs().cast<scope>();
        if(nul(funcScope)) return NM_I("nul(funcScope)"), false;
        if(!_isFrameLinkScope(fr, funcScope))
            return NM_I("frame not contain the funcScope(%s)", (void*) &funcScope), false;

        for(int n = 0; n < funcNameSize; n++) {
            const char* name = funcNames[n];
            if(!fr.subAll(name).isMatched())
                return NM_I("fr.sub(%s) is 0 or +2 founds", name), false;
        }

        if(!fr.subAll(name).isMatched())
            return NM_I("couldn't find %s func on frame(%s)", name, (void*) &fr), false;

        return true;
    }

    struct myObj: public obj {
        NM(CLASS(myObj, obj))
    };

    const char* func1Name = "obj1func1";
    const char* func2Name = "obj1func2";
} // namespace

TEST_F(funcTest, testfuncConstructNewFrame) {
    myObj obj;
    const char* funcNames[] = {"test"};
    myfunc func;

    obj.subs().add(funcNames[0], func);
    NM_I("obj.len=%d", obj.subs().len());
    int n = 0;
    for(auto e = obj.subs().begin(); e; e++)
        NM_I(" - fr[%d]=%s", n++, e);

    func.setLambda([&](const auto& a, const auto& sf) {
        if(sf.len() != 1) return false;

        return checkFrameHasfuncAndObjScope(sf[0], func, funcNames[0], obj, funcNames, 1);
    });

    ASSERT_EQ(nm::thread::get().getFrames().len(), 0);
    obj.run(funcNames[0], narr());
    ASSERT_EQ(nm::thread::get().getFrames().len(), 0);
    ASSERT_TRUE(func.isRun());
    ASSERT_TRUE(func.isSuccess());

    ASSERT_EQ(nm::thread::get().getFrames().len(), 0);
    obj.run(funcNames[0]);
    ASSERT_EQ(nm::thread::get().getFrames().len(), 0);
    ASSERT_TRUE(func.isRun());
    ASSERT_TRUE(func.isSuccess());
    func.setLambda(nullptr);
}

TEST_F(funcTest, testCallfuncInsidefunc) {
    myObj obj1;
    myfunc obj1func1;
    myfunc obj1func2;
    obj1.subs().add(func1Name, obj1func1);
    obj1.subs().add(func2Name, obj1func2);
    const char* obj1FuncNames[] = {func1Name, func2Name};

    myObj obj2;
    myfunc obj2func1;
    obj2func1.getParams().add(new param("obj2", obj2));
    const char* obj2FuncNames[] = {"obj2func1"};
    obj2.subs().add("obj2func1", obj2func1);

    obj1func1.setLambda([&](const auto& a, const auto& sf) {
        if(sf.len() != 1) return NM_I("%s: sf.len() != 1", func1Name), false;
        if(!checkFrameHasfuncAndObjScope(sf[0], obj1func1, func1Name, obj1, obj1FuncNames, 2))
            return false;

        narr funcArgs;
        obj1.run(func2Name, funcArgs);
        if(sf.len() != 1) return NM_I("return of %s: sf.len() != 1", func1Name), false;
        return true;
    });
    obj1func2.setLambda([&](const auto& a, const auto& sf) {
        if(sf.len() != 2) return NM_I("%s: sf.len(%d) > 2", func2Name, sf.len()), false;

        if(!checkFrameHasfuncAndObjScope(sf[1], obj1func2, func2Name, obj1, obj1FuncNames, 2))
            return false;

        args funcArgs;
        funcArgs.add(obj2);
        funcArgs.setMe(obj2);
        obj2.run(obj2FuncNames[0], funcArgs);
        if(sf.len() != 2) return NM_I("return of %s: sf.len() != 2", func2Name), false;
        return true;
    });
    obj2func1.setLambda([&](const auto& a, const auto& sf) {
        if(sf.len() != 3) return false;

        if(!checkFrameHasfuncAndObjScope(sf[2], obj2func1, "obj2func1", obj2, obj2FuncNames, 1))
            return false;
        return true;
    });

    args a(obj1);
    ASSERT_EQ(nm::thread::get().getFrames().len(), 0);
    obj1.run(func1Name, a);
    ASSERT_EQ(nm::thread::get().getFrames().len(), 0);
    ASSERT_TRUE(obj1func1.isSuccess());

    obj2func1.setUp();
    obj2.run(func2Name);
    ASSERT_EQ(nm::thread::get().getFrames().len(), 0);
    ASSERT_FALSE(obj2func1.isSuccess());
    obj2func1.setUp();
    obj2.run("obj2func1", narr(obj2));
    ASSERT_EQ(nm::thread::get().getFrames().len(), 0);
    ASSERT_FALSE(obj2func1.isSuccess());
}

TEST_F(funcTest, testfuncHasStrParameter) {
    // prepare:
    std::string expectVal = "hello world!";
    myfunc func1;
    myObj obj;
    obj.subs().add("myfunc", func1);

    params& types = func1.getParams();
    types.add(new param("", new nStr()));
    func1.setLambda([&](const auto& a, const frames& sf) { return true; });

    args a;
    a.add(new nStr(expectVal));
    auto e = a.iterate(1);

    func1.run(a);
    ASSERT_FALSE(func1.isSuccess());

    obj.run("myfunc", narr());
    ASSERT_FALSE(func1.isSuccess());

    obj.run("myfunc", a);
    ASSERT_TRUE(func1.isSuccess());
}

TEST_F(funcTest, testArgsAttachedName) {
    myObj o;
    myfunc f;
    o.subs().add("myfunc", f);
    params& ps = f.getParams();
    ps.add(new param("msg", new nStr()));
    ps.add(new param("age", new nInt()));
    f.setLambda([&](const auto& a, const frames& sf) {
        const frame& fr = sf[sf.len() - 1];
        return fr["msg"].cast<nStr>().get() == "hello world" && fr["age"].cast<nInt>().get() == 55;
    });

    o.run("myfunc");
    ASSERT_FALSE(f.isRun());

    nStr msg("hello world");
    nInt age(55);
    args a;
    a.add(msg);
    a.add(age);

    o.run("myfunc", a);
    ASSERT_TRUE(f.isRun());
    ASSERT_TRUE(f.isSuccess());

    f.setUp();
    ASSERT_FALSE(f.isRun());
    a.rel();
    a.add(o);
    a.add(msg);
    a.add(age);
    o.run("myfunc", a);
    ASSERT_FALSE(f.isRun());
}
