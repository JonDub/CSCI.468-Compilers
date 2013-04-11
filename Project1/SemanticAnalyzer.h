#pragma once

#include <string>
#include <fstream>
#include "SemanticRecord.h"
#include "Tokens.h"
using namespace std;

class SemanticAnalyzer
{
public:
	SemanticAnalyzer(void);
	~SemanticAnalyzer(void);

	void genAssign(SemanticRecord*);

private:
	string irFilename;
	std::ofstream irFile;

	void write(string);
};

