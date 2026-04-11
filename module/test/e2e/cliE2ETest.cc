#include "test/byeolE2ETest.hpp"

using namespace by;
using namespace std;

struct cliE2ETest: public byeolE2ETest {};

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

TEST_F(cliE2ETest, samePodAmongMultipleFiles) {
    auto res = parse(2, "testdata/b-main.byeol", "testdata/b.byeol").run();
    ASSERT_EQ(res.res, 132);
    ASSERT_FALSE(res.rpt);
}

TEST_F(cliE2ETest, exampleMikes) {
    auto res = parse(1, "testdata/mikes.byeol").run();
    ASSERT_EQ(res.res, 17);
    ASSERT_FALSE(res.rpt);
}
