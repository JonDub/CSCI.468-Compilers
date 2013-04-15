#pragma once

#include <string>
#include <fstream>
#include "SemanticRecord.h"
#include "SymbolTable.h"
#include "Tokens.h"
using namespace std;

class SemanticAnalyzer
{
public:
	SemanticAnalyzer(SymbolTable* table);
	~SemanticAnalyzer(void);

	void genAssign(SemanticRecord*);

private:
	string irFilename;
	std::ofstream irFile;
	SymbolTable* symbolTable;

	void write(string);
};

