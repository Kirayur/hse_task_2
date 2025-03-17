#include "expression.hpp"
#include <iostream>
#include <unordered_map>
#include <sstream>

void parseVariable(const std::string& arg, std::unordered_map<std::string, double>& variables) {
    size_t equalsPos = arg.find('=');
    if (equalsPos == std::string::npos) {
        throw std::runtime_error("Invalid variable format: " + arg);
    }
    std::string varName = arg.substr(0, equalsPos);
    double varValue = std::stod(arg.substr(equalsPos + 1));
    variables[varName] = varValue;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " --eval <expression> [variables] OR --diff <expression> --by <variable>" << std::endl;
        return 1;
    }
    std::string mode = argv[1];
    if (mode == "--eval") {
        if (argc < 3) {
            std::cerr << "Usage: " << argv[0] << " --eval <expression> [variables]" << std::endl;
            return 1;
        }
        std::string expressionStr = argv[2];
        std::unordered_map<std::string, double> variables;
        for (int i = 3; i < argc; ++i) {
            parseVariable(argv[i], variables);
        }
        Expression<double> expr = Expression<double>::parse(expressionStr);
        double result = expr.calculate(variables);
        std::cout << result << std::endl;

    } else if (mode == "--diff") {
        if (argc < 5 || std::string(argv[3]) != "--by") {
            std::cerr << "Usage: " << argv[0] << " --diff <expression> --by <variable>" << std::endl;
            return 1;
        }

        std::string expressionStr = argv[2];
        std::string varName = argv[4];
        Expression<double> expr = Expression<double>::parse(expressionStr);
        Expression<double> derivative = expr.differentiate(varName);
        std::cout << derivative.to_string() << std::endl;

    } else {
        std::cerr << "Unknown mode: " << mode << std::endl;
        return 1;
    }

    return 0;
}