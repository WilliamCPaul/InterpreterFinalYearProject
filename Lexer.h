#pragma once
#include <vector>
#include <string>
#include "LexParserDataTypes.h"
class Lexer
{
public:
    Lexer(std::string source);
    std::vector<Token> tokenize();
    ~Lexer();

private:
    std::string toBeTokenized;
    enum TokenizeState { DEFAULT, WORD, NUMBER, STRING, COMMENT };
    std::string opChar="\n=+-*/<>()";
    TokenType types[10] = { TokenType::LINE, TokenType::EQUALS,
            TokenType::OPERATOR, TokenType::OPERATOR, TokenType::OPERATOR,
            TokenType::OPERATOR, TokenType::OPERATOR, TokenType::OPERATOR,
            TokenType::LEFT_PAREN, TokenType::RIGHT_PAREN };
};