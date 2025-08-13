#include <utility>

#include "test/byeolTest.hpp"

using namespace by;

namespace {

    struct myObj: public obj {
        BY(CLASS(myObj, obj))

    public:
        myObj(int newVal = 0): val(newVal) {}

        int val;

        nbool _onSame(const typeProvidable& rhs) const override {
            const myObj& cast = (const myObj&) rhs;
            return val == cast.val;
        }
    };

    class myfunc: public func {
        BY(ME(myfunc, func))

        class myBlock: public blockExpr {
            BY(CLASS(myBlock, blockExpr))

        public:
            str run(const args& a) override {
                BY_I("hello world!");
                _executed = true;

                if(_lambda) _res = _lambda(a, (frames&) by::thread::get().getFrames());
                return str();
            }

            void setLambda(std::function<nbool(const ucontainable&, const frames&)> lambda) {
                _lambda = std::move(lambda);
            }

            std::function<nbool(const ucontainable&, const frames&)> _lambda;
            nbool _res;
            nbool _executed;
        };

    public:
        myfunc():
            super(*new modifier(), funcMgdType("myfunc", ttype<me>::get(), params(), false, new nVoid()),
                *new myBlock()) {
            BY_I("myfunc(%s) new", this);
        }

        ~myfunc() override { BY_I("myfunc(%s) delete", this); }

        nbool isRun() const { return getBlock().cast<myBlock>()->_executed; }

        void setLambda(std::function<nbool(const ucontainable&, const frames&)> lambda) {
            getBlock().cast<myBlock>()->_lambda = std::move(lambda);
        }

        nbool isSuccess() const { return getBlock().cast<myBlock>()->_res; }
    };
} // namespace

struct immutableTest: public byeolTest {
    frames& getFrames() { return (frames&) thread::get().getFrames(); }

    void SetUp() override {
        byeolTest::SetUp();
        getFrames().add(new frame());
    }

    void TearDown() override {
        thread::get().rel();
        byeolTest::TearDown();
    }
};

TEST_F(immutableTest, testImmutablePositive) {
    tstr<nFlt> r1(new nFlt(1.0f));
    str r2 = r1;
    ASSERT_TRUE(r1);
    ASSERT_TRUE(r2);
    ASSERT_EQ(*r1, *r2);

    r1->get() = 0.5f;
    ASSERT_EQ(*r1, *r2);

    param r3("", *r1);
    ASSERT_EQ(r3.getOrigin().getType(), ttype<nFlt>::get());

    scope s;
    s.add("r1", *r1);
    const nFlt& cast = s["r1"].cast<nFlt>() OR_ASSERT(cast);
    ASSERT_NE(r1.get(), &cast);
    ASSERT_EQ(*r1, cast);
}

TEST_F(immutableTest, testImmutableNegative) {
    str r1(new myObj(1));
    str r2 = r1;
    ASSERT_TRUE(r1);
    ASSERT_TRUE(r2);
    ASSERT_EQ(*r1, *r2);

    r1->cast<myObj>()->val = 2;
    ASSERT_EQ(*r1, *r2);

    nmap m;
    m.add("r1", *r1);
    const myObj& cast = m["r1"].cast<myObj>() OR_ASSERT(cast);
    ASSERT_EQ(r1.get(), &cast);
    ASSERT_EQ(r1->cast<myObj>()->val, cast.val);
}

TEST_F(immutableTest, testFrameImmutability) {
    myObj o1;
    o1.subs().add("age", new nInt(18));
    node& age = o1.subs().begin().getVal() OR_ASSERT(age);

    myfunc mf;
    mf.setLambda([&](const ucontainable&, const frames&) {
        frame& fr = (frame*) by::thread::get().getNowFrame() OR.err("there is no frame in this thread").ret(false);
        // test assign:
        auto e = fr.subs().iterate("age");
        if(e.isEnd()) return BY_E("there is no key"), false;

        fr.addLocal("age1", age);

        nInt& age1 = fr.sub<nInt>("age1") OR.err("couldn't find age1").ret(false);
        if(age1 != age) return BY_E("age1 != age"), false;
        if(&age1 == &age) return BY_E("address of age1 and age are different"), false;

        return true;
    });

    o1.subs().add("foo", mf);
    o1.run("foo", narr());
    ASSERT_TRUE(mf.isSuccess());
}
