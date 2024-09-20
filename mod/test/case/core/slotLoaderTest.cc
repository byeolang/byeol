#include "../../namuTest.hpp"

using namespace nm;
using namespace std;

struct slotLoaderTest: public namuTest {};

TEST_F(slotLoaderTest, testDefaultLoaderInit) {
    nmap& systemSlots =
        (nmap&) nm::thread::get().getSlots(); // don't worry for casting. I know what I'm doing >_o
    ASSERT_FALSE(nul(systemSlots));

    slot& s =
        systemSlots.get<slot>([](const std::string& name, const slot& e) { return name == "sys"; });
    ASSERT_FALSE(nul(s));

    ASSERT_EQ(s.subs().len(), 1);
    node& origin = s["con"];
    ASSERT_FALSE(nul(origin));
    {
        baseFunc& sayFunc = origin["say"].cast<baseFunc>();
        ASSERT_FALSE(nul(sayFunc));

        ASSERT_EQ(sayFunc.getRet().getType(), nVoid().getType());
        ASSERT_EQ(sayFunc.getParams().len(), 0); // 'me' of func won't be passed as an argument.

        args a(narr{origin});
        str res = sayFunc.run(a);
        ASSERT_FALSE(res);          // don't run func itself.
        res = origin.run("say", a); // don't need to pass 'me' argument
        ASSERT_FALSE(res);
        res = origin.run("say", args());
        ASSERT_TRUE(res);
        ASSERT_EQ(res->cast<nVoid>(), nVoid());
    }

    {
        baseFunc& add = origin["add"].cast<baseFunc>();
        ASSERT_FALSE(nul(add));

        const params& argTypes = add.getParams();
        ASSERT_EQ(argTypes.len(), 2);
        ASSERT_EQ(argTypes[0].getOrigin().getType(), ttype<nInt>());
        ASSERT_EQ(argTypes[1].getOrigin().getType(), ttype<nInt>::get());

        nInt arg1(5);
        str retVal = add.run(narr{arg1}); // should nothing happen
        ASSERT_FALSE(retVal);

        args a;
        a.add(new nInt(5));
        a.add(new nInt(3));
        retVal = add.run(a); // don't run func itself as I said before
        ASSERT_FALSE(retVal);
        retVal = origin.run("add", a); // correct!
        ASSERT_TRUE(retVal);
        ASSERT_EQ(retVal->cast<nInt>().get(), 8);
    }
}
