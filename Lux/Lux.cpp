#include "pch.h"
#include "Lux.h"
#include "Scanner.h"

#include <fstream>
#include <sstream>

Lux::Lux()
{}

void Lux::runFile(const char* fileName)
{
	std::fstream file(fileName);
	std::stringstream buffer;
	buffer << file.rdbuf();

	runCode(buffer.str());
}

void Lux::runPromt()
{
}

void Lux::runCode(const std::string& source)
{
	Scanner scanner(source);
	auto tokens = scanner.scanTokens();

}