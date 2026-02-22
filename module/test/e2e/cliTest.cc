#include "test/byeolTest.hpp"

using namespace by;
using namespace std;

struct cliTest: public byeolTest {
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

    flag::res callEval(interpreter& ip, flagArgs& a, starter& s, errReport& rpt) {
        return ep._evalArgs(ip, a, s, rpt);
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

TEST_F(cliTest, canTakeMultipleFilesAsFlags) {
    interpreter ip;
    starter s;
    errReport rpt;
    callEval(ip, parse(2, "a.byeol", "b.byeol"), s, rpt);
    parser& p = ip.getParser();
    auto& srcs = p.getSrcSupplies();
    ASSERT_EQ(srcs.len(), 2);

    fileSupply& file1 = srcs[0].cast<fileSupply>() OR_ASSERT(file1);
    ASSERT_EQ(file1.getPath(), "a.byeol");
    fileSupply& file2 = srcs[1].cast<fileSupply>() OR_ASSERT(file2);
    ASSERT_EQ(file2.getPath(), "b.byeol");
}

TEST_F(cliTest, canTakeMultipleStreamAsFlags) {
    interpreter ip;
    starter s;
    errReport rpt;
    callEval(ip, parse(4, "-s", R"SRC(
def a
    hello() void
        print("hello\n")
)SRC", "-s", R"SRC(
main() void
    a.hello()
)SRC"), s, rpt);
    parser& p = ip.getParser();
    auto& srcs = p.getSrcSupplies();
    ASSERT_EQ(srcs.len(), 2);

    ASSERT_NE(srcs[0].cast<bufSupply>(), nullptr);
    ASSERT_NE(srcs[1].cast<bufSupply>(), nullptr);
}

TEST_F(cliTest, interpretMultipleStreams) {
    interpreter ip;
    starter s;
    errReport rpt;
    cli::programRes res = ep.eval(parse(4, "-s", R"SRC(
def a
    hello() int
        ret 100
)SRC", "-s", R"SRC(
main() int
    a.hello()
)SRC"));

    ASSERT_FALSE(res.rpt);
    ASSERT_EQ(res.res, 100);
}

TEST_F(cliTest, interpretMultipleStreamsWithPackSpecified) {
    interpreter ip;
    starter s;
    errReport rpt;
    cli::programRes res = ep.eval(parse(4, "-s", R"SRC(
pack test
def a
    hello() int
        ret 100
)SRC", "-s", R"SRC(
pack test
main() int
    a.hello()
)SRC"));

    ASSERT_FALSE(res.rpt);
    ASSERT_EQ(res.res, 100);
}

TEST_F(cliTest, interpretMultipleStreamsWhenDependencyTwisted) {
    interpreter ip;
    starter s;
    errReport rpt;
    cli::programRes res = ep.eval(parse(4, "-s", R"SRC(
pack test

def a
    age := b.age # refers b in other src file.
    hello() int
        ret 100 + age
)SRC", "-s", R"SRC(
pack test

def b
    age := 22

main() int
    a.hello()
)SRC"));

    ASSERT_FALSE(res.rpt);
    ASSERT_EQ(res.res, 122);
}
