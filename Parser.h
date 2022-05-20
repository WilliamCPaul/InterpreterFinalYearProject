#pragma once
#include <vector>
#include <map>
#include "LexParserDataTypes.h"
#include "ParserUnitClasses.h"
class Parser
{
public:
    Parser(std::vector<Token> tokensToParse, std::map<std::string, Value*>* pvar);
    std::vector<Statement* > parse(std::map<std::string, int> *l);
    ~Parser();

private:
    Expression *expression();
    Expression *operate();
    Expression *atomic();

    bool match(TokenType t1, TokenType t2);
    bool match(TokenType t);
    bool match(std::string s);
    Token consume(TokenType t);
    Token consume(std::string s);
    Token last(int off);
    Token get(int off);

    std::vector<Token> tokens;
    int position;
    std::map<std::string, Value*>* pvarMap;
};