#include "expression.hpp"
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <complex>
#include <regex>
#include <stdexcept>

bool isComplexNumber(const std::string& str) {
    return str.find('i') != std::string::npos;
}

template <typename T>
T parseValue(const std::string& str);

template <>
std::complex<double> parseValue<std::complex<double>>(const std::string& str) {
    std::string s = str;
    s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
    if (s == "i") {
        return std::complex<double>(0, 1);
    } else if (s == "-i") {
        return std::complex<double>(0, -1); 
    }
    std::regex complexRegex(R"(([+-]?)(\d*\.?\d*)([+-]?)(\d*\.?\d*)i?)");
    std::smatch match;
    double real = 0.0, imag = 0.0;
    if (std::regex_match(s, match, complexRegex)) {
        if (!match[2].str().empty() || match[1].str() == "+" || match[1].str() == "-") {
            std::string realPart = match[1].str() + match[2].str();
            real = realPart.empty() ? 0 : std::stod(realPart);
        }
        if (match[4].str().empty() && (match[3].str() == "+" || match[3].str() == "-")) {
            imag = (match[3].str() == "+") ? 1 : -1; // Случаи "+i" или "-i"
        } else if (!match[4].str().empty()) {
            std::string imagPart = match[3].str() + match[4].str();
            imag = imagPart.empty() ? 0 : std::stod(imagPart);
        }
    } else {
        throw std::runtime_error("Invalid complex number format: " + str);
    }

    return std::complex<double>(real, imag);
}

template <>
double parseValue<double>(const std::string& str) {
    if (isComplexNumber(str)) {
        throw std::runtime_error("Expected real number, but got complex: " + str);
    }
    return std::stod(str);
}

template <typename T>
void parseVariable(const std::string& arg, std::unordered_map<std::string, T>& variables) {
    size_t equalsPos = arg.find('=');
    if (equalsPos == std::string::npos) {
        throw std::runtime_error("Invalid variable format: " + arg);
    }
    std::string varName = arg.substr(0, equalsPos);
    std::string valueStr = arg.substr(equalsPos + 1);
    variables[varName] = parseValue<T>(valueStr);
}

template <typename T>
void printResult(const T& result);

template <>
void printResult<double>(const double& result) {
    std::cout << result << std::endl;
}

template <>
void printResult<std::complex<double>>(const std::complex<double>& result) {
    std::cout << result.real();
    if (result.imag() != 0) {
        std::cout << (result.imag() > 0 ? "+" : "") << result.imag() << "i";
    }
    std::cout << std::endl;
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
        try {
            bool isComplex = false;
            for (int i = 3; i < argc; ++i) {
                std::string arg = argv[i];
                if (isComplexNumber(arg.substr(arg.find('=') + 1))) {
                    isComplex = true;
                    break;
                }
            }

            if (isComplex) {
                std::unordered_map<std::string, std::complex<double>> variables;
                for (int i = 3; i < argc; ++i) {
                    parseVariable(argv[i], variables);
                }
                variables["i"] = std::complex<double>(0.0, 1.0);
                auto expr = Expression<std::complex<double>>::parse(expressionStr);
                auto result = expr.calculate(variables);
                printResult(result);
            } else {
                std::unordered_map<std::string, double> variables;
                for (int i = 3; i < argc; ++i) {
                    parseVariable(argv[i], variables);
                }
                auto expr = Expression<double>::parse(expressionStr);
                double result = expr.calculate(variables);
                printResult(result);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }

    } else if (mode == "--diff") {
        if (argc < 5 || std::string(argv[3]) != "--by") {
            std::cerr << "Usage: " << argv[0] << " --diff <expression> --by <variable>" << std::endl;
            return 1;
        }

        std::string expressionStr = argv[2];
        std::string varName = argv[4];
        try {
            bool isComplex = false;
            for (int i = 3; i < argc; ++i) {
                std::string arg = argv[i];
                if (isComplexNumber(arg.substr(arg.find('=') + 1))) {
                    isComplex = true;
                    break;
                }
            }

            if (isComplex) {
                auto expr = Expression<std::complex<double>>::parse(expressionStr);
                auto derivative = expr.differentiate(varName);
                std::cout << "Expression: " << expr.to_string() << std::endl;
                std::cout << "Derivative: " << derivative.to_string() << std::endl;
            } else {
                auto expr = Expression<double>::parse(expressionStr);
                auto derivative = expr.differentiate(varName);
                std::cout << "Expression: " << expr.to_string() << std::endl;
                std::cout << "Derivative: " << derivative.to_string() << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }

    } else {
        std::cerr << "Unknown mode: " << mode << std::endl;
        return 1;
    }

    return 0;
}
