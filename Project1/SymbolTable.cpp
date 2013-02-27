#include "SymbolTable.h"


SymbolTable::SymbolTable(void)
{
}


SymbolTable::~SymbolTable(void)
{
}


// Insert a record into the top most table in the vector
int SymbolTable::InsertRecord(Token token, string name, int row, int col)
{
	Table* t = tables.at(tables.size() - 1);
	//t->records[pair< Token, string >(token, name)] = new Record(token, name, row, col); 
	t->records[ token] = new Record(token, name, row, col); 
	return 1;
}

// Search the top most table in the vector, then its parent, and so on
Record* SymbolTable::LookupRecord(Token token, string name)
{
	Table* t = tables.at(tables.size() - 1);
	unordered_map< Token, Record* >::const_iterator p = t->records.find(token);

	if (p != t->records.end()) {
		return p->second;
	}
	return NULL;
}

// Always creates a table at the top of the vector 
bool SymbolTable::CreateTable()
{	
	tables.push_back(new Table);
	return true;
}

// Always remove the top most table in the vecotr
bool SymbolTable::RemoveTable()
{
	tables.pop_back();
	return true;
}
