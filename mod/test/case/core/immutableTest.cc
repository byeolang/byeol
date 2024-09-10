#include <utility>

#include "../../namuTest.hpp"

using namespace nm;

namespace {

    struct myObj: public obj {
        NM(CLASS(myObj, obj))

    public:
        myObj(int newVal = 0): val(newVal) {}

        int val;

        nbool _onSame(const typeProvidable& rhs) const override {
            const myObj& cast = (const myObj&) rhs;
            return val == cast.val;
        }
    };

    class myfunc: public func {
        NM(CLASS(myfunc, func))

        class myBlock: public blockExpr {
            NM(CLASS(myBlock, blockExpr))

        public:
            str run(const args& a) override {
                NM_I("hello world!");
                _executed = true;

                if(_lambda) _res = _lambda(a, (frames&) nm::thread::get().getFrames());
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
        myfunc(): super(*new modifier(), params(), *new nVoid(), *new myBlock()) {
            NM_I("myfunc(%s) new", this);
        }

        ~myfunc() override { NM_I("myfunc(%s) delete", this); }

        nbool isRun() const { return getBlock().cast<myBlock>()._executed; }

        void setLambda(std::function<nbool(const ucontainable&, const frames&)> lambda) {
            getBlock().cast<myBlock>()._lambda = std::move(lambda);
        }

        nbool isSuccess() const { return getBlock().cast<myBlock>()._res; }

        str getRet() const override {
            static str inner(new nVoid());
            return inner;
        }

        using super::getParams;
        params& getParams() override { return _params; }

    private:
        params _params;
    };
} // namespace

struct immutableTest: public namuTest {
    frames& getFrames() { return (frames&) thread::get().getFrames(); }

    void SetUp() override {
        namuTest::SetUp();
        getFrames().add(new frame());
    }

    void TearDown() override {
        thread::get().rel();
        namuTest::TearDown();
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
    ASSERT_FALSE(nul(r3.getOrigin()));
    ASSERT_EQ(r3.getOrigin().getType(), ttype<nFlt>::get());

    scope s;
    s.add("r1", *r1);
    const nFlt& cast = s["r1"].cast<nFlt>();
    ASSERT_FALSE(nul(cast));
    ASSERT_NE(&r1.get(), &cast);
    ASSERT_EQ(*r1, cast);
}

TEST_F(immutableTest, testImmutableNegative) {
    str r1(new myObj(1));
    str r2 = r1;
    ASSERT_TRUE(r1);
    ASSERT_TRUE(r2);
    ASSERT_EQ(*r1, *r2);

    r1->cast<myObj>().val = 2;
    ASSERT_EQ(*r1, *r2);

    nmap m;
    m.add("r1", *r1);
    const myObj& cast = m["r1"].cast<myObj>();
    ASSERT_FALSE(nul(cast));
    ASSERT_EQ(&r1.get(), &cast);
    ASSERT_EQ(r1->cast<myObj>().val, cast.val);
}

TEST_F(immutableTest, testFrameImmutability) {
    myObj o1;
    o1.subs().add("age", new nInt(18));
    node& age = o1.subs().begin().getVal();

    myfunc mf;
    mf.setLambda([&](const ucontainable&, const frames&) {
        frame& fr = (frame&) nm::thread::get().getNowFrame();
        // test assign:
        auto e = fr.subs().iterate("age");
        if(e.isEnd()) return NM_E("there is no key"), false;

        fr.addLocal("age1", age);

        nInt& age1 = fr.sub<nInt>("age1");
        if(age1 != age) return NM_E("age1 != age"), false;
        if(&age1 == &age) return NM_E("address of age1 and age are different"), false;

        return true;
    });

    o1.subs().add("foo", mf);
    o1.run("foo", narr());
    ASSERT_TRUE(mf.isSuccess());
}
