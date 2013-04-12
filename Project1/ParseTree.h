#pragma once

#include <string>
#include <fstream>
#include <vector>
#include "Tokens.h"
using namespace std;

class ParseTree
{
public:
	ParseTree(string);
	~ParseTree(void);

	bool ReadCFGRules(string);
	void LogExpansion(int);
	void LogMessage(string);

private:
	ofstream outFile; // outfile to write parse tree
	vector<string> rules;
		
};

