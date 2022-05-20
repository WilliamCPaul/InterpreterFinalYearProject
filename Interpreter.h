#pragma once
#include <string>
#include <map>
#include "Lexer.h"
#include "Parser.h"
class Interpreter
{
public:
    Interpreter();
    ~Interpreter();
    bool interpret(std::vector<Statement*> statementsToRun);
    std::map<std::string, int>* getLabels();
    std::map<std::string, Value*>* getVariables();

private:
    std::map<std::string, Value*> variables;
    std::map<std::string, int> labels;
    int currentStatement;
};