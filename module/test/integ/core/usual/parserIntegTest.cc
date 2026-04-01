#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct parserIntegTest: public byeolIntegTest {};
}

TEST_F(parserIntegTest, packNoOnTray) {
    make().parse(R"SRC(
        main() void
            ret
    )SRC");
    shouldVerified(true);

    const pack& pak = getPack() OR_ASSERT(slot);
    auto& owns = (scope::super&) pak.subs().getContainer();
    ASSERT_EQ(owns.len(), 0);
    ASSERT_EQ(pak.getManifest().name, manifest::DEFAULT_NAME);

    scope& next = pak.subs().getNext() OR_ASSERT(next);
    auto& shares = (scope::super&) next.getContainer();
    ASSERT_EQ(shares.len(), 3); // 2 builtin func
    ASSERT_EQ(&pak, getSubPack());
    ASSERT_TRUE(getSubPack()->sub<func>("main"));
}

TEST_F(parserIntegTest, packNoOnTrayWithoutMake) {
    // no make() call:
    //  so setPack(new pack(manifest())) won't be called.
    //  but it should works too.
    parse(R"SRC(
        main() void
            ret
    )SRC");
    shouldVerified(true);

    pack& pak = getPack() OR_ASSERT(pak);
    ASSERT_TRUE(pak.subs().getNext());
    ASSERT_EQ(pak.getManifest().name, manifest::DEFAULT_NAME);
    ASSERT_EQ(&pak, getSubPack());
    ASSERT_TRUE(getSubPack()->sub<func>("main"));
}

TEST_F(parserIntegTest, packNotSpecifiedButCodeSpecifyPackNegative) {
    // make without name:
    //  pack will be generated. but its name is '{default}'.
    make()
        .negative()
        .parse(R"SRC(
        pack demo
        main() void
            ret
    )SRC")
        .shouldParsed(false);
}

TEST_F(parserIntegTest, packProperlySpecified) {
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

TEST_F(parserIntegTest, parserReturnsPackAsOutput) {
    make("sample")
        .parse(R"SRC(
        pack sample

        def person
            age := 32

        main() void
            ret
    )SRC")
        .shouldVerified(true);

    node& pack = getPack() OR_ASSERT(pack);
    ASSERT_TRUE(pack.sub("person"));
}
