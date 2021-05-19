#include <iostream>
#include <string>
#include "Header.h"

int main() {
    while (true) {
        try {
            std::string expression;
            std::cout << "Enter expression: ";
            std::getline(std::cin, expression);

            Calculator calculator(expression);
            std::cout << calculator.GetRPN();
            std::cout << std::endl << "Read variable values:" << std::endl;
            calculator.ReadVariables();
            std::cout << "Value: " << calculator.Evaluate() << std::endl;

        }
        catch (const std::string& s) {
            std::cout << s << std::endl;
        }
    }
}