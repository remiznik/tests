#include "pch.h"
#include "Token.h"

Token::Token(TokenType type, const std::string & lexeme, const std::string & literal, int line) :
	type_(type), lexeme_(lexeme), literal_(literal), line_(line)
{
}

Token::~Token()
{
}

std::string Token::toString() const
{
	return  std::to_string(type_) + " " + lexeme_ + " " + literal_;
}