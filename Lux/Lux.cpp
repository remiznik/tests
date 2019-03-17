#include "pch.h"
#include "Lux.h"
#include "Scanner.h"
#include "Parser.h"
#include "AstPrinter.h"

#include <fstream>
#include <sstream>
#include <iostream>

Lux::Lux()
{}

void Lux::runFile(const char* fileName)
{
	std::fstream file(fileName);
	std::stringstream buffer;
	buffer << file.rdbuf();

	runCode(fileName);
}

void Lux::runPromt()
{
}

void Lux::runCode(const std::string& source)
{
	Scanner scanner(source);
	auto tokens = scanner.scanTokens();
	//for (auto token : tokens)
	//{
	//	std::cout << token.toString() << std::endl;
	//}
	Parser parser(tokens);
	Expr* expr = parser.expression();
	AstPrinter printer;
	std::cout << printer.print(expr);

}