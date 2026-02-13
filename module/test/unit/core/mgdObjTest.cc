#include "test/byeolTest.hpp"

using namespace by;
using namespace std;

struct mgdObjTest: public byeolTest {};

namespace {
    struct myObj: public obj {
        BY(CLASS(myObj, obj))
    };
}

TEST_F(mgdObjTest, testGetOriginPointingThis) {
    myObj obj1;
    ASSERT_EQ(&obj1.getOrigin(), &obj1);

    tstr<obj> copied((obj*) obj1.clone());
    ASSERT_TRUE(copied);
    ASSERT_NE(copied.get(), &obj1);
}
