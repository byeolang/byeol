#include "test/byeolSyntaxTest.hpp"

using namespace nm;

struct parserTest: public byeolSyntaxTest {};

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

    slot s((manifest()));
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

TEST_F(parserTest, slotNoOnTray) {
    make().parse(R"SRC(
        main() void
            ret
    )SRC");
    shouldVerified(true);

    const slot& slot = getSlot() OR_ASSERT(slot);
    auto& owns = (scope::super&) slot.subs().getContainer();
    ASSERT_EQ(owns.len(), 0);
    ASSERT_EQ(slot.getManifest().name, manifest::DEFAULT_NAME);

    scope& next = slot.subs().getNext() OR_ASSERT(next);
    auto& shares = (scope::super&) next.getContainer();
    ASSERT_EQ(shares.len(), 3); // 2 builtin func
    ASSERT_EQ(&slot.getPack(), getSubPack());
    ASSERT_TRUE(getSubPack()->sub<func>("main"));
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

    slot& slot = getSlot() OR_ASSERT(slot);
    ASSERT_TRUE(slot.subs().getNext());
    ASSERT_EQ(slot.getManifest().name, manifest::DEFAULT_NAME);
    ASSERT_EQ(&slot.getPack(), getSubPack());
    ASSERT_TRUE(getSubPack()->sub<func>("main"));
}

TEST_F(parserTest, slotNotSpecifiedButCodeSpecifyPackNegative) {
    // make without name:
    //  slot will be generated. but its name is '{default}'.
    make()
        .negative()
        .parse(R"SRC(
        pack demo
        main() void
            ret
    )SRC")
        .shouldParsed(false);
}

TEST_F(parserTest, slotProperlySpecified) {
    // make with name:
    //  pack will be generated and its name is 'demo'.
    make("demo")
        .parse(R"SRC(
        pack demo
        main() void
            ret
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);
}
