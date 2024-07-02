#include "../../namuSyntaxTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct cloneDeepTest : public namuSyntaxTest {};
}

TEST_F(cloneDeepTest, narrCloneDeep) {
    obj o1;
    scope& subs = o1.subs();
    nInt i1(1), i2(2);
    subs.add("1", i1);
    subs.add("2", i2);

    tstr<obj> o2((obj*) o1.cloneDeep());
    ASSERT_EQ(o2->subs().len(), 2);
    nInt& s1 = o2->sub<nInt>("1");
    nInt& s2 = o2->sub<nInt>("2");
    ASSERT_EQ(s1, i1);
    ASSERT_EQ(s2, i2);
    ASSERT_NE(&s1, &i1);
    ASSERT_NE(&s2, &i2);
}
