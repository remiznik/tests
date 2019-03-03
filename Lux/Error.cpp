#include "pch.h"
#include "Error.h"
#include <iostream>

void error(int line, const std::string& message)
{
	std::cout << " Error : " + std::to_string(line) + "  " + message << std::endl;
}
