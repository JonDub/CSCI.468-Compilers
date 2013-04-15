#include "SemanticAnalyzer.h"


SemanticAnalyzer::SemanticAnalyzer(SymbolTable* tbl)
{
	// open the log file 
	irFilename = "uCode.pas";
	irFile.open(irFilename, ofstream::out);
}


SemanticAnalyzer::~SemanticAnalyzer(void)
{
	irFile.close();
}


void SemanticAnalyzer::genAssign(SemanticRecord* rec)
{

}

void SemanticAnalyzer::write(string msg)
{
	irFile << msg << endl;
}