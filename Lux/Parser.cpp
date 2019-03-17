#include "pch.h"
#include "Parser.h"
#include "Exprs.h"
#include "Error.h"

namespace
{
	void errorParser(Token token, const std::string& message)
	{
		if (token.type_ == TokenType::EOF_T)
		{
			std::string m("at end :");
			m.append(message);
			error(token.line_, m);
		}
		else
		{
			std::string m("at '");
			m.append(token.lexeme_);
			m.append("'");
			m.append(message);
			error(token.line_, m);
		}
	}
}
Parser::Parser(const std::vector<Token>& tokens) : tokens_(tokens)
{
}

Expr* Parser::expression()
{
	return equality();
}

Expr* Parser::equality()
{
	auto expr = comprasion();

	while (match({ BANG_EQUAL, BANG_EQUAL }))
	{
		Token opr = previos();
		Expr* right = comprasion();
		expr = new Binary(expr, opr, right);
	}

	return expr;	 
}

Expr* Parser::comprasion()
{
	Expr* expr = addition();
	while (match({ GREATER, GREATER_EQUAL, LESS, LESS_EQUAL }))
	{
		Token opr = previos();
		Expr* right = addition();
		expr = new Binary(expr, opr, right);
	}

	return expr;
}

Expr* Parser::addition()
{
	Expr* expr = multiplication();

	while (match({MINUS, PLUS}))
	{
		Token opr = previos();
		Expr* right = multiplication();
		expr = new Binary(expr, opr, right);
	}
	return expr;
}

Expr* Parser::multiplication()
{
	Expr* expr = unary();

	while (match({ SLASH, STAR }))
	{
		Token opr = previos();
		Expr* right = unary();
		expr = new Binary(expr, opr, right);
	}
	return expr;
}

Expr* Parser::unary()
{
	if (match({ BANG, MINUS }))
	{
		Token opr = previos();
		Expr* right = unary();
		return new Unary(opr, right);
	}
	return primary();
}

Expr* Parser::primary()
{
	if (match({ FALSE })) return new Literal("false");
	if (match({ TRUE })) return new Literal("true");
	if (match({ NIL })) return new Literal("nil");

	if (match({ NUMBER, STRING }))
	{
		return new Literal(previos().literal_);
	}

	if (match({ LEFT_PAREN }))
	{
		Expr* expr = expression();
		consume(RIGHT_PAREN, "Expect ')' after expression.");
		return new Grouping(expr);
	}
}

Token Parser::consume(TokenType type, const std::string& message)
{
	if (check(type)) return advance();
	errorParser(peek(), message);
	return Token(EOF_T, "error","error", -1);
}

bool Parser::match(const std::vector<TokenType>& types)
{
	for (auto type : types)
	{
		if (check(type))
		{
			advance();
			return true;
		}
	}
	return false;
}

bool Parser::check(TokenType type) const
{
	if (isAtEnd()) return false;
	return peek().type_ == type;
}

Token Parser::advance()
{
	if (!isAtEnd()) current_++;
	return previos();
}

bool Parser::isAtEnd() const
{
	return peek().type_ == EOF_T;
}

Token Parser::peek() const
{
	return tokens_[current_];
}

Token Parser::previos() const
{
	return tokens_[current_ - 1];
}


Parser::~Parser()
{
}
