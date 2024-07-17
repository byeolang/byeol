#include "../../common/dep.hpp"
#include <iostream>

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

    struct bridgeCPPTest : public ::testing::Test {
        void TearDown() override {
            kniz::isRun = false;
        }
    };
}

TEST_F(bridgeCPPTest, testNormalWrapping) {
    tstr<tcppBridge<kniz>> bridge(tcppBridge<kniz>::def()
        .func<int, string>("say", &kniz::say));
        // TODO: how to handle void return & void parameter
        //.func<void, void>(&kniz::say);

    node& func = bridge->sub("say");
    ASSERT_FALSE(nul(func));

    bridge->run("say", args(narr{*new nStr("hello native!")}));
    ASSERT_TRUE(kniz::isRun);
}

TEST_F(bridgeCPPTest, testFuncDoesntHaveObjNegative) {
    tstr<tcppBridge<kniz>> bridge(tcppBridge<kniz>::def()
        .func<int, string>("say", &kniz::say));
        // TODO: how to handle void return & void parameter
        //.func<void, void>(&kniz::say);

    args a;
    a.add(*bridge);
    a.add(new nStr("hello native!"));
    bridge->run("say", args(*bridge, narr{*bridge, *new nStr("hello native!")}));
    ASSERT_FALSE(kniz::isRun);

    // bridge set its address to args's setMe()
    bridge->run("say", args(narr{*new nStr("hello native!")}));
    ASSERT_TRUE(kniz::isRun);
}

TEST_F(bridgeCPPTest, testHasName) {
    tstr<obj> bridge(tcppBridge<kniz>::def()
        .func<int, string>("say", &kniz::say));
    nmap m;
    ASSERT_TRUE(bridge);
    m.add("kniz", *bridge);

    baseFunc& say = m["kniz"]["say"].cast<baseFunc>();
    ASSERT_FALSE(nul(say));
}

namespace {
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
        int init(window* win) {
            return win->getY() + win->getX();
        }
    };
}

TEST_F(bridgeCPPTest, passObj) {
    str winBridge(tcppBridge<window>::def()
            .func("getX", &window::getX)
            .func("getY", &window::getY)
            .func("setY", &window::setY));
    str winOpenGL(tcppBridge<openGL>::def()
            .func("init", &openGL::init));

    winBridge->run("setY", args{ narr{*new nInt(20)}});
    str res = winOpenGL->run("init", args{ narr{*winBridge}});
    ASSERT_TRUE(res);
    ASSERT_EQ(res->cast<nint>(), 25);
}

TEST_F(bridgeCPPTest, returnObj) {
    str winBridge(tcppBridge<window>::def()
            .func("getX", &window::getX)
            .func("getY", &window::getY)
            .func("setY", &window::setY)
            .func("new1", &window::new1));
    str winOpenGL(tcppBridge<openGL>::def()
            .func("init", &openGL::init));

    str newWin = winBridge->run("new1", args{ narr{*new nInt(15)}});
    str res = winOpenGL->run("init", args{ narr{*newWin}});
    ASSERT_TRUE(res);
    ASSERT_EQ(res->cast<nint>(), 20);
}

namespace {
    struct windowManager {
        void add(narr& wins) {
            _wins.add(wins);
        }
        void del(int n) {
            _wins.del(n);
        }

        narr _wins;
    };
}

TEST_F(bridgeCPPTest, passArray) {
    str mgrBridge(tcppBridge<windowManager>::def()
            .func("add", &windowManager::add)
            .func("del", &windowManager::del));

    tstr<tcppBridge<narr>> narrBridge(tcppBridge<narr>::def());
    narrBridge->get().add(*new nInt(0));
    narrBridge->get().add(*new nInt(1));
    narrBridge->get().add(*new nInt(2));
    mgrBridge->run("add", args{narr{*narrBridge}});
    mgrBridge->run("del", args{narr{*new nInt(1)}}); // 0, 2 remains.

    const narr& res = mgrBridge->cast<tcppBridge<windowManager>>().get()._wins;
    ASSERT_FALSE(nul(res));
    ASSERT_EQ(res.len(), 2);
    ASSERT_EQ(res[0].cast<nint>(), 0);
    ASSERT_EQ(res[1].cast<nint>(), 2);
}

namespace {
    struct myObj : public obj {
        NM(CLASS(myObj, obj))

    public:
        myObj(): age(0) {}
        myObj(int newAge): age(newAge) {}

        int age;
    };

    struct stage {
        int foo(myObj& o) {
            return o.age;
        }
    };
}

TEST_F(bridgeCPPTest, passRawObj) {
    myObj o1;
    o1.age = 5;

    str stg(tcppBridge<stage>::def()
        .func("foo", &stage::foo));
    str res = stg->run("foo", args{narr{o1}});
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 5);
}

namespace {
    struct yourObj : public obj {
        NM(CLASS(yourObj, obj))
    };

    struct testObj {
        int len = 0;
        tstr<arr> _arr;

        testObj() {}

        int updateLen(tarr<myObj> a) { // NOLINT: to test whether bridge can support byval parameter.
            len = a.len();
            _arr.bind((tarr<myObj>*)a.clone());
            return len;
        }

        int sumOfLen() const {
            int ret = 0;
            for(const auto& elem : *_arr)
                ret += elem.cast<myObj>().age;
            return ret;
        }
    };
}

TEST_F(bridgeCPPTest, passArr) {
    myObj obj;
    arr a(obj);
    a.add(new myObj(1));
    a.add(new myObj(2));
    str yourobj(new yourObj());
    ASSERT_EQ(a.add(*yourobj), false);
    a.add(new myObj(3));
    ASSERT_EQ(a.len(), 3);

    str testobj(tcppBridge<testObj>::def()
        .func("updateLen", &testObj::updateLen)
        .func("sumOfLen", &testObj::sumOfLen));
    str res = testobj->run("updateLen", args{narr{a}});
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 3);

    res = testobj->run("sumOfLen");
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 6);
}

namespace {
    struct A : public baseObj {
        NM(CLASS(A, baseObj))

    public:
        nint foo(nint a) {
            return age + a;
        }

        scope& subs() override { return org->subs(); }

        const baseObj& getOrigin() const override { return *org; }

        static tstr<obj> org;
        nint age;
    };
    tstr<obj> A::org;
}

TEST_F(bridgeCPPTest, baseObjWithBridgeOrigin) {
    tstr<obj> origin(tcppBridge<A>::def()
                     .func("foo", &A::foo));
    A::org = origin;

    A a1, a2;
    a1.age = 1;
    a2.age = 2;
    ASSERT_EQ(a1.foo(1), 2);
    ASSERT_EQ(a2.foo(1), 3);

    auto& foo = origin->sub<baseFunc>("foo");
    ASSERT_FALSE(nul(foo));
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
