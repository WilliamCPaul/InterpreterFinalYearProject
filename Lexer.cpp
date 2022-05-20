#include "Lexer.h"
#include <iostream>
Lexer::Lexer(std::string source)
{
    this->toBeTokenized = source;
}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens = std::vector<Token>();

    std::string token = "";
    TokenizeState state = TokenizeState::DEFAULT;

    for (int i = 0; i < this->toBeTokenized.size(); i++) {
        char c = toBeTokenized.at(i);
        switch (state) {
        case DEFAULT:
            if (this->opChar.find(c) != std::string::npos) {
                tokens.push_back(Token({ c }, types[this->opChar.find(c)]));
            }
            else if (std::string("abcdefghiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ").find(c) != std::string::npos) {
                token += c;
                state = TokenizeState::WORD;
            }
            else if (std::string("0123456789").find(c) != std::string::npos) {
                token += c;
                state = TokenizeState::NUMBER;
            }
            else if (c == '"') {
                state = TokenizeState::STRING;
            }
            else if (c == '\'') {
                state = TokenizeState::COMMENT;
            }
            break;

        case WORD:
            if (std::string("abcdefghiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ").find(c) != std::string::npos || std::string("0123456789").find(c) != std::string::npos) {
                token += c;
            }
            else if (c == ':') {
                tokens.push_back(Token(token, TokenType::LABEL));
                token = "";
                state = TokenizeState::DEFAULT;
            }
            else {
                tokens.push_back(Token(token, TokenType::WORD));
                token = "";
                state = TokenizeState::DEFAULT;
                i--; 
            }
            break;

        case NUMBER:
            if (std::string("0123456789").find(c) != std::string::npos) {
                token += c;
            }
            else {
                tokens.push_back(Token(token, TokenType::NUMBER));
                token = "";
                state = TokenizeState::DEFAULT;
                i--;
            }
            break;

        case STRING:
            if (c == '"') {
                tokens.push_back(Token(token, TokenType::STRING));
                token = "";
                state = TokenizeState::DEFAULT;
            }
            else {
                token += c;
            }
            break;

        case COMMENT:
            if (c == '\n') {
                state = TokenizeState::DEFAULT;
            }
            break;
        }
    }
    return tokens;
}

Lexer::~Lexer()
{

}
