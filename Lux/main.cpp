#include "pch.h"
#include <iostream>
#include "Lux.h"

int main(int argc, char* argv[])
{
	Lux lux;
	if (argc == 2)
	{
		lux.runFile(argv[1]);
	}
	else
	{
		lux.runPromt();
	}
}
