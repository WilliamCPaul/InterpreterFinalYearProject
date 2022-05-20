#include "Parser.h"

Parser::Parser(std::vector<Token> tokensToParse, std::map<std::string, Value*> *pvar)
{
	this->tokens = tokensToParse;
	this->position = 0;
	this->pvarMap = pvar;
}
std::vector<Statement *> Parser::parse(std::map<std::string, int> *l) {
	std::vector<Statement *> statements = std::vector<Statement *>();
	while (true) {
		// Ignore empty lines.
		while (match(TokenType::LINE));

		if (match(TokenType::LABEL)) {
			l->insert(std::pair<std::string, int>(last(1).getTokenData(), statements.size()));
		}
		else if (match(TokenType::WORD, TokenType::EQUALS)) {
			std::string name = last(2).getTokenData();
			Expression *value = expression();
			statements.push_back(new AssignStatement(name, value));
		}
		else if (match("print")) {
			statements.push_back(new PrintStatement(expression()));
		}
		else if (match("input")) {
			statements.push_back(new InputStatement(consume(TokenType::WORD).getTokenData()));
		}
		else if (match("goto")) {
			statements.push_back(new GoToStatement(consume(TokenType::WORD).getTokenData()));
		}
		else if (match("if")) {
			Expression *condition = expression();
			consume("then");
			std::string label = consume(TokenType::WORD).getTokenData();
			statements.push_back(new IfThenStatement(condition, label));
		}
		else break; // Unexpected token (likely EOF), so end.
	}
	return statements;
}
Parser::~Parser()
{
}
Expression* Parser::expression()
{
	return operate();
}
Expression* Parser::operate()
{
	Expression *expression = atomic();
	// Recrusive descent that builds up the entire maths expression
	while (match(TokenType::OPERATOR) ||
		match(TokenType::EQUALS)) {
		char oper = last(1).getTokenData().at(0);
		Expression *right = atomic();
		expression = new OperatorExpression(expression, oper, right);
	}
	return expression;
}
//Singles out a piece of data as a potential terminal
Expression* Parser::atomic()
{
	if (match(TokenType::WORD)) {
		return new VariableExpression(last(1).getTokenData(),this->pvarMap);
	}
	else if (match(TokenType::NUMBER)) {
		return new NumberValue(std::stod(last(1).getTokenData()));
	}
	else if (match(TokenType::STRING)) {
		return new StringValue(last(1).getTokenData());
	}
	else if (match(TokenType::LEFT_PAREN)) {
		Expression* expression = operate();
		consume(TokenType::RIGHT_PAREN);
		return expression;
	}
	throw "Error parsing";
}
;
/*
Collection of functions that allow the main parser function to ensure that the combination of tokens follow the ruleset that the statement requires to be valid
*/

bool Parser::match(TokenType t1, TokenType t2)
{
	if (get(0).getTokenType() != t1) return false;
	if (get(1).getTokenType() != t2) return false;
	this->position += 2;
	return true;
};
bool Parser::match(TokenType t)
{
	if (get(0).getTokenType() != t) return false;
	position++;
	return true;
};
bool Parser::match(std::string s)
{
	if (get(0).getTokenType() != TokenType::WORD) return false;
	if (!(get(0).getTokenData() == s)) return false;
	position++;
	return true;
};
Token Parser::consume(TokenType t)
{
	if (get(0).getTokenType() != t) throw "Expected another token";
	return this->tokens.at(position++);
};
Token Parser::consume(std::string s) {
	if (!match(s)) throw "Expected another name";
	return last(1);
}
Token Parser::last(int off)
{
	return tokens.at(position - off);
};
Token Parser::get(int off)
{
	if (position + off >= tokens.size()) {
		return Token("", TokenType::EOf);
	}
	return tokens.at(position + off);
};

