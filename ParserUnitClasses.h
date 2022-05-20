#pragma once
#include <string>
#include <iostream>
#include <map>
class Value;

//The very basic class that many pieces of code that the program depends on
class Expression {
public:
    Expression() {};
    ~Expression() {};
    virtual Value* evaluate() { return nullptr; };
};

//Base class for all Values
class Value : public Expression {
public:
    Value() { type = '0'; };
    ~Value() {};
    virtual Value *evaluate() {
        return new Value();
    }
    virtual std::string getString() { return ""; };
    virtual double getNumber() { return 0; };
    virtual char getValueType() {
        return '0';
    }
    char type;
};
//Base Value class for a number value
class NumberValue : public Value {
public:
    NumberValue(double v) {
        this->value = v;
        this->type = 'n';
    }
    ~NumberValue() {};
    double getNumber() {
        return value;
    }
    std::string getString() {
        return std::to_string(value);
    }
    Value *evaluate() {
        return this;
    }
    char getValueType() {
        return 'n';
    }
private:
    double value;
};
//Base Value class for a string value
class StringValue : public Value {
public:
    StringValue(std::string s) {
        this->str = s;
        this->type = 's';
    }
    ~StringValue() {};
    double getNumber() {
        return std::stod(str);
    }
    std::string getString() {
        return str;
    }
    Value *evaluate() {
        return new StringValue(str);
    }
    char getValueType() {
        return 's';
    }
private:
    std::string str;
};
//Base class
class Statement {
public:
    Statement() {};
    ~Statement() {};
    virtual void execute(std::map<std::string, int>& labelMap, std::map<std::string, Value*>& variableMap, int& programCounter) {};
};

//This statement allows the user to print any information from maths to comparison to variable values into the command prompt
class PrintStatement : public Statement {
public:
    PrintStatement(Expression *expr) {
        this->expression = expr;
    }
    ~PrintStatement() {
    }
    void execute(std::map<std::string, int>& labelMap, std::map<std::string, Value*>& variableMap, int& programCounter) {
        Value* x = this->expression->evaluate();
        if (x->getValueType() == 'n') {
            std::cout << ((NumberValue*)x)->getString() << std::endl;
        }
        else {
            std::cout << ((StringValue*)x)->getString() << std::endl;
        }
    }
private:
    Expression* expression;
};

//This statement allows the user to add user data into a program
class InputStatement : public Statement {
public:
    InputStatement(std::string variableName) {
        this->varName = variableName;
    }
    ~InputStatement() {
    }
    void execute(std::map<std::string, int>& labelMap, std::map<std::string, Value*>& variableMap, int& programCounter) {
        std::string input;
        std::cin >> input;
        if (!input.empty() && input.find_first_not_of("-.0123456789") == std::string::npos) {
            variableMap.insert(std::pair<std::string, Value*>(varName, new NumberValue(std::stod(input))));
        }
        else{
            variableMap.insert(std::pair<std::string, Value*>(varName, new StringValue(input)));
        }
    }
private:
    std::string varName;
};
//This Statement takes an expression, evaluates it then stores it in the variable memory map of the interpreter
class AssignStatement : public Statement {
public:
    AssignStatement(std::string str, Expression *expr) {
        this->varName = str;
        this->valueToAssign = expr;
    }
    ~AssignStatement() {};
    void execute(std::map<std::string, int>& labelMap, std::map<std::string, Value*>& variableMap, int& programCounter) {
        if (variableMap.count(varName) != 0) {
            variableMap[varName] = (Value*)(valueToAssign->evaluate());
        }
        else {
            variableMap.insert(std::pair<std::string, Value*>(varName, (Value*)(valueToAssign->evaluate())));
        }
    }
private:
    std::string varName;
    Expression *valueToAssign;
};
//A simple statement that sets the Program counter to wherever the label directs to
class GoToStatement : public Statement {
public:
    GoToStatement(std::string label) {
        this->labelToJump = label;
    }
    ~GoToStatement() {};

    void execute(std::map<std::string, int>& labelMap, std::map<std::string, Value*>& variableMap, int& programCounter) {
        programCounter = labelMap[labelToJump];
    }
private:
    std::string labelToJump;
};

//Main piece of branching statement that will jump to a label if an expression does not evaluate to 0
class IfThenStatement : public Statement {
public:
    IfThenStatement(Expression* expr, std::string label) {
        this->e = expr;
        this->labelToJump = label;
    }
    ~IfThenStatement() {};
    void execute(std::map<std::string, int>& labelMap, std::map<std::string, Value*>& variableMap, int& programCounter) {
        if (labelMap.count(labelToJump)) {
            double value = (*e->evaluate()).getNumber();
            if (value != 0 ) {
                programCounter = labelMap[labelToJump];
            }
        }
    }
private:
    Expression* e;
    std::string labelToJump;
};
//This expression holds the value of a variable that can be called whenever needed
class VariableExpression : public Value {
public:
    VariableExpression(std::string varName, std::map<std::string, Value*>* pvarMap) {
        this->variableName = varName;
        this->pvar = pvarMap;
    }
    ~VariableExpression() {};
    Value *evaluate() {
        if (pvar->count(variableName)) {
               return (*pvar)[variableName];
        }
        return NULL;
    }
private:
    std::string variableName;
    std::map<std::string, Value*>* pvar;
};
//This expression handles all the maths and comparisons that are necessary to run logic and do calculations
class OperatorExpression : public Expression {
public:
    OperatorExpression(Expression* left, char operation, Expression* right) {
        this->l = left;
        this->oper = operation;
        this->r = right;
    }
    ~OperatorExpression() {};
    Value *evaluate() {
        Value *leftVal = (l->evaluate());
        Value *rightVal = (r->evaluate());
        switch (oper) {
        case '=':
            if (leftVal->getValueType() == 'n') {
                return new NumberValue((((NumberValue*)leftVal)->getNumber() == ((NumberValue*)rightVal)->getNumber()) ? 1 : 0);
            }
            else {
                return new NumberValue((((StringValue*)leftVal)->getString() == ((StringValue*)rightVal)->getString()) ? 1 : 0);
            }
        case '+':
            if (leftVal->getValueType() == 'n') {
                return new NumberValue(((NumberValue*)leftVal)->getNumber() + ((NumberValue*)rightVal)->getNumber());
            }
            else {
                return new StringValue((((StringValue*)leftVal)->getString() + ((StringValue*)rightVal)->getString()));
            }
        case '-':
            return new NumberValue(((NumberValue*)leftVal)->getNumber() - ((NumberValue*)rightVal)->getNumber());
        case '*':
            return new NumberValue(((NumberValue*)leftVal)->getNumber() * ((NumberValue*)rightVal)->getNumber());
        case '/':
            return new NumberValue(((NumberValue*)leftVal)->getNumber() / ((NumberValue*)rightVal)->getNumber());
        case '<':
            if (leftVal->getValueType() == 'n') {
                return new NumberValue((((NumberValue*)leftVal)->getNumber() < ((NumberValue*)rightVal)->getNumber()) ? 1 : 0);
            }
            else {
                return new NumberValue((((StringValue*)leftVal)->getString() < ((StringValue*)rightVal)->getString()) ? 1 : 0);
            }
        case '>':
            if (leftVal->getValueType() == 'n') {
                return new NumberValue((((NumberValue*)leftVal)->getNumber() > ((NumberValue*)rightVal)->getNumber()) ? 1 : 0);
            }
            else {
                return new NumberValue((((StringValue*)leftVal)->getString() > ((StringValue*)rightVal)->getString()) ? 1 : 0);
            }
        default:
            throw "Wrong operator";
        }
    }
private:
    Expression* l;
    Expression* r;
    char oper;
};