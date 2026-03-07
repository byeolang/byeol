#include "test/byeolIntegTest.hpp"

using namespace by;

struct starterIntegTest: public byeolIntegTest {};

TEST_F(starterIntegTest, simpleStarter) {
    make().parse(R"SRC(
        main() void
            ret
    )SRC");
    shouldVerified(true);

    str res = starter().setTask(getSlot()->getPack()).work();
    ASSERT_EQ(*res, nVoid());
}

TEST_F(starterIntegTest, mainReturnInt) {
    make()
        .parse(R"SRC(
        main() int
            ret 23
    )SRC")
        .shouldVerified(true);

    tstr<nInt> res = starter().setTask(getSlot()->getPack()).work();
    ASSERT_EQ(res->cast<nInt>()->get(), 23);
    ASSERT_EQ(*res->cast<int>(), 23);
}

TEST_F(starterIntegTest, mainCallFuncAndCheckReturn) {
    make()
        .parse(R"SRC(
        foo() int
            ret 23
        main() int
            ret foo()
    )SRC")
        .shouldVerified(true);

    tstr<nInt> res = starter().setTask(getSlot()->getPack()).work();
    ASSERT_EQ(res->cast<nInt>()->get(), 23);
    ASSERT_EQ(*res->cast<int>(), 23);
}

TEST_F(starterIntegTest, managedCallStack) {
    make()
        .negative() // this code occurs exception
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

    const baseErr& e = rpt.get(0) OR_ASSERT(e);

    const frames& cs = e.getFrames();

    // expecting callstack:
    //      at setAge()
    //      at say(int)
    //      at main()
    ASSERT_TRUE(cs.len() > 2);

    ASSERT_TRUE(cs.get(0));
    const baseFunc& fun = cs[0].getFunc() OR_ASSERT(fun);
    auto funName = fun.as<nStr>();
    ASSERT_TRUE(funName);
    ASSERT_EQ(funName->get(), "setAge(n int)");

    ASSERT_TRUE(cs.get(1));
    const baseFunc& fun1 = cs[1].getFunc() OR_ASSERT(fun1);
    auto fun1Name = fun1.as<nStr>();
    ASSERT_EQ(fun1Name->get(), "say(n int)");

    ASSERT_TRUE(cs.get(2));
    const baseFunc& fun2 = cs[2].getFunc() OR_ASSERT(fun1);
    auto fun2Name = fun2.as<nStr>();
    ASSERT_EQ(fun2Name->get(), "main()");
}
