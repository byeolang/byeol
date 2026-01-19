#include "test/common/dep.hpp"

using namespace by;

struct cliTest : public ::testing::Test {
    cli ep;
    flagArgs args;

    flagArgs& parse(const std::string& programArg) {
        flagArgs ret;
        std::stringstream ss(programArg);
        std::string splited;
        args.push_back("byeol"); // for program name

        while(std::getline(ss, splited, '\n'))
            args.push_back(splited);
        return args;
    }
};

TEST_F(cliTest, usualScenario) {
    nint res = ep.eval(parse(""));
    BY_I("res=%d", res);
    ASSERT_EQ(res, 0);
}
