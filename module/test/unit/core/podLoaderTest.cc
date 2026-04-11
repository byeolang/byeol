#include "test/byeolTest.hpp"

using namespace by;
using namespace std;

struct podLoaderTest: public byeolTest {};

TEST_F(podLoaderTest, testDefaultLoaderInit) {
    nmap& systemPods = (nmap&) by::thread::get().getPods(); // don't worry for casting. I know what I'm doing >_o
    pod& s = systemPods.get<pod>([](const std::string& name, const pod& e) { return name == "sys"; }) OR_ASSERT(s);

    ASSERT_TRUE(s.subs().len() > 1);             // s get chained to system pods.
    ASSERT_EQ(s.subs().getContainer().len(), 2); // `wrapper` from byeol code and `con` from native so file.
    node& origin = s.sub("con") OR_ASSERT(origin);
    {
        baseFunc& sayFunc = origin TO(sub("say")) TO(template cast<baseFunc>()) OR_ASSERT(sayFunc);
        ASSERT_EQ(sayFunc.getRet()->getType(), nVoid().getType());
        ASSERT_EQ(sayFunc.getParams().len(), 0); // 'me' of func won't be passed as an argument.

        args a((narr(origin)));
        str res = sayFunc.eval(a);
        ASSERT_FALSE(res);           // don't eval func itself.
        res = origin.eval("say", a); // don't need to pass 'me' argument
        ASSERT_FALSE(res);
        res = origin.eval("say", args());
        ASSERT_TRUE(res);
        ASSERT_TRUE(res->cast<nVoid>());
    }

    {
        baseFunc& add = origin TO(sub("add")) TO(template cast<baseFunc>()) OR_ASSERT(add);
        const params& argTypes = add.getParams();
        ASSERT_EQ(argTypes.len(), 2);
        ASSERT_EQ(argTypes[0].getOrigin().getType(), ttype<nInt>());
        ASSERT_EQ(argTypes[1].getOrigin().getType(), ttype<nInt>::get());

        nInt arg1(5);
        str retVal = add.eval(args(narr(arg1))); // should nothing happen
        ASSERT_FALSE(retVal);

        args a;
        a.add(new nInt(5));
        a.add(new nInt(3));
        retVal = add.eval(a); // don't eval func itself as I said before
        ASSERT_FALSE(retVal);
        retVal = origin.eval("add", a); // correct!
        ASSERT_TRUE(retVal);
        ASSERT_EQ(retVal->cast<nInt>()->get(), 8);
    }
}
