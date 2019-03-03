#pragma once
#include <string>
class Lux
{
public:
	Lux();

	void runFile(const char* fileName);
	void runPromt();

private:
	void runCode(const std::string& source);
};
