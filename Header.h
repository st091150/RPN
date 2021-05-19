#pragma once
#include "Header1.h"
#include <stack>
const int AmountOfOperation = 9;

struct sup{
    std::string NameOfOp;
    char Op;
    int Priority;
};
const sup symbols[] = { {"AND", '&',6},{"OR", 'v' ,5},{"NOT",'~',7},{"IMPL",'>',2},
    {"INV_IMPL",'<',2},{"XOR",'+',5},{"EQUAL",'=',1},{"SHEFFER",'|',4},{"PIRS",'@',3} };


class Calculator {
    std::string expression;
    std::string rpn;
    Variables variables;

    bool IsNumber(char c) const {return c == '0' || c == '1';}
    bool IsVariable(char c) const { return c != 'v' && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')); }
    bool IsOperator(char c) const {
        for (int i = 0; i < 9;i++) {
            if (c == symbols[i].Op)return true;
        }
        return false;
    }
    bool IsBracket(char c) const { return c == '(' || c == ')'; }
    bool IsSpace(char c) const { return c == ' '; }
    bool IsLexeme(char c) const { return IsNumber(c) || IsVariable(c) || IsOperator(c) || IsBracket(c); }
    std::string SplitToLexemes(const std::string& expression) const;

    int GetPriority(char c);
    bool IsMorePriority(char curr, char top);
    std::string RPN(); 

public:
    Calculator(const std::string& expression) {
        this->expression = SplitToLexemes(expression);
        this->rpn = RPN();
    }
    std::string GetRPN()const { return rpn; }
    bool EvaluateOperator(char op, bool arg1, bool arg2) const;
    bool Evaluate(); 
    void ReadVariables() { variables.Read(); }
};
bool Calculator::EvaluateOperator(char op, bool arg1, bool arg2) const {
    if (op == '&')
        return arg1 && arg2;

    if (op == 'v')
        return arg1 || arg2;

    if (op == '+')
        return arg1 ^ arg2;

    if (op == '=')
        return arg1 == arg2;

    if (op == '>')
        return !arg1 || arg2;

    if (op == '<')
        return arg1 || !arg2;

    if (op == '|')
        return !arg1 && !arg2;

    if (op == '@')
        return !arg1 || !arg2;

    throw std::string("Unhandled operator '") + op + "'";
}


std::string Calculator::SplitToLexemes(const std::string& expression) const {
    std::string supVec;
    for (int i = 0; i < expression.length(); i++) {
        if (IsLexeme(expression[i])) {
            supVec.push_back(expression[i]);
        }
        else if (!IsSpace(expression[i])) {
            throw std::string("Invalid character in expression '") + expression[i] + "'";
        }
    }
    return supVec;
}
int Calculator::GetPriority(char lexeme) {
    for (int i = 0; i < AmountOfOperation; i++) {
        if (lexeme == symbols[i].Op) {
            return symbols[i].Priority;
        }
    }
    return 0;
}

bool Calculator::IsMorePriority(char curr, char top) {
    if (curr == '~')
        return GetPriority(top) > GetPriority(curr);

    return GetPriority(top) >= GetPriority(curr);
}

std::string Calculator::RPN() {
    std::string rpn = "";
    std::stack<char> stack;

    for (int i = 0; i < expression.length(); i++) {
        char lexeme = expression[i];

        if (IsNumber(lexeme)) {
            rpn += lexeme;
        }
        else if (IsOperator(lexeme)) {
            while (stack.size() > 0 && IsMorePriority(lexeme, stack.top())) {
                rpn += stack.top();
                stack.pop();
            }

            stack.push(lexeme);
        }
        else if (IsVariable(lexeme)) {
            rpn += lexeme;
            variables.AddVariable(lexeme);
        }
        else if (lexeme == '(') {
            stack.push(lexeme);
        }
        else if (lexeme == ')') {
            while (stack.size() > 0 && stack.top() != '(') {
                rpn += stack.top();
                stack.pop();
            }

            if (stack.size() == 0)
                throw std::string("Incorrect expression: brackets are disbalanced");

            stack.pop();
        }
        else
            throw std::string("Incorrect expression: unknown lexeme '") + lexeme + "'";
    }

    while (stack.size() > 0) {
        if (stack.top() == '(')
            throw std::string("Incorrect expression: brackets are disbalanced");

        rpn += stack.top();
        stack.pop();
    }

    return rpn;
}

bool Calculator::Evaluate() {
    std::stack<bool> stack = {};
    for (int i = 0; i < rpn.size(); i++) {
        char lexeme = rpn[i];

        if (lexeme == '~') {
            if (stack.size() < 1) {
                throw std::string("Unable to evaluate NOT");
            }
            bool sup = stack.top(); stack.pop();
            stack.push(!sup);
        }
        else if (IsOperator(lexeme)) {
            if (stack.size() < 2)
                throw std::string("Unable to evaluate operator '") + lexeme + "'";

            bool arg2 = stack.top();
            stack.pop();
            bool arg1 = stack.top();
            stack.pop();

            stack.push(EvaluateOperator(lexeme, arg1, arg2));
        }
        else if (IsVariable(lexeme)) {
            stack.push(variables.GetValue(lexeme));
        }
        else if (IsNumber(lexeme)) {
            stack.push(lexeme);
        }
        else
            throw std::string("Unknown rpn lexeme '") + lexeme + "'";
    }

    if (stack.size() != 1)
        throw std::string("Incorrect expression");

    return stack.top();
}
