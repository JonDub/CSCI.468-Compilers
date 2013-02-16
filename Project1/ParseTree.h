#pragma once

#include <string>
#include <fstream>
#include <vector>
#include "Tokens.h"

class ParseTree
{
public:
	ParseTree(std::string);
	~ParseTree(void);

	bool ReadCFGRules(std::string);
	void LogExpansion(int);

private:
	std::ofstream outFile; // outfile to write parse tree
	std::vector<std::string> rules;
		
};

