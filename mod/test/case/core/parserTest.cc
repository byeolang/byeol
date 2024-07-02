#include "../../namuSyntaxTest.hpp"

using namespace nm;

struct parserTest : public namuSyntaxTest {};

TEST_F(parserTest, testHelloWorld) {
    parser p;
    const nchar* stringScript = R"SRC(
        main() void
            console.print("hello world!")
    )SRC";
    std::string script(stringScript);

    tstr<obj> rootBinder = p.addSupply(*new bufSupply(script)).work();
    ASSERT_TRUE(rootBinder);
    rootBinder = p.relSupplies()
                  .addSupply(*new bufSupply(stringScript))
                  .work();
    ASSERT_TRUE(rootBinder);

    slot s((manifest()));
    scope::super& shares = (scope::super&) (s.subs().getNext().getContainer());
    ASSERT_FALSE(nul(shares));
    p.setTask(s);
    shares.add("hello", new nStr("hello"));
    ASSERT_TRUE(shares.len() == 1);

    p.relSupplies().addSupply(*new bufSupply(script)).work();
    ASSERT_EQ(shares.len(), 4); // @ctor*2 + main + 'hello': @preCtor doesn't exist.
    p.relSupplies().addSupply(*new bufSupply(script)).work();
    ASSERT_EQ(shares.len(), 5); // add func main on every parse() call.

    ASSERT_TRUE(shares.get<nStr>("hello") == nStr("hello"));
}

TEST_F(parserTest, slotNoOnTray) {
    make().parse(R"SRC(
        main() void
            ret
    )SRC");
    shouldVerified(true);

    auto& owns = (scope::super&) getSlot().subs().getContainer();
    ASSERT_FALSE(nul(owns));
    ASSERT_EQ(owns.len(), 0);
    ASSERT_EQ(getSlot().getManifest().name, manifest::DEFAULT_NAME);
    auto& shares = (scope::super&) getSlot().subs().getNext().getContainer();
    ASSERT_FALSE(nul(shares));
    ASSERT_EQ(shares.len(), 3); // 2 builtin func
    ASSERT_EQ(&getSlot().getPack(), &getSubPack());
    func& f = getSubPack().sub<func>("main");
    ASSERT_FALSE(nul(f));
}

TEST_F(parserTest, slotNoOnTrayWithoutMake) {
    // no make() call:
    //  so setPack(new slot(manifest())) won't be called.
    //  but it should works too.
    parse(R"SRC(
        main() void
            ret
    )SRC");
    shouldVerified(true);

    auto& shares = (scope::super&) getSlot().subs().getNext().getContainer();
    ASSERT_FALSE(nul(shares));
    ASSERT_EQ(getSlot().getManifest().name, manifest::DEFAULT_NAME);
    ASSERT_EQ(&getSlot().getPack(), &getSubPack());
    func& f = getSubPack().sub<func>("main");
    ASSERT_FALSE(nul(f));
}

TEST_F(parserTest, slotNotSpecifiedButCodeSpecifyPackNegative) {
    // make without name:
    //  slot will be generated. but its name is '{default}'.
    make().negative().parse(R"SRC(
        pack demo
        main() void
            ret
    )SRC").shouldParsed(false);
}

TEST_F(parserTest, slotProperlySpecified) {
    // make with name:
    //  pack will be generated and its name is 'demo'.
    make("demo").parse(R"SRC(
        pack demo
        main() void
            ret
    )SRC").shouldParsed(true);
    shouldVerified(true);
}
