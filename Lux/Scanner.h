#pragma once

#include "Token.h"

#include <vector>

class Scanner
{
public:
	Scanner();
	~Scanner();

	std::vector<Token> scanTokens(const std::string& soucrce);
};

