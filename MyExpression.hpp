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

    Expression(Type op, Expression<T> a) {
        node_left = std::make_shared<Expression<T>>(a);
        node_right = nullptr; 
        type = op;
        variable_name = "";
    }   

    T NodeCopy() {

    }

    Expression(const Expression& other) :
        value(other.value),
        variable_name(other.variable_name),
        type(other.type),
        node_left(other.node_left ? std::make_shared<Expression>(*other.node_left) : nullptr),
        node_right(other.node_right ? std::make_shared<Expression>(*other.node_right) : nullptr)
    {}

    Expression& operator=(const Expression& other) {
        if (this != &other) {
            value = other.value;
            variable_name = other.variable_name;
            type = other.type;
            node_left = other.node_left ? std::make_shared<Expression>(*other.node_left) : nullptr;
            node_right = other.node_right ? std::make_shared<Expression>(*other.node_right) : nullptr;
        }
        return *this;
    }       

    Expression(Expression&& other) noexcept = default;

    Expression& operator=(Expression&& other) noexcept = default;

    Expression substitute(const std::string& var_name, const Expression& replacement) const {
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

    ~Expression() = default;
    
    /*
    ~Expression() {
        if (node_left != nullptr) {
            (*node_left).~Expression();
        }
        if (node_right != nullptr) {
            (*node_right).~Expression();
        }
    }
    */

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
    Expression sin() const {
        return Expression<T>(Type::Sin, *this);
    };
    Expression cos() const {
        return Expression<T>(Type::Cos, *this);
    };
    Expression ln() const {
        return Expression<T>(Type::Ln, *this);
    };  
    Expression exp() const {
        return Expression<T>(Type::Exp, *this);
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
            return std::sin(node_left->calculate(variables));
        
        case Type::Cos:
            return std::cos(node_left->calculate(variables));

        case Type::Ln:
            return std::log(node_left->calculate(variables));
        
        case Type::Exp:
            return std::exp(node_left->calculate(variables)); 

        default:
            break;
        }
        throw std::exception();
    }


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
/*
int main() {
    Expression<int> a = Expression<int>(1);
    Expression<int> b = Expression<int>("x");
    Expression<int> c = Expression<int>(2);
    Expression<int> r = (a + c);
    std::unordered_map<std::string, int> map = std::unordered_map<std::string, int>();
    map["x"] = 5;
    std::cout << r.calculate();
    return 0;
}
*/