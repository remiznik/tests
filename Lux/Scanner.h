#pragma once

#include "Token.h"

#include <string>
#include <vector>

class Scanner
{
public:
	Scanner(const std::string& source);
	~Scanner();

	std::vector<Token> scanTokens();

private:
	bool isAtEnd() const;
	void scanToken();
	char advance();
	void addToken(TokenType type);
	void addToken(TokenType type, const std::string& literal);
	bool match(char expected);
	char peek() const;
	void string();
	bool isDigit(char c) const;
	void number();
	char peekNext() const;
	bool isAlpha(char c) const;
	void identifier();
	bool isAlphaNumeric(char c) const;
private:
	std::string source_;
	std::vector<Token> tokens_;

	std::size_t start_{ 0 };
	std::size_t current_{ 0 };
	int line_{ 1 };
};

