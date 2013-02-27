#include "SymbolTable.h"


SymbolTable::SymbolTable(void)
{
}


SymbolTable::~SymbolTable(void)
{
}


// Insert a record into the top most table in the vector
bool SymbolTable::InsertRecord(Token token, string name, int row, int col)
{
	Record* r = LookupRecord(name);

	if (r == NULL){
		Table* t = tables.at(tables.size() - 1);
		t->records[name] = new Record(token, name, row, col); 
		return true;
	} else {
		return false;
	}
}

// Search the top most table in the vector, then its parent, and so on to the root table
Record* SymbolTable::LookupRecord(string name)
{
	unordered_map< string, Record* >::const_iterator p;

	for (int i = 0; i < tables.size(); i++)
	{
		Table* t = tables.at(tables.size() - 1 - i);
		p = t->records.find(name);

		if (p != t->records.end())
			return p->second;
		else
			continue;
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
