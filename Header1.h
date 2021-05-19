#pragma once

#include <vector>

struct Variable {
    char name;
    bool value;
};

class Variables {
    std::vector<Variable> variables;

    int find(char name) const;
public:
    void AddVariable(char name);
    void SetVariable(char name, bool value);
    bool GetValue(char name) const;
    int GetCountOfVariables() const { return variables.size(); };
    void Read();
};


int Variables::find(char name) const {
    for (int i = 0; i < variables.size(); i++)
        if (variables[i].name == name)return i;

    return -1;
}

void Variables::AddVariable(char name) {
    if (find(name) != -1) {
        return;
    }
    Variable var;
    var.name = name;
    var.value = 0;

    variables.push_back(var);
}

void Variables::SetVariable(char name, bool value) {
    int index = find(name);

    if (index == -1) {
        Variable v;
        v.name = name;
        v.value = value;

        variables.push_back(v);
    }
    else {
        variables[index].value = value;
    }
}

bool Variables::GetValue(char name) const {
    int index = find(name);
    if (index == -1)
        throw std::string("Variable '") + name + "' is not correct!";

    return variables[index].value;
}

void Variables::Read() {
    for (int i = 0; i < variables.size(); i++) {
        std::cout << "Enter value of '" << variables[i].name << "': ";
        std::cin >> variables[i].value;
    }
}