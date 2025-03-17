#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <cmath>
#include <complex>
#include <unordered_map>
#include <memory>

template <typename T>
class Expression {
public:
    T value;
    std::string variable_name;
    enum Type { Variable, Number, Addition, Subtraction, Multiplication, Division, Exponentiation, Sin, Cos, Ln, Exp };
    std::shared_ptr<Expression<T>> node_left;
    std::shared_ptr<Expression<T>> node_right;
    Type type;

    Expression(T number);
    Expression(std::string variable);
    Expression(Type op, Expression<T> a, Expression<T> b);
    Expression(Type op, Expression<T> a);

    Expression(const Expression& other);
    Expression& operator=(const Expression& other);
    Expression(Expression&& other) noexcept = default;
    Expression& operator=(Expression&& other) noexcept = default;

    static Expression<T> parse(const std::string& expr);

    Expression substitute(const std::string& var_name, const Expression& replacement) const;

    ~Expression() = default;

    Expression operator+(const Expression& other) const;
    Expression operator-(const Expression& other) const;
    Expression operator*(const Expression& other) const;
    Expression operator/(const Expression& other) const;
    Expression operator^(const Expression& other) const;
    Expression sin() const;
    Expression cos() const;
    Expression ln() const;
    Expression exp() const;

    T calculate();
    T calculate(std::unordered_map<std::string, T>& variables);

    std::string to_string();

    Expression<T> differentiate(const std::string& var_name) const;

private:
    static Expression<T> parseExpression(const std::string& expr, size_t& pos);
    static Expression<T> parseTerm(const std::string& expr, size_t& pos);
    static Expression<T> parsePower(const std::string& expr, size_t& pos);
    static Expression<T> parseFactor(const std::string& expr, size_t& pos);
    static Expression<T> parseNumber(const std::string& expr, size_t& pos);
    static Expression<T> parseFunctionOrVariable(const std::string& expr, size_t& pos);
    static void skipWhitespace(const std::string& expr, size_t& pos);
};

#include "expression1.tpp"

#endif