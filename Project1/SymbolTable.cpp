#include "SymbolTable.h"


SymbolTable::SymbolTable(void)
{
}


SymbolTable::~SymbolTable(void)
{
}


// Insert a record into the top most table in the vector
bool SymbolTable::insertRecord(Token token, string name, Token type)
{
	Record* r = lookupRecord(name);

	if (r == NULL){
		Table* t = tables.at(tables.size() - 1);
		t->records[name] = new Record(token, name, t->offset, type); 
		t->offset++;
		return true;
	} else {
		return false;	// could throw an error here if needed
	}
}

bool SymbolTable::contains(string s)
{
	if (lookupRecord(s) == NULL) return false; else return true;
}

// Search the top most table in the vector, then its parent, and so on to the root table
Record* SymbolTable::lookupRecord(string name)
{
	unordered_map< string, Record* >::const_iterator p;

	for (int i = 0; i < tables.size(); i++)
	{
		Table* t = tables.at(tables.size() - 1 - i);
		p = t->records.find(name);

		if (p != t->records.end())
			return p->second;
	}
	return NULL;
}



// Always creates a table at the top of the vector 
bool SymbolTable::createTable()
{	
	Table* t = new Table();
	t->offset = 0;
	tables.push_back(t);
	return true;
}

// Always remove the top most table in the vecotr
bool SymbolTable::removeTable()
{
	tables.pop_back();
	return true;
}
