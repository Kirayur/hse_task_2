#include <cassert>
#include <iostream>
#include <string>
#include "expression.hpp"

void symbol() {
    Expression<int> number = Expression<int>("x");
    std::string expected = "x";
    assert(number.to_string() == expected);
    std::cout << "Symbol test: OK" << std::endl;
}

void addition() {
    Expression<int> number1 = Expression<int>(1);
    Expression<int> number2 = Expression<int>(7);
    long double expected = 8;
    assert((number1 + number2).calculate() == expected);
    std::cout << "Addition test: OK" << std::endl;
}

void subtraction() {
    Expression<int> number1 = Expression<int>(1);
    Expression<int> number2 = Expression<int>(7);
    long double expected = -6;
    assert((number1 - number2).calculate() == expected);
    std::cout << "Subtraction test: OK" << std::endl;
}

void multiplication() {
    Expression<int> number1 = Expression<int>(1);
    Expression<int> number2 = Expression<int>(7);
    long double expected = 7;
    assert((number1 * number2).calculate() == expected);
    std::cout << "Multiplication test: OK" << std::endl;
}

void division() {
    Expression<int> number1 = Expression<int>(4);
    Expression<int> number2 = Expression<int>(2);
    long double expected = 2;
    assert((number1 / number2).calculate() == expected);
    std::cout << "Division test: OK" << std::endl;
}

void exponentiation() {
    Expression<int> number1 = Expression<int>(2);
    Expression<int> number2 = Expression<int>(3);
    long double expected = 8;
    assert((number1 ^ number2).calculate() == expected);
    std::cout << "Exponentiation test: OK" << std::endl;
}

void my_sin() {
    Expression<int> number = Expression<int>(0);
    long double expected = 0; 
    assert((number.sin()).calculate() == expected);
    std::cout << "Sin test: OK" << std::endl;
}

void my_cos() {
    Expression<int> number = Expression<int>(0);
    long double expected = 1; 
    assert((number.cos()).calculate() == expected);
    std::cout << "Cos test: OK" << std::endl;
}

void my_ln() {
    Expression<int> number = Expression<int>(1);
    long double expected = 0; 
    assert((number.cos()).calculate() == expected);
    std::cout << "Ln test: OK" << std::endl;
}

void my_exp() {
    Expression<int> number = Expression<int>(0);
    long double expected = 1; 
    assert((number.cos()).calculate() == expected);
    std::cout << "Exp test: OK" << std::endl;
}

int main() {
    symbol();
    addition();
    subtraction();
    multiplication();
    division();
    exponentiation();
    my_sin();
    my_cos();
    my_ln();
    my_exp();
    std::cout << "All tests passed!" << std::endl;

    try {
        auto expr = Expression<double>::parse("3 ^ 4");
        std::unordered_map<std::string, double> vars;
        double result = expr.calculate(vars);
        std::cout << "Result 3 ^ 4: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error" << std::endl;
    }

    auto expr = Expression<double>::parse("x^y");
    auto derivative = expr.differentiate("y");
    std::cout << "Выражение: " << expr.to_string() << std::endl;
    std::cout << "Производная: " << derivative.to_string() << std::endl;

    return 0;
}
