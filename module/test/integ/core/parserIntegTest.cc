#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct parserIntegTest : public byeolIntegTest {};
}

TEST_F(parserIntegTest, slotNoOnTray) {
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

TEST_F(parserIntegTest, slotNoOnTrayWithoutMake) {
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

TEST_F(parserIntegTest, slotNotSpecifiedButCodeSpecifyPackNegative) {
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

TEST_F(parserIntegTest, slotProperlySpecified) {
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
