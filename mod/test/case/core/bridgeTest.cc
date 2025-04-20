#include <iostream>

#include "test/common/dep.hpp"

using namespace nm;
using namespace std;

namespace {
    struct kniz {
        void say() {
            cout << "I'm kniz. have lived in Korea.\n";
            isRun = true;
        }

        int say(string msg) { // NOLINT
            isRun = true;
            return 0;
        }

        bool sayCharPtr(const char* msg) {
            cout << "Oh, it's charactor pointer. Of course I can say it without problem like '"
                 << msg << "'. were you suprise?\n";
            return isRun = true;
        }

        static inline nbool isRun = false;
    };

    struct window {
        int getX() { return 5; }

        int getY() { return _y; }

        void setY(int newY) { _y = newY; }

        window& new1(int newY) {
            window& ret = *new window();
            ret._y = newY;
            return ret;
        }

        int _y;
    };

    struct openGL {
        int init(window* win) { return win->getY() + win->getX(); }
    };
} // namespace

struct bridgeTest: public ::testing::Test {
    void SetUp() override {
        static nbool isFirst = true;
        if(isFirst) {
            tbridger<kniz>::ctor()
                .ctor<kniz>()
                .func("sayCharPtr", &kniz::sayCharPtr)
                .func<int, string>("say", &kniz::say);
            tbridger<window>::ctor()
                .ctor<window>()
                .func("new1", &window::new1)
                .func("getX", &window::getX)
                .func("getY", &window::getY)
                .func("setY", &window::setY);
            tbridger<openGL>::ctor().ctor<openGL>().func("init", &openGL::init);
            isFirst = false;
        }
    }

    void TearDown() override { kniz::isRun = false; }
};

TEST_F(bridgeTest, makeAndReferScopeDoesLeakMemory) {
    scope& inner = tbridger<kniz>::subs();
    // tbridger object released.
    // but does scope still bridge funcs?
    ASSERT_EQ(inner.len(), 4);
    ASSERT_FALSE(nul(inner.get<baseFunc>("sayCharPtr")));

    {
        str b = tbridger<kniz>::make(new kniz());
        ASSERT_EQ(b->subs().len(), 4);

        kniz::isRun = false;
        b->run("say", narr{nStr("hello")});
        ASSERT_EQ(kniz::isRun, true);

        str new1 = b->run();
        ASSERT_TRUE(new1);
        tstr<tbridge<kniz>> cast = new1;
        ASSERT_TRUE(cast);
        ASSERT_EQ(cast->get().isRun, true);
    }
}

TEST_F(bridgeTest, testNormalWrapping) {
    tstr<tbridge<kniz>> bridge(tbridger<kniz>::make(new kniz()));
    // TODO: how to handle void return & void parameter
    //.func<void, void>(&kniz::say);

    node& func = bridge->sub("say");
    ASSERT_FALSE(nul(func));

    kniz::isRun = false;
    bridge->run("say", narr{nStr("hello native!")});
    ASSERT_TRUE(kniz::isRun);
}

TEST_F(bridgeTest, testHasName) {
    tstr<baseObj> bridge(tbridger<kniz>::make(new kniz()));
    nmap m;
    ASSERT_TRUE(bridge);
    m.add("kniz", *bridge);

    baseFunc& say = m["kniz"]["say"].cast<baseFunc>();
    ASSERT_FALSE(nul(say));
}

TEST_F(bridgeTest, passObj) {
    str winBridge(tbridger<window>::make(new window()));
    str winOpenGL(tbridger<openGL>::make(new openGL()));

    winBridge->run("setY", args{narr{*new nInt(20)}});
    str res = winBridge->run("getY");
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 20);

    res = winOpenGL->run("init", args{narr{*winBridge}});
    ASSERT_TRUE(res);
    ASSERT_EQ(res->cast<nint>(), 25);
}

TEST_F(bridgeTest, returnObj) {
    str winBridge(tbridger<window>::make(new window()));
    str winOpenGL(tbridger<openGL>::make(new openGL()));

    str newWin = winBridge->run("new1", args{narr{*new nInt(15)}});
    str res = winOpenGL->run("init", args{narr{*newWin}});
    ASSERT_TRUE(res);
    ASSERT_EQ(res->cast<nint>(), 20);
}

namespace {
    struct windowManager {
        void add(narr& wins) { _wins.add(wins); }

        void del(int n) { _wins.del(n); }

        narr _wins;
    };
}

TEST_F(bridgeTest, passArray) {
    str mgrBridge(tbridger<windowManager>::ctor()
                      .ctor<windowManager>()
                      .func("add", &windowManager::add)
                      .func("del", &windowManager::del)
                      .make(new windowManager()));

    arr a(*new nInt(0));
    a.subs(); // for bridging narr.
    tstr<tbridge<narr>> narrBridge(tbridger<narr>::make(new narr()));

    narrBridge->get().add(*new nInt(0)); // call func directly.
    narrBridge->get().add(*new nInt(1));
    narrBridge->get().add(*new nInt(2));
    mgrBridge->run("add", args{narr{*narrBridge}});
    mgrBridge->run("del", args{narr{*new nInt(1)}}); // 0, 2 remains.

    const narr& res = mgrBridge->cast<tbridge<windowManager>>().get()._wins;
    ASSERT_FALSE(nul(res));
    ASSERT_EQ(res.len(), 2);
    ASSERT_EQ(res[0].cast<nint>(), 0);
    ASSERT_EQ(res[1].cast<nint>(), 2);
}

namespace {
    struct myObj: public obj {
        NM(CLASS(myObj, obj))

    public:
        myObj(): age(0) {}

        myObj(int newAge): age(newAge) {}

        int age;
    };

    struct stage {
        int foo(myObj& o) { return o.age; }
    };
}

TEST_F(bridgeTest, passRawObj) {
    myObj o1;
    o1.age = 5;

    str stg(tbridger<stage>::ctor().ctor<stage>().func("foo", &stage::foo).make(new stage()));
    str res = stg->run("foo", args{narr{o1}});
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 5);
}

namespace {
    struct yourObj: public obj {
        NM(CLASS(yourObj, obj))
    };

    struct testObj {
        int len = 0;
        tstr<arr> _arr;

        testObj() {}

        int updateLen(
            tarr<myObj> a) { // NOLINT: to test whether bridge can support byval parameter.
            len = a.len();
            _arr.bind((tarr<myObj>*) a.clone());
            return len;
        }

        int sumOfLen() const {
            int ret = 0;
            for(const auto& elem: *_arr)
                ret += elem.cast<myObj>().age;
            return ret;
        }
    };
}

TEST_F(bridgeTest, passArr) {
    myObj obj;
    arr a(obj);
    a.add(new myObj(1));
    a.add(new myObj(2));
    str yourobj(new yourObj());
    ASSERT_EQ(a.add(*yourobj), false);
    a.add(new myObj(3));
    ASSERT_EQ(a.len(), 3);

    str testobj(tbridger<testObj>::ctor()
                    .ctor<testObj>()
                    .func("updateLen", &testObj::updateLen)
                    .func("sumOfLen", &testObj::sumOfLen)
                    .make(new testObj()));
    str res = testobj->run("updateLen", args{narr{a}});
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 3);

    res = testobj->run("sumOfLen");
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 6);
}

namespace {
    struct A: public baseObj {
        NM(CLASS(A, baseObj))

    public:
        nint foo(nint a) { return age + a; }

        using super::subs;

        scope& subs() override {
            static scope inner = tbridger<A>::ctor().ctor<A>().func("foo", &A::foo).subs();

            return inner;
        }

        const baseObj& getOrigin() const override { return *this; }

        nint age;
    };
}

TEST_F(bridgeTest, baseObjWithBridgeOrigin) {
    A a1, a2;
    a1.age = 1;
    a2.age = 2;
    ASSERT_EQ(a1.foo(1), 2);
    ASSERT_EQ(a2.foo(1), 3);

    auto& foo = a1.sub<baseFunc>("foo");
    ASSERT_FALSE(nul(foo));
    auto& foo2 = a2.sub<baseFunc>("foo");
    ASSERT_FALSE(nul(foo2));

    const auto& ps = foo.getParams();
    ASSERT_EQ(ps.len(), 1);
    ASSERT_TRUE(ps[0].getOrigin().isSub<nInt>());

    auto res = a1.run("foo", {narr{nInt(1)}});
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->isSub<nInt>());
    ASSERT_EQ(res->cast<nint>(), 2);

    res = a2.run("foo", {narr{nInt(1)}});
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->isSub<nInt>());
    ASSERT_EQ(res->cast<nint>(), 3);
}

namespace {
    struct B {
        B(int newAge): age(newAge) {}

        int getAge() const { return age; }

        int age;
    };
}

TEST_F(bridgeTest, bridgeWhatDoesntHaveDefaultCtor) {
    tstr<tbridge<B>> bridge = tbridger<B>::ctor<int>().func("getAge", &B::getAge).make(new B(1));
    ASSERT_TRUE(bridge);
    ASSERT_EQ(bridge->get().age, 1);

    tstr<tbridge<B>> res = bridge->run(narr{nInt(5)});
    ASSERT_TRUE(res);

    str age = res->run("getAge");
    ASSERT_TRUE(age);
    ASSERT_EQ(age.cast<nint>(), 5);
}

TEST_F(bridgeTest, accessOrigin) {
    tstr<tbridge<kniz>> bStr = tbridger<kniz>::make(new kniz());
    tbridge<kniz>& b = *bStr;
    ASSERT_NE(&b.getOrigin(), &b);
}
