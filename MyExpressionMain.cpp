#include "MyExpression.cpp"
#include <iostream>
#include <string>

int main() {
    Expression<int> a = Expression<int>(1);
    Expression<int> b = Expression<int>("x");
    Expression<int> c = Expression<int>(7);
    Expression<int> r = (a + c);
    std::unordered_map<std::string, int> map = std::unordered_map<std::string, int>();
    std::cout << r.calculate();
    return 0;
}
