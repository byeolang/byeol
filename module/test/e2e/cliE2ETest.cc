#include "test/byeolE2ETest.hpp"

using namespace by;
using namespace std;

struct cliE2ETest : public byeolE2ETest {};

TEST_F(cliE2ETest, interpretPartialFileNegative) {
    auto res = negative().parse(1, "testdata/a-main.byeol").run();
    ASSERT_NE(res.res, 0);
    ASSERT_TRUE(res.rpt);
}

TEST_F(cliE2ETest, interpretMultipleFiles) {
    auto res = parse(2, "testdata/a-main.byeol", "testdata/a.byeol").run();
    ASSERT_EQ(res.res, 132); // 100 + 32
    ASSERT_FALSE(res.rpt);
}
