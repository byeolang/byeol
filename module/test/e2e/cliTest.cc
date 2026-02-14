#include "test/common/dep.hpp"

using namespace by;

struct cliTest: public ::testing::Test {
    cli ep;
    flagArgs args;

    flagArgs& parse(nint argc, ...) {
        va_list va;
        va_start(va, argc);
        for(nint n = 0; n < argc; n++)
            args.push_back(std::string(va_arg(va, const nchar*)));
        va_end(va);
        return args;
    }
};

TEST_F(cliTest, noSourceCodeProvidedErrorNegative) {
    cli::programRes res = ep.eval(parse(0, ""));
    ASSERT_NE(res.res, 0);
    ASSERT_TRUE(res.rpt); // should have an error.
    ASSERT_TRUE(res.rpt.inErr(by::NOT_SPECIFIED));
    ASSERT_TRUE(res.rpt.inErr(by::NO_SRC));
}

TEST_F(cliTest, helpFlag) {
    cli::programRes res = ep.eval(parse(1, "-h"));
    ASSERT_EQ(res.res, 0);
    ASSERT_FALSE(res.rpt);
}

TEST_F(cliTest, tryOptionClusteringWillBeSuspendedIfProgramExit) {
    auto res = ep.eval(parse(1, "-Svh")); // S and v sill wants file path to execute,
                                          // but `-h` exit the program.
    ASSERT_EQ(res.res, 0);
    ASSERT_FALSE(res.rpt);
}

TEST_F(cliTest, tryOptionClusteringNegative) {
    auto res = ep.eval(parse(1, "-vS"));
    ASSERT_EQ(res.res, -1); // no source code provided
    ASSERT_TRUE(res.rpt);
}

TEST_F(cliTest, provideNoOptionArgumentEvenIfItWantsNegative) {
    auto res = ep.eval(parse(1, "-s")); // -s expects a trailing argument.
    ASSERT_EQ(res.res, -1);             // no source code provided
    ASSERT_TRUE(res.rpt);
}

TEST_F(cliTest, helloWorld) {
    auto res = ep.eval(parse(2, "-s", "main() void: print(\"hello world\")"));
    ASSERT_EQ(res.res, 0);
    ASSERT_FALSE(res.rpt);
}

TEST_F(cliTest, checkProgramExitCode) {
    auto res = ep.eval(parse(2, "-s", "main() int: 2 + 3"));
    ASSERT_FALSE(res.rpt);
    ASSERT_EQ(res.res, 5);
}

TEST_F(cliTest, whiteSpaceProgramArgumentShouldBeIgnored) {
    auto res = ep.eval(parse(3, "-s", "main() int: 2 + 3", "    "));
    ASSERT_FALSE(res.rpt);
    ASSERT_EQ(res.res, 5);
}

TEST_F(cliTest, verFlag) {
    cli::programRes res = ep.eval(parse(1, "--version"));
    ASSERT_EQ(res.res, 0);
    ASSERT_FALSE(res.rpt);
}
