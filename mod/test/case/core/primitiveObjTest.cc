#include "../../namuTest.hpp"

using namespace nm;
using namespace std;

struct primitiveObjTest: public namuTest {};

TEST_F(primitiveObjTest, testCloneWIntInMgd) {
    nInt origin(1);

    tstr<nInt> inst((nInt*) origin.clone());
    ASSERT_TRUE(inst);
    ASSERT_EQ(origin.get(), inst->get());
    ASSERT_NE(&origin, &(*inst));
}

TEST_F(primitiveObjTest, subsIsDummy) {
    nInt val(2);
    int initial = val.subs().len();

    bicontainable& subs = val.subs();
    ASSERT_FALSE(nul(subs));
    subs.add("not work", new nInt(3));
    ASSERT_EQ(subs.len(), initial);
    ASSERT_FALSE(subs.has("not work"));
    ASSERT_FALSE(subs.begin().isEnd());
}
