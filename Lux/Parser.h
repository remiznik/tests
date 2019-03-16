#pragma once

#include <vector>

#include "Token.h"

class Expr;

class Parser
{
public:
	Parser(const std::vector<Token>& tokens);
	~Parser();

	Expr* expression();
	Expr* equality();
	Expr* comprasion();
	Expr* addition();
	Expr* multiplication();
	Expr* unary();
	Expr* primary();

private: 
	bool match(const std::vector<TokenType>& types);
	bool check(TokenType type) const;
	Token advance();
	bool isAtEnd() const;
	Token peek() const;
	Token previos() const;

private:
	size_t current_{ 0 };
	std::vector<Token> tokens_;
};

