#include "test/byeolTest.hpp"
#include "core/ast/exprs/FUOExpr.hpp"
#include "core/builtin/scalar/nInt.hpp"
#include "core/builtin/scalar/nByte.hpp"

using namespace by;
using namespace std;

struct FUOExprTest: public byeolTest {};

TEST_F(FUOExprTest, testPostfixDoublePlusConstructor) {
    nInt operand(5);
    FUOExpr expr(FUOExpr::SYMBOL_POSTFIX_DOUBLE_PLUS, operand);

    ASSERT_EQ(expr.getSymbol(), FUOExpr::SYMBOL_POSTFIX_DOUBLE_PLUS);
    ASSERT_TRUE(expr.getOperand().isSub<nInt>());
}

TEST_F(FUOExprTest, testPostfixDoubleMinusConstructor) {
    nInt operand(10);
    FUOExpr expr(FUOExpr::SYMBOL_POSTFIX_DOUBLE_MINUS, operand);

    ASSERT_EQ(expr.getSymbol(), FUOExpr::SYMBOL_POSTFIX_DOUBLE_MINUS);
    ASSERT_TRUE(expr.getOperand().isSub<nInt>());
}

TEST_F(FUOExprTest, testBitwiseNotConstructor) {
    nInt operand(7);
    FUOExpr expr(FUOExpr::SYMBOL_BITWISE_NOT, operand);

    ASSERT_EQ(expr.getSymbol(), FUOExpr::SYMBOL_BITWISE_NOT);
    ASSERT_TRUE(expr.getOperand().isSub<nInt>());
}

TEST_F(FUOExprTest, testGetOperand) {
    nInt operand(42);
    FUOExpr expr(FUOExpr::SYMBOL_POSTFIX_DOUBLE_PLUS, operand);

    const node& retrieved = expr.getOperand();
    ASSERT_TRUE(retrieved.isSub<nInt>());
    ASSERT_EQ(retrieved.cast<nInt>()->get(), 42);
}

TEST_F(FUOExprTest, testGetOperandNonConst) {
    nInt operand(42);
    FUOExpr expr(FUOExpr::SYMBOL_POSTFIX_DOUBLE_PLUS, operand);

    node& retrieved = expr.getOperand();
    ASSERT_TRUE(retrieved.isSub<nInt>());
    ASSERT_EQ(retrieved.cast<nInt>()->get(), 42);
}

TEST_F(FUOExprTest, testInfer) {
    nInt operand(10);
    FUOExpr expr(FUOExpr::SYMBOL_POSTFIX_DOUBLE_PLUS, operand);

    str inferredType = expr.infer();
    ASSERT_TRUE(inferredType);
    ASSERT_TRUE(inferredType->isSub<ntype>());
}

TEST_F(FUOExprTest, testGetSymbolName) {
    const nchar* name1 = FUOExpr::getSymbolName(FUOExpr::SYMBOL_POSTFIX_DOUBLE_PLUS);
    ASSERT_TRUE(name1 != nullptr);
    ASSERT_TRUE(strlen(name1) > 0);

    const nchar* name2 = FUOExpr::getSymbolName(FUOExpr::SYMBOL_POSTFIX_DOUBLE_MINUS);
    ASSERT_TRUE(name2 != nullptr);
    ASSERT_TRUE(strlen(name2) > 0);

    const nchar* name3 = FUOExpr::getSymbolName(FUOExpr::SYMBOL_BITWISE_NOT);
    ASSERT_TRUE(name3 != nullptr);
    ASSERT_TRUE(strlen(name3) > 0);
}

TEST_F(FUOExprTest, testSymbolEnumRange) {
    ASSERT_EQ(FUOExpr::SYMBOL_START, FUOExpr::SYMBOL_POSTFIX_DOUBLE_PLUS);
    ASSERT_EQ(FUOExpr::SYMBOL_POSTFIX_DOUBLE_PLUS, 0);
    ASSERT_GT(FUOExpr::SYMBOL_END, FUOExpr::SYMBOL_START);
}

TEST_F(FUOExprTest, testBitwiseNotWithByte) {
    nByte operand(0b11001100);
    FUOExpr expr(FUOExpr::SYMBOL_BITWISE_NOT, operand);

    ASSERT_TRUE(expr.getOperand().isSub<nByte>());
    ASSERT_EQ(expr.getSymbol(), FUOExpr::SYMBOL_BITWISE_NOT);
}

TEST_F(FUOExprTest, testMultipleExpressionsWithSameOperand) {
    nInt operand(5);
    FUOExpr expr1(FUOExpr::SYMBOL_POSTFIX_DOUBLE_PLUS, operand);
    FUOExpr expr2(FUOExpr::SYMBOL_POSTFIX_DOUBLE_MINUS, operand);
    FUOExpr expr3(FUOExpr::SYMBOL_BITWISE_NOT, operand);

    ASSERT_EQ(expr1.getSymbol(), FUOExpr::SYMBOL_POSTFIX_DOUBLE_PLUS);
    ASSERT_EQ(expr2.getSymbol(), FUOExpr::SYMBOL_POSTFIX_DOUBLE_MINUS);
    ASSERT_EQ(expr3.getSymbol(), FUOExpr::SYMBOL_BITWISE_NOT);
}

TEST_F(FUOExprTest, testDifferentOperandTypes) {
    nInt intOperand(10);
    FUOExpr expr1(FUOExpr::SYMBOL_POSTFIX_DOUBLE_PLUS, intOperand);
    ASSERT_TRUE(expr1.getOperand().isSub<scalar>());

    nByte byteOperand(255);
    FUOExpr expr2(FUOExpr::SYMBOL_BITWISE_NOT, byteOperand);
    ASSERT_TRUE(expr2.getOperand().isSub<scalar>());
}

TEST_F(FUOExprTest, testExprTypeInheritance) {
    nInt operand(1);
    FUOExpr expr(FUOExpr::SYMBOL_POSTFIX_DOUBLE_PLUS, operand);

    ASSERT_TRUE(expr.isSub<by::expr>());
    ASSERT_FALSE(expr.isSub<nInt>());
}
