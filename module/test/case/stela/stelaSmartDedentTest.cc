#include "test/byeolTest.hpp"

using namespace std;
using namespace by;

struct stelaSmartDedentTest: public byeolTest {};

TEST_F(stelaSmartDedentTest, checkDefaultAction) {
    stelaSmartDedent sd;
    ASSERT_FALSE(sd.canDedent());

    sd.useLater();
    ASSERT_FALSE(sd.canDedent());

    sd.countUp();
    ASSERT_TRUE(sd.isOn());

    sd.useLater();
    ASSERT_TRUE(sd.canDedent());
    ASSERT_NE(sd.dedent(), 0);
    ASSERT_TRUE(sd.isOn());

    sd.countDown();
    ASSERT_FALSE(sd.canDedent());
    ASSERT_FALSE(sd.isOn());

    sd.rel();
    ASSERT_FALSE(sd.canDedent());
}
