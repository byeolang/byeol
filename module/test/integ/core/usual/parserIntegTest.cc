#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct parserIntegTest: public byeolIntegTest {};
}

TEST_F(parserIntegTest, podNoOnTray) {
    make().parse(R"SRC(
        main() void
            ret
    )SRC");
    shouldVerified(true);

    const pod& p = getPod() OR_ASSERT(slot);
    auto& owns = (scope::super&) p.subs().getContainer();
    ASSERT_EQ(owns.len(), 0);
    ASSERT_EQ(p.getManifest().name, manifest::DEFAULT_NAME);

    scope& next = p.subs().getNext() OR_ASSERT(next);
    auto& shares = (scope::super&) next.getContainer();
    ASSERT_EQ(shares.len(), 3); // 2 builtin func
    ASSERT_EQ(&p, getSubPod());
    ASSERT_TRUE(getSubPod()->sub<func>("main"));
}

TEST_F(parserIntegTest, podNoOnTrayWithoutMake) {
    // no make() call:
    //  so setPod(new pod(manifest())) won't be called.
    //  but it should works too.
    parse(R"SRC(
        main() void
            ret
    )SRC");
    shouldVerified(true);

    pod& p = getPod() OR_ASSERT(pak);
    ASSERT_TRUE(p.subs().getNext());
    ASSERT_EQ(p.getManifest().name, manifest::DEFAULT_NAME);
    ASSERT_EQ(&p, getSubPod());
    ASSERT_TRUE(getSubPod()->sub<func>("main"));
}

TEST_F(parserIntegTest, podNotSpecifiedButCodeSpecifyPodNegative) {
    // make without name:
    //  pod will be generated. but its name is '{default}'.
    make()
        .negative()
        .parse(R"SRC(
        pod demo
        main() void
            ret
    )SRC")
        .shouldParsed(false);
}

TEST_F(parserIntegTest, podProperlySpecified) {
    // make with name:
    //  pod will be generated and its name is 'demo'.
    make("demo")
        .parse(R"SRC(
        pod demo
        main() void
            ret
    )SRC")
        .shouldParsed(true);
    shouldVerified(true);
}

TEST_F(parserIntegTest, parserReturnsPodAsOutput) {
    make("sample")
        .parse(R"SRC(
        pod sample

        def person
            age := 32

        main() void
            ret
    )SRC")
        .shouldVerified(true);

    node& pod = getPod() OR_ASSERT(pod);
    ASSERT_TRUE(pod.sub("person"));
}
