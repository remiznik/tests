#include "pch.h"
#include <iostream>
#include "Lux.h"

int main(int argc, char* argv[])
{
	Lux lux;
	if (argc == 1)
	{
		lux.runFile("var language = \"lox\";");
	}
	else
	{
		lux.runPromt();
	}
}
