#include "test/namuSyntaxTest.hpp"

using namespace nm;

struct starterTest: public namuSyntaxTest {};

TEST_F(starterTest, simpleStarter) {
    make().parse(R"SRC(
        main() void
            ret
    )SRC");
    shouldVerified(true);

    str res = starter().setTask(getSlot().getPack()).work();
    ASSERT_EQ(*res, nVoid());
}

TEST_F(starterTest, mainReturnInt) {
    make()
        .parse(R"SRC(
        main() int
            ret 23
    )SRC")
        .shouldVerified(true);

    tstr<nInt> res = starter().setTask(getSlot().getPack()).work();
    ASSERT_EQ(res->cast<nInt>().get(), 23);
    ASSERT_EQ(res->cast<int>(), 23);
}

TEST_F(starterTest, mainCallFuncAndCheckReturn) {
    make()
        .parse(R"SRC(
        foo() int
            ret 23
        main() int
            ret foo()
    )SRC")
        .shouldVerified(true);

    tstr<nInt> res = starter().setTask(getSlot().getPack()).work();
    ASSERT_EQ(res->cast<nInt>().get(), 23);
    ASSERT_EQ(res->cast<int>(), 23);
}

TEST_F(starterTest, managedCallStack) {
    make()
        .parse(R"SRC(
        def Person
            ctor():;
            ctor(n int)
                setAge(n)
            age int
            setAge(n int) void
                age = n
                ret err("you didn't expect this err, don't you?")
            say() void
                print("I'm " + age as str + " years old\n")
            say(n int) void
                setAge(n)
                say()
        main() void
            p1 Person
            p1.say(5)
    )SRC")
        .shouldVerified(true);

    str res = run();
    const auto& rpt = getReport();
    ASSERT_TRUE(rpt);

    const baseErr& e = rpt[0];
    ASSERT_FALSE(nul(e));

    const callstack& cs = e.getStack();
    ASSERT_FALSE(nul(cs));

    // expecting callstack:
    //      at setAge()
    //      at say(int)
    //      at main()
    ASSERT_TRUE(cs.len() > 2);

    ASSERT_FALSE(nul(cs[0]));
    ASSERT_EQ(cs[0].at, "setAge(n int)");
    ASSERT_FALSE(nul(cs[1]));
    ASSERT_EQ(cs[1].at, "say(n int)");
    ASSERT_FALSE(nul(cs[2]));
    ASSERT_EQ(cs[2].at, "main()");
}
