#include "test/byeolTest.hpp"

using namespace by;

struct parserTest: public byeolTest {};

TEST_F(parserTest, testHelloWorld) {
    parser p;
    const nchar* stringScript = R"SRC(
        main() void
            console.print("hello world!")
    )SRC";
    std::string script(stringScript);

    tstr<obj> rootBinder = p.addSupply(*new bufSupply(script)).work();
    ASSERT_TRUE(rootBinder);
    rootBinder = p.relSupplies().addSupply(*new bufSupply(stringScript)).work();
    ASSERT_TRUE(rootBinder);

    pod s((manifest()));
    scope& next = s.subs().getNext() OR_ASSERT(next);
    scope::super& shares = next.getContainer();
    p.setTask(s);
    shares.add("hello", new nStr("hello"));
    ASSERT_TRUE(shares.len() == 1);

    p.relSupplies().addSupply(*new bufSupply(script)).work();
    ASSERT_EQ(shares.len(), 4); // @ctor*2 + main + 'hello': @expand doesn't exist.
    p.relSupplies().addSupply(*new bufSupply(script)).work();
    ASSERT_EQ(shares.len(), 7); // add (main() + @ctor*2) on every parse() call.

    ASSERT_TRUE(*shares.get<nStr>("hello") == nStr("hello"));
}
