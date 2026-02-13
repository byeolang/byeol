#include "test/byeolTest.hpp"

using namespace by;
using namespace std;

struct ctorTest: public byeolTest {};

TEST_F(ctorTest, checkDefaultValues) {
    origin myObj(typeMaker::make<obj>("myObj"));

    defaultCtor c(myObj);
    ASSERT_TRUE(&c.getOrigin());
    const ntype& myObjType = c.getType();
    ASSERT_TRUE(&myObjType);
    ASSERT_TRUE(myObjType.getRet());
    ASSERT_TRUE(myObjType.getRet()->is(myObj));
}

TEST_F(ctorTest, checkDefaultCopyCtor) {
    origin myObj(typeMaker::make<obj>("myObj"));
    myObj.setCallComplete(new evalExpr(&myObj, args()));
    myObj.getOwns().add("age", new nInt(23));

    defaultCopyCtor c(myObj);
    obj cloned;
    args a{&cloned, narr{myObj}};

    ASSERT_EQ(cloned.getOwns().len(), 0);
    auto res = c.eval(a);
    ASSERT_EQ(&cloned, res.get());
    ASSERT_EQ(cloned.getOwns().len(), 1);

    int value = cloned.getOwns()["age"].cast<int>() OR_ASSERT(value);
    ASSERT_EQ(value, 23);
}
