#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct commentIntegTest: public byeolIntegTest {};
}

TEST_F(commentIntegTest, singleLineComment) {
    // control group.
    make()
        .parse(R"SRC(
        age int # age is age
        main() int # main is also a main
            ret 0
    )SRC")
        .shouldVerified(true);

    scope::super& owns = (scope::super*) (getPack() TO(subs().getContainer())) OR_ASSERT(owns);
    scope::super& shares = (scope::super*) (getSubPack() TO(subs().getNext()) TO(getContainer())) OR_ASSERT(shares);
    ASSERT_EQ(owns.len(), 1);   // 1 for age
    ASSERT_EQ(shares.len(), 3); // 1 for main() 2 for @ctor
}

TEST_F(commentIntegTest, multiLineComment) {
    // control group.
    make()
        .parse(R"SRC(
        age int ### age is age
        main() int ###
        main() int
            ret 2
    )SRC")
        .shouldVerified(true);
    scope::super& owns = (scope::super*) (getPack() TO(subs().getContainer())) OR_ASSERT(owns);
    scope::super& shares = (scope::super*) (getSubPack() TO(subs().getNext()) TO(getContainer())) OR_ASSERT(shares);
    ASSERT_EQ(owns.len(), 1);   // 1 for age
    ASSERT_EQ(shares.len(), 3); // 1 for main() 2 for @ctor
}

TEST_F(commentIntegTest, multiLineComment2) {
    // control group.
    make()
        .parse(R"SRC(
        age ### age is age
        main() int # <- this singleline comment and new line should be ignored.
 sdfas   ##int
        main() void
            ret
    )SRC")
        .shouldVerified(true);
    scope::super& owns = (scope::super*) (getPack() TO(subs().getContainer())) OR_ASSERT(owns);
    scope::super& shares = (scope::super*) (getSubPack() TO(subs().getNext()) TO(getContainer())) OR_ASSERT(shares);
    ASSERT_EQ(owns.len(), 1);   // 1 for age
    ASSERT_EQ(shares.len(), 3); // 1 for main() 2 for @ctor
}

TEST_F(commentIntegTest, negativeMultiLineComment) {
    make()
        .negative()
        .parse(R"SRC(
        age ### age is age
        main() int # <-- still comment this entire line.
sdfjwer ###int
        main() void
            # you should not close multline comment with more than 3 '#'.
            ret
    )SRC")
        .shouldVerified(false);
}

TEST_F(commentIntegTest, multiLineComment3Negative) {
    // control group.
    make()
        .negative()
        .parse(R"SRC(
        age int ### age is age
        main() int
 sdfas###main() int
            ret 33
    )SRC")
        .shouldParsed(false);
    /* above case is same to,
     *
     *      age int main() bool
     *          ret false
     */
}
