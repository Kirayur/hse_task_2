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
    Expression(T number) {
        node_left = nullptr;
        node_right = nullptr;
        type = Type::Number;
        value = number;
        variable_name = "";
    }
    Expression(std::string variable) {
        node_left = nullptr;
        node_right = nullptr;
        type = Type::Variable;
        //value;
        variable_name = variable;
    }
    Expression(Type op, Expression<T> a, Expression<T> b = nullptr) {
        node_left = std::make_shared<Expression<T>>(a);
        node_right = std::make_shared<Expression<T>>(b);
        type = op;
        //value
        variable_name = "";
    }
    //~Expression();

    Expression operator+(const Expression& other) const {
        return Expression<T>(Type::Addition, *this, other);
    };
    Expression operator-(const Expression& other) const {
        return Expression<T>(Type::Subtraction, *this, other);
    };
    Expression operator*(const Expression& other) const {
        return Expression<T>(Type::Multiplication, *this, other);
    };
    Expression operator/(const Expression& other) const {
        return Expression<T>(Type::Division, *this, other);
    };
    Expression operator^(const Expression& other) const {
        return Expression<T>(Type::Exponentiation, *this, other);
    };
    Expression sin(const Expression& other) const {
        return Expression<T>(Type::Sin, *this, other);
    };
    Expression cos(const Expression& other) const {
        return Expression<T>(Type::Cos, *this, other);
    };
    Expression ln(const Expression& other) const {
        return Expression<T>(Type::Ln, *this, other);
    };
    Expression exp(const Expression& other) const {
        return Expression<T>(Type::Exp, *this, other);
    };
    T calculate() {
        std::unordered_map<std::string, T> x;
        return calculate(x);
    }

    T calculate(std::unordered_map<std::string, T>& variables) {

        switch (type)
        {
        case Type::Number:
            return value;

        case Type::Variable:
            if (variables.find(variable_name) != variables.end()) {
                return variables[variable_name];
            }
            throw std::exception();

        case Type::Addition:
            return node_left->calculate(variables) + node_right->calculate(variables);

        case Type::Subtraction:
            return node_left->calculate(variables) - node_right->calculate(variables);
        
        case Type::Multiplication:
            return node_left->calculate(variables) * node_right->calculate(variables);
        
        case Type::Division:
            return node_left->calculate(variables) / node_right->calculate(variables);
        
        case Type::Exponentiation:
            return std::pow(node_left->calculate(variables), node_right->calculate(variables));

        case Type::Sin:
            return sin(node_left->calculate(variables));
        
        case Type::Cos:
            return cos(node_left->calculate(variables));

        case Type::Ln:
            return log(node_left->calculate(variables));
        
        case Type::Exp:
            return exp(node_left->calculate(variables));    

        default:
            break;
        }
        throw std::exception();
    }


    // обработать все случаи в строках
    std::string to_string() {

        switch (type)
        {
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
            break;
        }
        throw std::exception();
    }

private:
};

//Expression<double> operator""_val(long double val);
//Expression<double> operator""_var(const char* name, size_t size);

#endif

int main() {
    Expression<int> a = Expression<int>(1);
    Expression<int> b = Expression<int>("x");
    Expression<int> c = Expression<int>(2);
    Expression<int> r = (a + c - b);
    std::unordered_map<std::string, int> map = std::unordered_map<std::string, int>();
    map["x"] = 5;
    //std::cout << r.calculate(map);
    std::cout << r.to_string();
    return 0;
}
