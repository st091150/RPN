#include <iostream>
#include <string>
#include "Header.h"

int main() {
    while (true) {
        try {
            std::string str;
            std::cout << "Enter expression: ";
            std::getline(std::cin, expression);
            Calculator calculator(str);
            std::cout << calculator.GetRPN();
            std::cout << std::endl << "Variable value" << std::endl;
            calculator.ReadVariables();
            std::cout << "Value: " << calculator.Evaluate() << std::endl;

        }
        catch (const std::string& s) {
            std::cout << s << std::endl;
        }
    }
}
