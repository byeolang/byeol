#include "test/byeolIntegTest.hpp"

using namespace by;

namespace {
    struct stmtChainIntegTest: public byeolIntegTest {};
}

TEST_F(stmtChainIntegTest, simpleChain) {
    make()
        .parse(R"SRC(
        age := for n in 2..5: n
    )SRC")
        .shouldParsed(true);
}

TEST_F(stmtChainIntegTest, chain2) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            for n in 2..5: foo(3); boo(3)
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(stmtChainIntegTest, chain3) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            for n in 2..5: foo(3); age := 2 + 5
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(stmtChainIntegTest, chain4) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            for n in 2..5: foo(3); if true: return
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(stmtChainIntegTest, chainNegative1) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            for n in 2..5: foo(3); if true: foo(5); print("this is inside of if expr")
                return ## invalid syntax: when you define block as inline, it should be ended in one line.
                          so next line can't be part of the block. ##
    )SRC")
        .shouldParsed(false);
}

TEST_F(stmtChainIntegTest, chain5) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            for n in 2..5: foo(3); if true: foo(5); print("this is inside of if expr")
            return # ok. belonged to 'main()' func.
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(stmtChainIntegTest, lambdaWithChain) {
    make()
        .negative() // TODO: remove this when I implement lambda syntax
        .parse(R"SRC(
        main() void
            foo((a, b): a + b
            ) # when smartDedent disabled.
    )SRC")
        .shouldParsed(true);
}

TEST_F(stmtChainIntegTest, lambdaWithChainAndSmartDedent) {
    make()
        .negative() // TODO: remove this when I implement lambda syntax
        .parse(R"SRC(
        main() void
            foo((a, b): a + b) # when smartDedent enabled.
    )SRC")
        .shouldParsed(true);
}

TEST_F(stmtChainIntegTest, lambdaWithChainAndSmartDedent2) {
    make()
        .negative() // TODO: remove this when I implement lambda syntax
        .parse(R"SRC(
        main() void
            foo((a, b): for n in 2..5: foo(3))
    )SRC")
        .shouldParsed(true);
}

TEST_F(stmtChainIntegTest, lambdaAndSmartDedentDisabled) {
    make()
        .negative() // TODO: remove this when I implement lambda syntax
        .parse(R"SRC(
        main() void
            a := 5
            foo((a, b)
                for n in 2..5
                    doSomething(n)
            )
    )SRC")
        .shouldParsed(true);
}

TEST_F(stmtChainIntegTest, lambdaWithChainAndSmartDedent3) {
    make()
        .negative() // TODO: remove this when I implement lambda syntax
        .parse(R"SRC(
        main() void
            foo((a, b): for n in 2..5: foo(3), boo(2, 3), (): for n in 2..5
                doSomething(n)
            )
    )SRC")
        .shouldParsed(true);
}
