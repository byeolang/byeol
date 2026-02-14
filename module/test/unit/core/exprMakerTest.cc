#include "test/byeolTest.hpp"
#include "core/ast/exprs/exprMaker.hpp"
#include "core/ast/exprs/breakExpr.hpp"
#include "core/ast/exprs/blockExpr.hpp"
#include "core/builtin/scalar/nInt.hpp"
#include "core/type/ntype.hpp"

using namespace by;
using namespace std;

struct exprMakerTest: public byeolTest {};

TEST_F(exprMakerTest, testDefaultConstructor) {
    exprMaker maker;

    // exprMaker should be valid
    ASSERT_TRUE(maker.isSub<exprMaker>());
}

TEST_F(exprMakerTest, testConstructorWithSourceInfo) {
    srcFile file;
    ncnt row = 10;
    ncnt col = 5;

    exprMaker maker(file, row, col);

    ASSERT_TRUE(maker.isSub<exprMaker>());
}

TEST_F(exprMakerTest, testGetPos) {
    srcFile file;
    ncnt row = 15;
    ncnt col = 20;

    exprMaker maker(file, row, col);

    const point& pos = maker.getPos();
    ASSERT_EQ(pos.row, 15);
    ASSERT_EQ(pos.col, 20);
}

TEST_F(exprMakerTest, testGetSrcFile) {
    srcFile file;
    ncnt row = 1;
    ncnt col = 1;

    exprMaker maker(file, row, col);

    const srcFile& retrievedFile = maker.getSrcFile();
    ASSERT_TRUE(retrievedFile.isSub<srcFile>());
}

TEST_F(exprMakerTest, testSetRow) {
    exprMaker maker;

    maker.setRow(25);

    const point& pos = maker.getPos();
    ASSERT_EQ(pos.row, 25);
}

TEST_F(exprMakerTest, testSetCol) {
    exprMaker maker;

    maker.setCol(30);

    const point& pos = maker.getPos();
    ASSERT_EQ(pos.col, 30);
}

TEST_F(exprMakerTest, testAddRow) {
    exprMaker maker;
    maker.setRow(5);

    maker.addRow();

    const point& pos = maker.getPos();
    ASSERT_EQ(pos.row, 6);
}

TEST_F(exprMakerTest, testAddRowWithStep) {
    exprMaker maker;
    maker.setRow(10);

    maker.addRow(5);

    const point& pos = maker.getPos();
    ASSERT_EQ(pos.row, 15);
}

TEST_F(exprMakerTest, testAddCol) {
    exprMaker maker;
    maker.setCol(3);

    maker.addCol();

    const point& pos = maker.getPos();
    ASSERT_EQ(pos.col, 4);
}

TEST_F(exprMakerTest, testAddColWithStep) {
    exprMaker maker;
    maker.setCol(10);

    maker.addCol(7);

    const point& pos = maker.getPos();
    ASSERT_EQ(pos.col, 17);
}

TEST_F(exprMakerTest, testChainedCalls) {
    exprMaker maker;

    maker.setRow(5).setCol(10).addRow(2).addCol(3);

    const point& pos = maker.getPos();
    ASSERT_EQ(pos.row, 7);
    ASSERT_EQ(pos.col, 13);
}

TEST_F(exprMakerTest, testMakeSrc) {
    srcFile file;
    exprMaker maker(file, 1, 1);

    tstr<src> source = maker.makeSrc("testName");
    ASSERT_TRUE(source);
}

TEST_F(exprMakerTest, testMakeSrcWithEmptyName) {
    srcFile file;
    exprMaker maker(file, 1, 1);

    tstr<src> source = maker.makeSrc("");
    ASSERT_TRUE(source);
}

TEST_F(exprMakerTest, testSetSrcFile) {
    exprMaker maker;
    srcFile file;

    maker.setSrcFile(file);

    const srcFile& retrievedFile = maker.getSrcFile();
    ASSERT_TRUE(retrievedFile.isSub<srcFile>());
}

TEST_F(exprMakerTest, testMakeBreakExpr) {
    exprMaker maker;

    breakExpr* expr = maker.make<breakExpr>();

    ASSERT_TRUE(expr != nullptr);
    ASSERT_TRUE(expr->isSub<breakExpr>());
    delete expr;
}

TEST_F(exprMakerTest, testMakeBlockExpr) {
    exprMaker maker;

    blockExpr* expr = maker.make<blockExpr>();

    ASSERT_TRUE(expr != nullptr);
    ASSERT_TRUE(expr->isSub<blockExpr>());
    delete expr;
}

TEST_F(exprMakerTest, testBirthWithName) {
    exprMaker maker;

    breakExpr* expr = maker.birth<breakExpr>("testBreak");

    ASSERT_TRUE(expr != nullptr);
    ASSERT_TRUE(expr->isSub<breakExpr>());
    delete expr;
}

TEST_F(exprMakerTest, testBirthBlockExprWithName) {
    exprMaker maker;

    blockExpr* expr = maker.birth<blockExpr>("testBlock");

    ASSERT_TRUE(expr != nullptr);
    ASSERT_TRUE(expr->isSub<blockExpr>());
    delete expr;
}

TEST_F(exprMakerTest, testMultipleMakes) {
    exprMaker maker;

    breakExpr* expr1 = maker.make<breakExpr>();
    blockExpr* expr2 = maker.make<blockExpr>();

    ASSERT_TRUE(expr1 != nullptr);
    ASSERT_TRUE(expr2 != nullptr);
    ASSERT_TRUE(expr1->isSub<breakExpr>());
    ASSERT_TRUE(expr2->isSub<blockExpr>());

    delete expr1;
    delete expr2;
}

TEST_F(exprMakerTest, testMultipleBirths) {
    exprMaker maker;

    breakExpr* expr1 = maker.birth<breakExpr>("break1");
    breakExpr* expr2 = maker.birth<breakExpr>("break2");

    ASSERT_TRUE(expr1 != nullptr);
    ASSERT_TRUE(expr2 != nullptr);

    delete expr1;
    delete expr2;
}

TEST_F(exprMakerTest, testGetPosAfterDefaultConstruction) {
    exprMaker maker;

    const point& pos = maker.getPos();
    // Position should be initialized (likely to 0,0 or similar)
    (void) pos; // Intentionally unused - just testing the function works
}

TEST_F(exprMakerTest, testSetRowMultipleTimes) {
    exprMaker maker;

    maker.setRow(10);
    ASSERT_EQ(maker.getPos().row, 10);

    maker.setRow(20);
    ASSERT_EQ(maker.getPos().row, 20);

    maker.setRow(5);
    ASSERT_EQ(maker.getPos().row, 5);
}

TEST_F(exprMakerTest, testSetColMultipleTimes) {
    exprMaker maker;

    maker.setCol(5);
    ASSERT_EQ(maker.getPos().col, 5);

    maker.setCol(15);
    ASSERT_EQ(maker.getPos().col, 15);

    maker.setCol(1);
    ASSERT_EQ(maker.getPos().col, 1);
}

TEST_F(exprMakerTest, testAddRowFromZero) {
    exprMaker maker;
    maker.setRow(0);

    maker.addRow(10);

    ASSERT_EQ(maker.getPos().row, 10);
}

TEST_F(exprMakerTest, testAddColFromZero) {
    exprMaker maker;
    maker.setCol(0);

    maker.addCol(5);

    ASSERT_EQ(maker.getPos().col, 5);
}

TEST_F(exprMakerTest, testMakeSrcWithDifferentNames) {
    srcFile file;
    exprMaker maker(file, 1, 1);

    tstr<src> src1 = maker.makeSrc("name1");
    tstr<src> src2 = maker.makeSrc("name2");

    ASSERT_TRUE(src1);
    ASSERT_TRUE(src2);
}
