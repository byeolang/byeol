#include "test/common/dep.hpp"

using namespace by;

struct cliTest: public ::testing::Test {
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
    ASSERT_NE(res.res, 0);
    ASSERT_TRUE(res.rpt); // should have an error.
    ASSERT_TRUE(res.rpt.inErr(by::NOT_SPECIFIED));
    ASSERT_TRUE(res.rpt.inErr(by::NO_SRC));
}

TEST_F(cliTest, helpFlag) {
    cli::programRes res = ep.eval(parse("-h"));
    ASSERT_EQ(res.res, 0);
    ASSERT_FALSE(res.rpt);
}

TEST_F(cliTest, tryOptionClusteringWillBeSuspendedIfProgramExit) {
    auto res = ep.eval(parse("-Svh")); // S and v sill wants file path to execute,
                                       // but `-h` exit the program.
    ASSERT_EQ(res.res, 0);
    ASSERT_FALSE(res.rpt);
}

TEST_F(cliTest, tryOptionClusteringNegative) {
    auto res = ep.eval(parse("-vS"));
    ASSERT_EQ(res.res, -1); // no source code provided
    ASSERT_TRUE(res.rpt);
}
