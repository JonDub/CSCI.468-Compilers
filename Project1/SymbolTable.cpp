#include "SymbolTable.h"


SymbolTable::SymbolTable(void)
{
}


SymbolTable::~SymbolTable(void)
{
}


// Insert a record into the top most table in the vector
int SymbolTable::InsertRecord(Record)
{
	return 0;
}

// Search the top most table in the vector, then its parent, and so on
Record SymbolTable::LookupRecord(std::string name, Token type)
{
	Record n;
	return n;
}

// Always creates a table at the top of the vector 
bool SymbolTable::CreateTable()
{
	Table t;	
	tables.push_back(t);
	return 0;
}

// Always remove the top most table in the vecotr
bool SymbolTable::RemoveTable()
{
	return true;
}
