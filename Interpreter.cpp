#include "Interpreter.h"
#include <vector>
Interpreter::Interpreter()
{
	this->currentStatement = 0;
	variables = std::map<std::string, Value*>();
	labels = std::map<std::string, int>();
}

Interpreter::~Interpreter()
{
}

bool Interpreter::interpret(std::vector<Statement*> statementsToRun)
{
    //Simply takes in the list of statements and runs them, while keeping the labels and status of variables intact throughout the entire program
    while (currentStatement < statementsToRun.size()) {
        int thisStatement = currentStatement;
        currentStatement++;
        statementsToRun.at(thisStatement)->execute(labels,variables,currentStatement);
    }
    return true;
}

std::map<std::string, int>* Interpreter::getLabels()
{
    return &labels;
}

std::map<std::string, Value*>* Interpreter::getVariables()
{
    return &variables;
}
