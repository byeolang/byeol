#include "test/common/dep.hpp"

using namespace by;

struct cliTest : public ::testing::Test {
    cli ep;
    flagArgs args;

    flagArgs& parse(const std::string& programArg) {
        flagArgs ret;
        std::stringstream ss(programArg);
        std::string splited;

        while(std::getline(ss, splited, '\n'))
            args.push_back(splited);
        return args;
    }
};

TEST_F(cliTest, noSourceCodeProvidedErrorNegative) {
    cli::programRes res = ep.eval(parse(""));
    ASSERT_TRUE(res.res != 0);
    ASSERT_TRUE(res.rpt); // should have an error.
    ASSERT_TRUE(res.rpt.inErr(by::NOT_SPECIFIED));
    ASSERT_TRUE(res.rpt.inErr(by::NO_SRC));
}
