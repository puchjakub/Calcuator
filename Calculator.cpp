#include "Calculator.h"
#include <stdexcept>
#include <cmath>
#include "CalculatorMode.h"

CalculatorMode Calculator::mode = CalculatorMode::FLT;

void Calculator::switchMode(CalculatorMode newMode) {
    mode = newMode;
}

CalculatorMode Calculator::getMode() {
    return mode;
}

NumberExpression::NumberExpression(double value) : number(value) {}

double NumberExpression::evaluate() const {
    return number;
}

ExponentiationExpression::ExponentiationExpression(std::unique_ptr<Expression> baseExpression, std::unique_ptr<Expression> exponentExpression)
    : base(std::move(baseExpression)), exponent(std::move(exponentExpression)) {}

double ExponentiationExpression::evaluate() const {
    double baseEval = base->evaluate();
    double exponentEval = exponent->evaluate();
    return std::pow(baseEval, exponentEval);
}

BinaryExpression::BinaryExpression(wchar_t operation, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
    : op(operation), lhs(std::move(left)), rhs(std::move(right)) {}

double BinaryExpression::evaluate() const {
    switch (op) {
    case L'+': return lhs->evaluate() + rhs->evaluate();
    case L'-': return lhs->evaluate() - rhs->evaluate();
    case L'×': return lhs->evaluate() * rhs->evaluate();
    case L'÷': {
        double rightEval = rhs->evaluate();
        if (rightEval == 0) {
            throw std::runtime_error("Division by zero");
        }
        return lhs->evaluate() / rightEval;
    }
    default: throw std::runtime_error("Unsupported operation");
    }
}

std::unique_ptr<Expression> Calculator::parseExpression(std::wstring::const_iterator& it, const std::wstring::const_iterator& end) {
    auto left = parseTerm(it, end);

    while (it != end && (*it == L'+' || *it == L'-')) {
        wchar_t op = *it++;
        auto right = parseTerm(it, end);
        left = std::make_unique<BinaryExpression>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<Expression> Calculator::parseTerm(std::wstring::const_iterator& it, const std::wstring::const_iterator& end) {
    auto left = parseFactor(it, end);

    while (it != end && (*it == L'×' || *it == L'÷')) {
        wchar_t op = *it++;
        auto right = parseFactor(it, end);
        left = std::make_unique<BinaryExpression>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<Expression> Calculator::parseFactor(std::wstring::const_iterator& it, const std::wstring::const_iterator& end) {
    auto base = parsePrimary(it, end);

    while (it != end && *it == L'^') {
        // Found an exponentiation operator
        ++it; // Skip the '^' character
        auto exponent = parsePrimary(it, end); // Parse the exponent
        base = std::make_unique<ExponentiationExpression>(std::move(base), std::move(exponent));
    }

    return base;
}

std::unique_ptr<Expression> Calculator::parsePrimary(std::wstring::const_iterator& it, const std::wstring::const_iterator& end) {

    std::wstring numberStr;
    while (it != end && (iswdigit(*it) || *it == L'.')) {
        numberStr.push_back(*it++);
    }

    if (numberStr.empty()) {
        throw std::runtime_error("Expected a number");
    }

    return std::make_unique<NumberExpression>(std::stod(numberStr));
}

std::unique_ptr<Expression> Calculator::parse(const std::wstring& expr) {
    auto it = expr.begin();
    auto end = expr.end();
    return parseExpression(it, end);
}

std::wstring Calculator::convertBinaryToDecimalExpression(const std::wstring& binaryExpr) {
    std::wstring decimalExpr;
    std::wstring::const_iterator it = binaryExpr.begin();

    while (it != binaryExpr.end()) {
        if (*it == L'0' || *it == L'1') {
            std::wstring binaryNumber;
            while (it != binaryExpr.end() && (*it == L'0' || *it == L'1')) {
                binaryNumber.push_back(*it++);
            }
            decimalExpr += std::to_wstring(binaryToDecimal(binaryNumber));
        }
        else {
            decimalExpr.push_back(*it++);
        }
    }

    return decimalExpr;
}

std::wstring Calculator::decimalToBinary(double decimalValue) {
    if (decimalValue < 0) {
        throw std::invalid_argument("Negative numbers cannot be converted to binary.");
    }

    unsigned long long intPart = static_cast<unsigned long long>(decimalValue);
    if (intPart == 0) return L"0";

    std::wstring binaryStr;
    while (intPart > 0) {
        binaryStr = (wchar_t)(L'0' + intPart % 2) + binaryStr;
        intPart /= 2;
    }

    return binaryStr;
}

double Calculator::binaryToDecimal(const std::wstring& binaryNumber) {
    double decimalValue = 0;
    for (wchar_t digit : binaryNumber) {
        decimalValue = 2 * decimalValue + (digit - L'0');
    }
    return decimalValue;
}

std::wstring Calculator::convertOctalToDecimalExpression(const std::wstring& octalExpr) {
    std::wstring decimalExpr;
    std::wstring::const_iterator it = octalExpr.begin();

    while (it != octalExpr.end()) {
        if (*it >= L'0' && *it <= L'7') {
            std::wstring octalNumber;
            while (it != octalExpr.end() && (*it >= L'0' && *it <= L'7')) {
                octalNumber.push_back(*it++);
            }
            decimalExpr += std::to_wstring(octalToDecimal(octalNumber));
        }
        else {
            decimalExpr.push_back(*it++);
        }
    }

    return decimalExpr;
}

std::wstring Calculator::decimalToOctal(double decimalValue) {
    if (decimalValue < 0) {
        throw std::invalid_argument("Negative numbers cannot be converted to octal.");
    }

    unsigned long long intPart = static_cast<unsigned long long>(decimalValue);
    if (intPart == 0) return L"0";

    std::wstring octalStr;
    while (intPart > 0) {
        octalStr = (wchar_t)(L'0' + intPart % 8) + octalStr;
        intPart /= 8;
    }

    return octalStr;
}

double Calculator::octalToDecimal(const std::wstring& octalNumber) {
    double decimalValue = 0;
    double base = 1;

    for (auto it = octalNumber.rbegin(); it != octalNumber.rend(); ++it) {
        if (*it < L'0' || *it > L'7') {
            throw std::invalid_argument("Invalid octal digit.");
        }
        decimalValue += (*it - L'0') * base;
        base *= 8;
    }

    return decimalValue;
}

std::wstring Calculator::convertHexToDecimalExpression(const std::wstring& hexExpr) {
    std::wstring decimalExpr;
    std::wstring::const_iterator it = hexExpr.begin();

    while (it != hexExpr.end()) {
        if ((*it >= L'0' && *it <= L'9') || (*it >= L'A' && *it <= L'F') || (*it >= L'a' && *it <= L'f')) {
            std::wstring hexNumber;
            while (it != hexExpr.end() && ((*it >= L'0' && *it <= L'9') || (*it >= L'A' && *it <= L'F') || (*it >= L'a' && *it <= L'f'))) {
                hexNumber.push_back(*it++);
            }
            decimalExpr += std::to_wstring(hexToDecimal(hexNumber));
        }
        else {
            decimalExpr.push_back(*it++);
        }
    }

    return decimalExpr;
}

std::wstring Calculator::decimalToHex(double decimalValue) {
    if (decimalValue < 0) {
        throw std::invalid_argument("Negative numbers cannot be converted to hexadecimal.");
    }

    unsigned long long intPart = static_cast<unsigned long long>(decimalValue);
    if (intPart == 0) return L"0";

    std::wstring hexStr;
    while (intPart > 0) {
        int digit = intPart % 16;
        wchar_t hexChar = digit < 10 ? L'0' + digit : L'A' + digit - 10;
        hexStr = hexChar + hexStr;
        intPart /= 16;
    }

    return hexStr;
}

double Calculator::hexToDecimal(const std::wstring& hexNumber) {
    double decimalValue = 0;
    double base = 1;

    for (auto it = hexNumber.rbegin(); it != hexNumber.rend(); ++it) {
        int digit = 0;

        if (*it >= L'0' && *it <= L'9') {
            digit = *it - L'0';
        }
        else if (*it >= L'A' && *it <= L'F') {
            digit = 10 + (*it - L'A');
        }
        else if (*it >= L'a' && *it <= L'f') {
            digit = 10 + (*it - L'a');
        }
        else {
            throw std::invalid_argument("Invalid hexadecimal digit.");
        }

        decimalValue += digit * base;
        base *= 16;
    }

    return decimalValue;
}







