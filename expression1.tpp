#ifndef EXPRESSION_TPP
#define EXPRESSION_TPP

#include "expression.hpp"
#include <stdexcept>

template <typename T>
Expression<T>::Expression(T number) {
    node_left = nullptr;
    node_right = nullptr;
    type = Type::Number;
    value = number;
    variable_name = "";
}

template <typename T>
Expression<T>::Expression(std::string variable) {
    node_left = nullptr;
    node_right = nullptr;
    type = Type::Variable;
    variable_name = variable;
}

template <typename T>
Expression<T>::Expression(Type op, Expression<T> a, Expression<T> b) {
    node_left = std::make_shared<Expression<T>>(a);
    node_right = std::make_shared<Expression<T>>(b);
    type = op;
    variable_name = "";
}

template <typename T>
Expression<T>::Expression(Type op, Expression<T> a) {
    node_left = std::make_shared<Expression<T>>(a);
    node_right = nullptr;
    type = op;
    variable_name = "";
}

template <typename T>
Expression<T>::Expression(const Expression& other)
    : value(other.value),
      variable_name(other.variable_name),
      type(other.type),
      node_left(other.node_left ? std::make_shared<Expression>(*other.node_left) : nullptr),
      node_right(other.node_right ? std::make_shared<Expression>(*other.node_right) : nullptr) {}

template <typename T>
Expression<T>& Expression<T>::operator=(const Expression& other) {
    if (this != &other) {
        value = other.value;
        variable_name = other.variable_name;
        type = other.type;
        node_left = other.node_left ? std::make_shared<Expression>(*other.node_left) : nullptr;
        node_right = other.node_right ? std::make_shared<Expression>(*other.node_right) : nullptr;
    }
    return *this;
}

template <typename T>
Expression<T> Expression<T>::parse(const std::string& expr) {
    size_t pos = 0;
    return parseExpression(expr, pos);
}

template <typename T>
Expression<T> Expression<T>::substitute(const std::string& var_name, const Expression& replacement) const {
    if (type == Type::Variable) {
        return (variable_name == var_name) ? replacement : *this;
    } else if (type == Type::Number) {
        return *this;
    } else {
        switch (type) {
            case Type::Addition:
            case Type::Subtraction:
            case Type::Multiplication:
            case Type::Division:
            case Type::Exponentiation: {
                Expression new_left = node_left->substitute(var_name, replacement);
                Expression new_right = node_right->substitute(var_name, replacement);
                return Expression(type, new_left, new_right);
            }
            case Type::Sin:
            case Type::Cos:
            case Type::Ln:
            case Type::Exp: {
                Expression new_left = node_left->substitute(var_name, replacement);
                return Expression(type, new_left);
            }
            default:
                throw std::runtime_error("Unknown operation type during substitution");
        }
    }
}

template <typename T>
Expression<T> Expression<T>::operator+(const Expression& other) const {
    return Expression<T>(Type::Addition, *this, other);
}

template <typename T>
Expression<T> Expression<T>::operator-(const Expression& other) const {
    return Expression<T>(Type::Subtraction, *this, other);
}

template <typename T>
Expression<T> Expression<T>::operator*(const Expression& other) const {
    return Expression<T>(Type::Multiplication, *this, other);
}

template <typename T>
Expression<T> Expression<T>::operator/(const Expression& other) const {
    return Expression<T>(Type::Division, *this, other);
}

template <typename T>
Expression<T> Expression<T>::operator^(const Expression& other) const {
    return Expression<T>(Type::Exponentiation, *this, other);
}

template <typename T>
Expression<T> Expression<T>::sin() const {
    return Expression<T>(Type::Sin, *this);
}

template <typename T>
Expression<T> Expression<T>::cos() const {
    return Expression<T>(Type::Cos, *this);
}

template <typename T>
Expression<T> Expression<T>::ln() const {
    return Expression<T>(Type::Ln, *this);
}

template <typename T>
Expression<T> Expression<T>::exp() const {
    return Expression<T>(Type::Exp, *this);
}

template <typename T>
T Expression<T>::calculate() {
    std::unordered_map<std::string, T> x;
    return calculate(x);
}

template <typename T>
T Expression<T>::calculate(std::unordered_map<std::string, T>& variables) {
    switch (type) {
        case Type::Number:
            return value;

        case Type::Variable:
            if (variables.find(variable_name) != variables.end()) {
                return variables[variable_name];
            }
            throw std::runtime_error("Variable not found: " + variable_name);

        case Type::Addition:
            return node_left->calculate(variables) + node_right->calculate(variables);

        case Type::Subtraction:
            return node_left->calculate(variables) - node_right->calculate(variables);

        case Type::Multiplication:
            return node_left->calculate(variables) * node_right->calculate(variables);

        case Type::Division:
            if (node_right->calculate(variables) == 0) {
                throw std::runtime_error("Division by zero");
            }
            return node_left->calculate(variables) / node_right->calculate(variables);

        case Type::Exponentiation:
            return std::pow(node_left->calculate(variables), node_right->calculate(variables));

        case Type::Sin:
            return std::sin(node_left->calculate(variables));

        case Type::Cos:
            return std::cos(node_left->calculate(variables));

        case Type::Ln:
            if (node_left->calculate(variables) <= 0) {
                throw std::runtime_error("Logarithm of non-positive number");
            }
            return std::log(node_left->calculate(variables));

        case Type::Exp:
            return std::exp(node_left->calculate(variables));

        default:
            throw std::runtime_error("Unsupported operation type");
    }
}

template <typename T>
std::string Expression<T>::to_string() {
    switch (type) {
        case Type::Number:
            return std::to_string(value);

        case Type::Variable:
            return variable_name;

        case Type::Addition:
            return "(" + node_left->to_string() + " + " + node_right->to_string() + ")";

        case Type::Subtraction:
            return "(" + node_left->to_string() + " - " + node_right->to_string() + ")";

        case Type::Multiplication:
            return "(" + node_left->to_string() + " * " + node_right->to_string() + ")";

        case Type::Division:
            return "(" + node_left->to_string() + " / " + node_right->to_string() + ")";

        case Type::Exponentiation:
            return "(" + node_left->to_string() + " ^ " + node_right->to_string() + ")";

        case Type::Sin:
            return "sin(" + node_left->to_string() + ")";

        case Type::Cos:
            return "cos(" + node_left->to_string() + ")";

        case Type::Exp:
            return "exp(" + node_left->to_string() + ")";

        case Type::Ln:
            return "ln(" + node_left->to_string() + ")";

        default:
            throw std::runtime_error("Unsupported operation type");
    }
}

template <typename T>
Expression<T> Expression<T>::differentiate(const std::string& var_name) const {
    switch (type) {
        case Type::Number:
            return Expression<T>(0);

        case Type::Variable:
            return Expression<T>(variable_name == var_name ? 1 : 0);

        case Type::Addition:
            return node_left->differentiate(var_name) + node_right->differentiate(var_name);

        case Type::Subtraction:
            return node_left->differentiate(var_name) - node_right->differentiate(var_name);

        case Type::Multiplication:
            return (node_left->differentiate(var_name) * (*node_right)) +
                   ((*node_left) * node_right->differentiate(var_name));

        case Type::Division:
            return (node_left->differentiate(var_name) * (*node_right) -
                   ((*node_left) * node_right->differentiate(var_name))) /
                   ((*node_right) ^ Expression<T>(2));

        case Type::Exponentiation:
            return ((*node_left) ^ (*node_right)) *
                   (node_right->differentiate(var_name) * node_left->ln() +
                    ((*node_right) * node_left->differentiate(var_name)) / (*node_left));

        case Type::Sin:
            return node_left->cos() * node_left->differentiate(var_name);

        case Type::Cos:
            return Expression<T>(-1) * node_left->sin() * node_left->differentiate(var_name);

        case Type::Ln:
            return node_left->differentiate(var_name) / (*node_left);

        case Type::Exp:
            return node_left->exp() * node_left->differentiate(var_name);

        default:
            throw std::runtime_error("Unsupported operation for differentiation");
    }
}

template <typename T>
Expression<T> Expression<T>::parseExpression(const std::string& expr, size_t& pos) {
    Expression<T> left = parseTerm(expr, pos);
    while (pos < expr.size()) {
        skipWhitespace(expr, pos);
        char op = expr[pos];
        if (op == '+' || op == '-') {
            pos++;
            Expression<T> right = parseTerm(expr, pos);
            if (op == '+') {
                left = left + right;
            } else {
                left = left - right;
            }
        } else {
            break;
        }
    }
    return left;
}

template <typename T>
Expression<T> Expression<T>::parseTerm(const std::string& expr, size_t& pos) {
    Expression<T> left = parsePower(expr, pos);
    while (pos < expr.size()) {
        skipWhitespace(expr, pos);
        char op = expr[pos];
        if (op == '*' || op == '/') {
            pos++;
            Expression<T> right = parsePower(expr, pos);
            if (op == '*') {
                left = left * right;
            } else {
                left = left / right;
            }
        } else {
            break;
        }
    }
    return left;
}

template <typename T>
Expression<T> Expression<T>::parsePower(const std::string& expr, size_t& pos) {
    Expression<T> left = parseFactor(expr, pos);
    while (pos < expr.size()) {
        skipWhitespace(expr, pos);
        char op = expr[pos];
        if (op == '^') {
            pos++;
            Expression<T> right = parseFactor(expr, pos);
            left = left ^ right;
        } else {
            break;
        }
    }
    return left;
}

template <typename T>
Expression<T> Expression<T>::parseFactor(const std::string& expr, size_t& pos) {
    skipWhitespace(expr, pos);
    if (pos >= expr.size()) {
        throw std::runtime_error("Unexpected end of expression");
    }

    if (expr[pos] == '(') {
        pos++;
        Expression<T> result = parseExpression(expr, pos);
        skipWhitespace(expr, pos);
        if (pos >= expr.size() || expr[pos] != ')') {
            throw std::runtime_error("Expected ')'");
        }
        pos++;
        return result;
    }

    if (isdigit(expr[pos]) || expr[pos] == '.') {
        return parseNumber(expr, pos);
    }

    if (isalpha(expr[pos])) {
        return parseFunctionOrVariable(expr, pos);
    }

    if (expr[pos] == '-') {
        pos++;
        return Expression<T>(Type::Subtraction, Expression<T>(0), parseFactor(expr, pos));
    }

    throw std::runtime_error("Unexpected character: " + std::string(1, expr[pos]));
}

template <typename T>
Expression<T> Expression<T>::parseNumber(const std::string& expr, size_t& pos) {
    size_t start = pos;
    while (pos < expr.size() && (isdigit(expr[pos]) || expr[pos] == '.')) {
        pos++;
    }
    std::string numStr = expr.substr(start, pos - start);
    return Expression<T>(static_cast<T>(std::stod(numStr)));
}

template <typename T>
Expression<T> Expression<T>::parseFunctionOrVariable(const std::string& expr, size_t& pos) {
    size_t start = pos;
    while (pos < expr.size() && isalpha(expr[pos])) {
        pos++;
    }
    std::string token = expr.substr(start, pos - start);

    if (token == "sin") {
        skipWhitespace(expr, pos);
        if (expr[pos] != '(') {
            throw std::runtime_error("Expected '(' after 'sin'");
        }
        pos++;
        Expression<T> arg = parseExpression(expr, pos);
        skipWhitespace(expr, pos);
        if (expr[pos] != ')') {
            throw std::runtime_error("Expected ')' after sin argument");
        }
        pos++;
        return arg.sin();
    }

    if (token == "cos") {
        skipWhitespace(expr, pos);
        if (expr[pos] != '(') {
            throw std::runtime_error("Expected '(' after 'cos'");
        }
        pos++;
        Expression<T> arg = parseExpression(expr, pos);
        skipWhitespace(expr, pos);
        if (expr[pos] != ')') {
            throw std::runtime_error("Expected ')' after cos argument");
        }
        pos++;
        return arg.cos();
    }

    if (token == "exp") {
        skipWhitespace(expr, pos);
        if (expr[pos] != '(') {
            throw std::runtime_error("Expected '(' after 'exp'");
        }
        pos++;
        Expression<T> arg = parseExpression(expr, pos);
        skipWhitespace(expr, pos);
        if (expr[pos] != ')') {
            throw std::runtime_error("Expected ')' after exp argument");
        }
        pos++;
        return arg.exp();
    }

    if (token == "ln") {
        skipWhitespace(expr, pos);
        if (expr[pos] != '(') {
            throw std::runtime_error("Expected '(' after 'ln'");
        }
        pos++;
        Expression<T> arg = parseExpression(expr, pos);
        skipWhitespace(expr, pos);
        if (expr[pos] != ')') {
            throw std::runtime_error("Expected ')' after ln argument");
        }
        pos++;
        return arg.ln();
    }

    return Expression<T>(token);
}

template <typename T>
void Expression<T>::skipWhitespace(const std::string& expr, size_t& pos) {
    while (pos < expr.size() && isspace(expr[pos])) {
        pos++;
    }
}

#endif 