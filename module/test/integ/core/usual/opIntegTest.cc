#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

namespace {
    struct opIntegTest: public byeolIntegTest {};
}

TEST_F(opIntegTest, simpleBinaryOp1) {
    make("demo")
        .negative()
        .parse(R"SRC(
        pack demo
        main() void
            2 + 3*27 + 44 - 27/34*43 - 1
    )SRC")
        .shouldParsed(true);
}

TEST_F(opIntegTest, BinaryOpWithParan) {
    make("demo")
        .negative()
        .parse(R"SRC(
        pack demo
        main() void
         2 + (if 3
          3*27
         )+ 44 - 27/34*43 - 1
    )SRC")
        .shouldParsed(true);
}

TEST_F(opIntegTest, BinaryOpWithParan2) {
    make("demo")
        .negative()
        .parse(R"SRC(
        pack demo
        main() void
         2 + (if 3
          3*27)+ 44 - 27/34*43 - 1
    )SRC")
        .shouldParsed(false);
}

TEST_F(opIntegTest, BinaryOpWithParan3) {
    make("demo")
        .negative()
        .parse(R"SRC(
         pack demo
         main() void
          2 + if 3
           3*27
          + 44 - 27/34*43 - 1
    )SRC")
        .shouldParsed(false);
}

TEST_F(opIntegTest, defexprAsTerm) {
    make("demo")
        .negative()
        .parse(R"SRC(
        pack demo
        main() void
           age int
           grade := 0.5
           (age) + (if 3
            3*27
           ) + (grade) - 27/34*43 - 1
    )SRC")
        .shouldParsed(true);
}

TEST_F(opIntegTest, exprAddFuncCall) {
    make("demo")
        .negative()
        .parse(R"SRC(
        pack demo
        main() void
            2 + 3*27 + 44 - foo(2) * 27/34*43
    )SRC")
        .shouldParsed(true);
}

TEST_F(opIntegTest, exprAddFuncCall2) {
    make("demo")
        .negative()
        .parse(R"SRC(
        pack demo
        main() void
            2 + 3*27 + 44 - foo("a") * 27/34*43 - a.foo(b.boo(c.goo()))
    )SRC")
        .shouldParsed(true);
}

TEST_F(opIntegTest, exprAddFuncCall3) {
    make("demo")
        .negative()
        .parse(R"SRC(
        pack demo
        main() void
            2 + 3*27 + 44 - foo("a12") * 27/34*43 - a.foo(b.boo(c.goo()))
    )SRC")
        .shouldParsed(true);
    shouldVerified(false);
}

TEST_F(opIntegTest, IgnoreWhitespaceAfterBinaryOpAndComma) {
    make()
        .negative()
        .parse(R"SRC(
    main() void
        2 +
      3)SRC")
        .shouldParsed(true);
}

TEST_F(opIntegTest, IgnoreWhitespaceAfterBinaryOpAndComma2) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            activateFrame(ContextManager,
            view.update(context),
            22,
            34) + 2 +
      3)SRC")
        .shouldParsed(true);
}

TEST_F(opIntegTest, IgnoreWhitespaceAfterBinaryOpAndComma3) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            activateFrame(ContextManager,
                view.update(context),
        22,
                            34) + 2 +
      3
            butNextLine.ShouldStartHere()
    )SRC")
        .shouldParsed(true);
}

TEST_F(opIntegTest, IgnoreWhitespaceAfterBinaryOpAndComma4) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            activateFrame(
             ContextManager,
               view.update(
                context),
        22,
                            34) +
                             2 +
    3)SRC")
        .shouldParsed(true);
}
