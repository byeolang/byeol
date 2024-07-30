#include "../../common/dep.hpp"

using namespace nm;
using namespace std;

struct verifierTest: public ::testing::Test {
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(verifierTest, errMsgFor0ShouldExist) {
    ASSERT_TRUE(errCode::ERR_CODE_END > 0);
    ASSERT_EQ(err::getErrMsg(UNKNOWN), "unknown");
}
