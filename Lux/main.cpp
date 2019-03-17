#include "pch.h"
#include <iostream>
#include "Lux.h"
#include "Exprs.h"
#include "Token.h"
#include "AstPrinter.h"

int main(int argc, char* argv[])
{
	Lux lux;
	if (argc == 1)
	{
		lux.runFile("(1-3) * 7");
		//Expr* expression = new Binary(
		//	new Unary(
		//		Token(TokenType::MINUS, "-", "", 1),
		//		new Literal("123")),
		//	Token(TokenType::STAR, "*", "", 1),
		//	new Grouping(
		//		new Literal("45.67")));
		//Expr* exp = new Binary(new Binary(new Literal("1"), Token(TokenType::PLUS, "+", "", 1), new Literal("2")), 
		//	Token(TokenType::DOT, "*", "",1), new Binary(new Literal("4"), Token(TokenType::MINUS, "-", "", 1), new Literal("1")));
		//AstPrinter printer;
		//std::cout << printer.print(exp);
	}
	else
	{
		lux.runPromt();
	}
}
