#pragma once
#include <string>
const enum TokenType {WORD, NUMBER, STRING, LABEL, LINE, EQUALS, OPERATOR, LEFT_PAREN, RIGHT_PAREN, EOf};
class Token {
public:
	Token(std::string str, TokenType tpe) {
		this->tokenData = str;
		this->type = tpe;
	};
	~Token() {};
		std::string getTokenData() {
			return this->tokenData;
	}
		TokenType getTokenType() {
			return this->type;
		}
private:
	TokenType type;
	std::string tokenData;
};