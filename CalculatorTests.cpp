#include "Calculator.h"
#include <gtest/gtest.h>
#include <memory>

TEST(CalculatorTests, CanAddNumbers) {
    // Arrange
    std::wstring expression = L"2+2";
    double expected = 4.0;

    // Act
    auto calc = Calculator::parse(expression);
    double result = calc->evaluate();

    // Assert
    EXPECT_DOUBLE_EQ(expected, result);
}

TEST(CalculatorTests, CanAddMoreNumbers) {
    // Arrange
    std::wstring expression = L"2+2+5";
    double expected = 9.0;

    // Act
    auto calc = Calculator::parse(expression);
    double result = calc->evaluate();

    // Assert
    EXPECT_DOUBLE_EQ(expected, result);
}

TEST(CalculatorTests, OrderOfOperations_AdditionAndMultiplication) {
    // Arrange
    std::wstring expression = L"3+3×9";
    double expected = 30.0; // 3 + (3 * 9) = 30

    // Act
    auto calc = Calculator::parse(expression);
    double result = calc->evaluate();

    // Assert
    EXPECT_DOUBLE_EQ(expected, result);
}

TEST(CalculatorTests, OrderOfOperations_MultiplicationAndAddition) {
    // Arrange
    std::wstring expression = L"9×3+3";
    double expected = 30.0; // (9 * 3) + 3 = 30

    // Act
    auto calc = Calculator::parse(expression);
    double result = calc->evaluate();

    // Assert
    EXPECT_DOUBLE_EQ(expected, result);
}

TEST(CalculatorTests, OrderOfOperations_SubtractionAndDivision) {
    // Arrange
    std::wstring expression = L"10-2÷2";
    double expected = 9.0; // 10 - (2 / 2) = 9

    // Act
    auto calc = Calculator::parse(expression);
    double result = calc->evaluate();

    // Assert
    EXPECT_DOUBLE_EQ(expected, result);
}

TEST(CalculatorTests, BinaryMultiplication) {
    // Arrange
    Calculator::switchMode(CalculatorMode::BIN); // Switch calculator mode to binary
    std::wstring expression = L"11×11×111"; // Binary expression for 3*3*7
    std::wstring expectedBinary = L"111111"; // Binary result for 63

    // Convert expected result from binary to decimal for comparison
    double expected = Calculator::binaryToDecimal(expectedBinary);

    // Act
    auto calc = Calculator::parse(Calculator::convertBinaryToDecimalExpression(expression));
    double result = calc->evaluate();

    // Convert result back to binary
    std::wstring resultBinary = Calculator::decimalToBinary(result);

    // Assert
    EXPECT_EQ(expectedBinary, resultBinary);
}

TEST(CalculatorTests, BinaryAddition) {
    // Arrange
    Calculator::switchMode(CalculatorMode::BIN); // Switch calculator mode to binary
    std::wstring expression = L"10+1"; // Binary expression for 2+1
    std::wstring expectedBinary = L"11"; // Binary result for 3

    // Convert expected result from binary to decimal for comparison
    double expected = Calculator::binaryToDecimal(expectedBinary);

    // Act
    auto calc = Calculator::parse(Calculator::convertBinaryToDecimalExpression(expression));
    double result = calc->evaluate();

    // Convert result back to binary
    std::wstring resultBinary = Calculator::decimalToBinary(result);

    // Assert
    EXPECT_EQ(expectedBinary, resultBinary);
}

TEST(CalculatorTests, OctalAddition) {
    // Arrange
    Calculator::switchMode(CalculatorMode::OCT); // Switch calculator mode to octal
    std::wstring expression = L"7+1"; // Octal expression for 7+1
    std::wstring expectedOctal = L"10"; // Octal result for 8

    // Convert expected result from octal to decimal for comparison
    double expected = Calculator::octalToDecimal(expectedOctal);

    // Act
    auto calc = Calculator::parse(Calculator::convertOctalToDecimalExpression(expression));
    double result = calc->evaluate();

    // Convert result back to octal
    std::wstring resultOctal = Calculator::decimalToOctal(result);

    // Assert
    EXPECT_EQ(expectedOctal, resultOctal);
}

TEST(CalculatorTests, OctalAddition2) {
    // Arrange
    Calculator::switchMode(CalculatorMode::OCT); // Switch calculator mode to octal
    std::wstring expression = L"11+20"; // Octal expression for 7+1
    std::wstring expectedOctal = L"31"; // Octal result for 8

    // Convert expected result from octal to decimal for comparison
    double expected = Calculator::octalToDecimal(expectedOctal);

    // Act
    auto calc = Calculator::parse(Calculator::convertOctalToDecimalExpression(expression));
    double result = calc->evaluate();

    // Convert result back to octal
    std::wstring resultOctal = Calculator::decimalToOctal(result);

    // Assert
    EXPECT_EQ(expectedOctal, resultOctal);
}

TEST(CalculatorTests, HexadecimalAddition) {
    // Arrange
    Calculator::switchMode(CalculatorMode::HEX); // Switch calculator mode to hexadecimal
    std::wstring expression = L"A+B"; // Hexadecimal expression for 10 + 11
    std::wstring expectedHex = L"15"; // Hexadecimal result for 21

    // Convert expected result from hex to decimal for comparison
    double expected = Calculator::hexToDecimal(expectedHex);

    // Act
    auto calc = Calculator::parse(Calculator::convertHexToDecimalExpression(expression));
    double result = calc->evaluate();

    // Convert result back to hex
    std::wstring resultHex = Calculator::decimalToHex(result);

    // Assert
    EXPECT_EQ(expectedHex, resultHex);
}

TEST(CalculatorTests, HexadecimalMultiplication) {
    // Arrange
    Calculator::switchMode(CalculatorMode::HEX); // Switch calculator mode to hexadecimal
    std::wstring expression = L"A×C"; // Hexadecimal expression for 10 * 12
    std::wstring expectedHex = L"78"; // Hexadecimal result for 120

    // Convert expected result from hex to decimal for comparison
    double expected = Calculator::hexToDecimal(expectedHex);

    // Act
    auto calc = Calculator::parse(Calculator::convertHexToDecimalExpression(expression));
    double result = calc->evaluate();

    // Convert result back to hex
    std::wstring resultHex = Calculator::decimalToHex(result);

    // Assert
    EXPECT_EQ(expectedHex, resultHex);
}

TEST(CalculatorTests, CanHandleExponentiation) {
    // Arrange
    std::wstring expression = L"2^3"; // Expression for 2 raised to the power of 3
    double expected = 8.0; // Expected result is 8

    // Act
    auto calc = Calculator::parse(expression);
    double result = calc->evaluate();

    // Assert
    EXPECT_DOUBLE_EQ(expected, result);
}

TEST(CalculatorTests, SquareRootUsingExponentiation) {
    // Arrange
    std::wstring expression = L"2^0.5"; // Expression for square root of 2
    double expected = std::sqrt(2.0); // Expected result is the square root of 2

    // Act
    auto calc = Calculator::parse(expression);
    double result = calc->evaluate();

    // Assert with a small delta for floating point comparison
    EXPECT_NEAR(expected, result, 0.00001);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}