#include "SemanticAnalyzer.h"


SemanticAnalyzer::SemanticAnalyzer(SymbolTable* tbl)
{
	// open the log file 
	irFilename = "uCode.pas";
	irFile.open(irFilename, ofstream::out);

	write("MOV SP D0");
}


SemanticAnalyzer::~SemanticAnalyzer(void)
{
	write("MOV D0 SP");
	write("HLT");
	irFile.close();
}


void SemanticAnalyzer::genAssign(SemanticRecord* rec)
{
	write(rec->get());
}

void SemanticAnalyzer::write(string msg)
{
	irFile << msg << endl;
}