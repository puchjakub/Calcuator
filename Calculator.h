#pragma once
#include <memory>
#include <string>
#include "CalculatorMode.h"

class Expression {
public:
    virtual double evaluate() const = 0;
    virtual ~Expression() {}
};

class NumberExpression : public Expression {
    double number;
public:
    NumberExpression(double value);
    double evaluate() const override;
};

class ExponentiationExpression : public Expression {
    std::unique_ptr<Expression> base, exponent;
public:
    ExponentiationExpression(std::unique_ptr<Expression> baseExpression, std::unique_ptr<Expression> exponentExpression);
    double evaluate() const override;
};

class BinaryExpression : public Expression {
    wchar_t op; // Changed from char to wchar_t
    std::unique_ptr<Expression> lhs, rhs;
public:
    BinaryExpression(wchar_t operation, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);
    double evaluate() const override;
};

class Calculator {
public:
    static std::wstring convertBinaryToDecimalExpression(const std::wstring& binaryExpr);
    static std::wstring decimalToBinary(double decimalValue);
    static double binaryToDecimal(const std::wstring& binaryNumber);

    static std::wstring convertOctalToDecimalExpression(const std::wstring& octalExpr);
    static std::wstring decimalToOctal(double decimalValue);
    static double octalToDecimal(const std::wstring& octalNumber);

    static std::wstring convertHexToDecimalExpression(const std::wstring& hexExpr);
    static std::wstring decimalToHex(double decimalValue);
    static double hexToDecimal(const std::wstring& hexNumber);

    static void switchMode(CalculatorMode newMode); // Method to change the mode
    static CalculatorMode getMode(); // Method to get the current mode

    static std::unique_ptr<Expression> parse(const std::wstring& expr);

    static std::unique_ptr<Expression> parsePrimary(std::wstring::const_iterator& it, const std::wstring::const_iterator& end);

private:
    static CalculatorMode mode; // Static mode variable

    // Helper functions for parsing with operator precedence
    static std::unique_ptr<Expression> parseExpression(std::wstring::const_iterator& it, const std::wstring::const_iterator& end);
    static std::unique_ptr<Expression> parseTerm(std::wstring::const_iterator& it, const std::wstring::const_iterator& end);
    static std::unique_ptr<Expression> parseFactor(std::wstring::const_iterator& it, const std::wstring::const_iterator& end);
};