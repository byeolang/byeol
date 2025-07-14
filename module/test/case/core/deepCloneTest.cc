#include "test/namuSyntaxTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct cloneDeepTest: public namuSyntaxTest {};
}

TEST_F(cloneDeepTest, narrCloneDeep) {
    obj o1;
    scope& subs = o1.subs();
    nInt i1(1), i2(2);
    subs.add("1", i1);
    subs.add("2", i2);

    tstr<obj> o2((obj*) o1.cloneDeep());
    ASSERT_EQ(o2->subs().len(), 2);
    nInt& s1 = o2->sub<nInt>("1") OR_ASSERT(s1);
    nInt& s2 = o2->sub<nInt>("2") OR_ASSERT(s2);
    ASSERT_EQ(s1, i1);
    ASSERT_EQ(s2, i2);
    ASSERT_NE(&s1, &i1);
    ASSERT_NE(&s2, &i2);
}

TEST_F(cloneDeepTest, paramsDeepClone) {
    params p;
    p.add(new param("1", new nInt(1)));
    p.add(new param("2", new nFlt(1.5f)));
    ASSERT_EQ(p.len(), 2);
    ASSERT_EQ(*p[0].getOrigin().cast<nint>(), 1);
    ASSERT_EQ(*p[1].getOrigin().cast<nflt>(), 1.5f);

    params& p1 = (params*) p.cloneDeep() OR_ASSERT(p1);
    ASSERT_EQ(p1.len(), p.len());
    for(nidx n = 0; n < p.len(); n++) {
        ASSERT_EQ(*p1.get(n)->getOrigin().as<nInt>(), *p[n].getOrigin().as<nInt>());
        ASSERT_NE(p1.get(n), &p[n]);
        ASSERT_NE(&p1.get(n)->getOrigin(), &p[n].getOrigin());
    }
}
