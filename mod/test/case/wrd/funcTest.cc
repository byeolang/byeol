#include "../../wrdTest.hpp"
#include <functional>

using namespace wrd;
using namespace std;

struct funcTest : public wrdTest {};

namespace {
    class myfunc : public mgdFunc {
        WRD(CLASS(myfunc, mgdFunc))

        class myBlock : public blockExpr {
            WRD(CLASS(myBlock, blockExpr))

        public:
            str run(const ucontainable& args) override {
                if(!canRun(args)) return str();
                WRD_I("hello world!");
                _executed = true;

                if(_lambda)
                    _res = _lambda(args, (frames&) wrd::thread::get().getFrames());
                return str();
            }

            void setLambda(function<wbool(const ucontainable&, const frames&)> lambda) {
                _lambda = lambda;
            }

            function<wbool(const ucontainable&, const frames&)> _lambda;
            wbool _res;
            wbool _executed;
        };

    public:
        myfunc(): super(params(), ttype<node>::get(), *new myBlock()) {
            WRD_I("myfunc(%x) new", this);
        }
        ~myfunc() {
            WRD_I("myfunc(%x) delete", this);
        }

        void setUp() {
            myBlock& blk = getBlock().cast<myBlock>();
            blk._executed = false;
            blk._res = false;
        }

        wbool isRun() const {
            return getBlock().cast<myBlock>()._executed;
        }

        void setLambda(function<wbool(const ucontainable&, const frames&)> lambda) {
            getBlock().cast<myBlock>()._lambda = lambda;
        }

        wbool isSuccess() const {
            return getBlock().cast<myBlock>()._res;
        }

        const wtype& getRetType() const override {
            return ttype<node>::get();
        }

        const params& getParams() const override { return _params; }
        params& getParams() { return _params; }

    private:
        params _params;
    };

    wbool _isFrameLinkScope(const frame& fr, const scopes& subs) {
        for(const scopes* e=&fr.subs().cast<scopes>(); e ; e=&e->getNext())
            if(e == &subs)
                return true;
        return false;
    }

    wbool checkFrameHasfuncAndObjScope(const frame& fr, const func& func, const std::string& name,
        const obj& obj, const char* funcNames[], int funcNameSize) {
        if(nul(fr)) return false;

        int n = 0;
        WRD_I("fr.len=%d", fr.subs().len());
        for(auto e=fr.subs().begin(); e ;e++)
            WRD_I(" - func(\"%s\") calls: fr[%d]=%s", e.getKey().c_str(), n++, e.getVal().getType().getName().c_str());

        const scopes& funcScope = fr.subs().cast<scopes>();
        if(nul(funcScope)) return WRD_I("nul(funcScope)"), false;
        if(!_isFrameLinkScope(fr, funcScope))
            return WRD_I("frame not contain the funcScope(%x)", &funcScope), false;

        for(int n=0; n < funcNameSize; n++) {
            const char* name = funcNames[n];
            if(fr.subAll(name).len() != 1)
                return WRD_I("fr.sub(%s) is 0 or +2 founds", name), false;
        }

        narr foundfunc = fr.subAll(name);
        if(foundfunc.len() != 1)
            return WRD_I("couldn't find %s func on frame(%x)", name.c_str(), &fr), false;

        return true;
    }

    struct myObj : public mgdObj {
        WRD(CLASS(myObj, mgdObj))
    };

    const char* func1Name = "obj1func1";
    const char* func2Name = "obj1func2";
}

TEST_F(funcTest, testfuncConstructNewFrame) {
    myObj obj;
    const char* funcNames[] = {"test"};
    myfunc func;

    obj.subs().add(funcNames[0], func);
    WRD_I("obj.len=%d", obj.subs().len());
    int n = 0;
    for(auto e=obj.subs().begin(); e ;e++) {
        WRD_I(" - fr[%d]=%s", n++, e->getType().getName().c_str());
    }

    narr args;
    args.add(obj);

    func.setLambda([&](const auto& args, const auto& sf) {
        if(sf.len() != 1) return false;

        return checkFrameHasfuncAndObjScope(sf[0], func, funcNames[0], obj, funcNames, 1);
    });

    ASSERT_EQ(wrd::thread::get().getFrames().len(), 0);
    func.run(args);
    ASSERT_EQ(wrd::thread::get().getFrames().len(), 0);
    ASSERT_FALSE(func.isRun());
    ASSERT_FALSE(func.isSuccess());

    ASSERT_EQ(wrd::thread::get().getFrames().len(), 0);
    obj.run(funcNames[0]);
    ASSERT_EQ(wrd::thread::get().getFrames().len(), 0);
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

    obj1func1.setLambda([&](const auto& args, const auto& sf) {
        if(sf.len() != 1) return WRD_I("%s: sf.len() != 1", func1Name), false;
        if(!checkFrameHasfuncAndObjScope(sf[0], obj1func1, func1Name, obj1, obj1FuncNames, 2)) return false;

        narr funcArgs;
        funcArgs.add(obj1);
        obj1func2.run(funcArgs);
        if(sf.len() != 1)
            return WRD_I("return of %s: sf.len() != 1", func1Name), false;
        return true;
    });
    obj1func2.setLambda([&](const auto& args, const auto& sf) {
        if(sf.len() != 2) return WRD_I("%s: sf.len() != 2", func2Name), false;

        if(!checkFrameHasfuncAndObjScope(sf[1], obj1func2, "obj2func1", obj1, obj1FuncNames, 2)) return false;

        narr funcArgs;
        funcArgs.add(obj2);

        obj2func1.run(funcArgs);
        if(sf.len() != 2)
            return WRD_I("return of %s: sf.len() != 2", func2Name), false;
        return true;
    });
    obj2func1.setLambda([&](const auto& args, const auto& sf) {
        if(sf.len() != 3) return false;

        if(!checkFrameHasfuncAndObjScope(sf[2], obj2func1, "obj2func1", obj2, obj2FuncNames, 1)) return false;
        return true;
    });

    narr args;
    args.add(obj1);
    ASSERT_EQ(wrd::thread::get().getFrames().len(), 0);
    obj1func1.run(args);
    ASSERT_EQ(wrd::thread::get().getFrames().len(), 0);
    ASSERT_FALSE(obj1func1.isSuccess());
    obj1.run(func1Name, args);
    ASSERT_EQ(wrd::thread::get().getFrames().len(), 0);
    ASSERT_FALSE(obj1func1.isSuccess());
    obj1.run(func1Name);
    ASSERT_EQ(wrd::thread::get().getFrames().len(), 0);
    ASSERT_TRUE(obj1func1.isSuccess());
}

TEST_F(funcTest, testfuncHasStrParameter) {
    // prepare:
    std::string expectVal = "hello world!";
    myfunc func1;
    myObj obj;
    obj.subs().add("myfunc", func1);

    params& types = func1.getParams();
    types.add(new param("", obj));
    types.add(new param("", ttype<wStr>::get()));
    func1.setLambda([&](const auto& args, const frames& sf) { return true; });

    narr args;
    args.add(obj);
    args.add(new wStr(expectVal));
    auto e = args.iterate(1);

    func1.run(args);
    ASSERT_FALSE(func1.isSuccess());

    obj.run("myfunc", narr());
    ASSERT_FALSE(func1.isSuccess());

    obj.run("myfunc", args);
    ASSERT_TRUE(func1.isSuccess());
}

TEST_F(funcTest, testArgsAttachedName) {
    myObj o;
    myfunc f;
    o.subs().add("myfunc", f);
    params& ps = f.getParams();
    ps.add(new param("msg", ttype<wStr>::get()));
    ps.add(new param("age", ttype<wInt>::get()));
    f.setLambda([&](const auto& args, const frames& sf) {
        const frame& fr = sf[sf.len() - 1];
        return  fr["msg"].cast<wStr>().get() == "hello world" &&
                fr["age"].cast<wInt>().get() == 55;
    });

    o.run("myfunc");
    ASSERT_FALSE(f.isRun());

    narr args;
    wStr msg("hello world");
    args.add(msg);
    wInt age(55);
    args.add(age);

    o.run("myfunc", args);
    ASSERT_TRUE(f.isRun());
    ASSERT_TRUE(f.isSuccess());
}
