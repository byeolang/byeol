#include <utility>

#include "test/byeolTest.hpp"

using namespace by;
using namespace std;

struct nodeTest: public byeolTest {};

namespace {
    class myFunc: public func {
        BY(ME(myFunc, func))

    public:
        myFunc(): super(*new modifier(), funcMgdType("myFunc", ttype<me>::get(), params(), false, new nVoid())) {}

        void setUp() { _executed = false; }

        str run(const args& a) override {
            BY_I("hello world!");
            _executed = true;
            return str();
        }

        nbool isRun() const { return _executed; }

    private:
        nbool _executed;
    };

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

    class food: public obj {
        BY(CLASS(food, obj))

    public:
        food(string newName, int newCalorie): name(std::move(newName)), calorie(newCalorie) {}

        string name;
        int calorie;
    };

    class chef: public obj {
        struct tofood: public tas<food> {
            BY(CLASS(tofood, tas<food>))

        public:
            str as(const node& it, const type& to) const override {
                const chef& chef1 = it.cast<chef>() OR.ret(str());
                return str(new food(chef1.foodName, chef1.foodCalorie));
            }
        };

        class myType: public ntype {
            BY_ME(myType, ntype)

        protected:
            const impliAses& _getImpliAses() const override {
                static impliAses* inner = nullptr;
                if(inner) return *inner;

                inner = new impliAses();
                inner->add(new tofood());
                return *inner;
            }
        };
        BY(CLASS(chef, obj, myType))

    public:
        string foodName;
        int foodCalorie;
    };
} // namespace

TEST_F(nodeTest, testManuallyMakeNodeStructure) {
    // prepare:
    tstr<scope> frameEmulator;
    myObj obj;
    myFunc func;
    BY_DI("func.tag.chkId=%d", func.getBindTag()->getId().chkN);

    obj.subs().add("myFunc", func);
    myFunc funcOffunc;
    func.subs().add("funcOfFunc", funcOffunc);

    BY_DI("func.tag.chkId=%d", func.getBindTag()->getId().chkN);
    BY_DI("funcOffunc.tag.chkId=%d", funcOffunc.getBindTag()->getId().chkN);

    // when:
    frameEmulator.bind(obj.subs());
    //  push another:
    scope* chnOffunc = scope::wrap<scope>(func.subs());
    chnOffunc->link(*frameEmulator);
    frameEmulator.bind(*chnOffunc);

    BY_DI("func.tag.chkId=%d", func.getBindTag()->getId().chkN);
    BY_DI("funcOffunc.tag.chkId=%d", funcOffunc.getBindTag()->getId().chkN);

    // then:
    ASSERT_EQ(chnOffunc->len(), 2);
    ASSERT_EQ(chnOffunc->len(), 2);
    ASSERT_EQ(chnOffunc->len(), 2);

    int n = 0;
    for(const auto& elem: *chnOffunc)
        BY_I("[%d]=%s", n++, elem);

    BY_DI("func.tag.chkId=%d", func.getBindTag()->getId().chkN);
    BY_DI("funcOffunc.tag.chkId=%d", funcOffunc.getBindTag()->getId().chkN);

    ASSERT_EQ(n, 2);
    BY_DI("func.tag.chkId=%d", func.getBindTag()->getId().chkN);
    BY_DI("funcOffunc.tag.chkId=%d", funcOffunc.getBindTag()->getId().chkN);
}

TEST_F(nodeTest, testManualNativefuncCall) {
    // prepare:
    myFunc func;

    myObj obj;
    obj.subs().add("myFunc", func);

    args a;
    auto subs = ((const myObj&) obj).subAll("myFunc", &a);
    ASSERT_EQ(subs.len(), 1);
    ASSERT_TRUE(subs[0].canRun(a));

    // when:
    func.setUp();
    subs[0].run(a);
    ASSERT_TRUE(func.isRun());
}

TEST_F(nodeTest, testchefImplicitCastTofood) {
    // prepare:
    const string expectName = "HealthPotion";
    const int expectCalorie = 350;
    chef chef;
    chef.foodName = expectName;
    chef.foodCalorie = expectCalorie;

    const ntype& chefType = chef.getType();
    ASSERT_TRUE(chefType.isImpli<food>());
    tstr<food> cast = chef.asImpli<food>();
    ASSERT_TRUE(cast);

    EXPECT_EQ(cast->name, expectName);
    EXPECT_EQ(cast->calorie, expectCalorie);
}

TEST_F(nodeTest, ShouldNotSameNameVariableIntoSubs) {
    chef c;
    ASSERT_EQ(c.subs().len(), 0);
    c.subs().add("age", new nInt(22));
    c.subs().add(baseObj::CTOR_NAME, new defaultCtor(c));

    ASSERT_EQ(c.subs().len(), 2);
    ASSERT_EQ(c.sub<nInt>("age")->get(), 22);
    errReport rpt;
    verifier v;
    v.setReport(rpt).setTask(c).work();
    ASSERT_FALSE(rpt);

    c.subs().add("age1", new nInt(22));
    ASSERT_EQ(c.subs().len(), 3);
    auto matches = c.subAll<nInt>("age1");
    ASSERT_TRUE(matches.isMatched());
    ASSERT_EQ(matches.get()->get(), 22);

    v.setTask(c).work();
    ASSERT_FALSE(rpt);

    c.subs().add("age", new nInt(23));
    ASSERT_EQ(c.subs().len(), 4);
    ASSERT_EQ(c.subAll<nInt>("age").len(), 2);
    v.setTask(c).work();
    ASSERT_TRUE(rpt);
}
