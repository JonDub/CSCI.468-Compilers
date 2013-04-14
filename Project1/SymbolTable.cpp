#include "SymbolTable.h"


SymbolTable::SymbolTable(void)
{
}


SymbolTable::~SymbolTable(void)
{
	tables.clear();
}


// Insert a record into the top most table in the vector
bool SymbolTable::insertRecord(Token token, string name, int row, int col, SymbolTable::Type type)
{
	Record* r = lookupRecord(name, type);

	if (r == NULL){
		Table* t = tables.at(tables.size() - 1);
		Record* r = new Record();
		r->col = col;
		r->line = row;
		r->name = name;
		r->token = token;
		r->type = type;
		t->records.push_back(r); 
		return true;
	} else {
		return false;	// could throw an error here if needed
	}
}

bool SymbolTable::contains(string s, Type t)
{
	if (lookupRecord(s, t) == NULL) return false; else return true;
}

// Search the top most table in the vector, then its parent, and so on to the root table
SymbolTable::Record* SymbolTable::lookupRecord(string name, Type type)
{
	for (int i = tables.size() - 1; i >= 0; i--)
	{
		Table* t = tables.at(i);

		for (int j = t->records.size() - 1; j >= 0; j--)
		{
			if ((strcmp(t->records.at(j)->name.c_str(), name.c_str()) == 0 ) && (t->records.at(j)->type == type) )
				return t->records.at(j);
		}
	}
	return NULL;
}



// Always creates a table at the top of the vector 
bool SymbolTable::createTable()
{	
	tables.push_back(new Table());
	return true;
}

// Always remove the top most table in the vecotr
bool SymbolTable::removeTable()
{
	tables.pop_back();
	return true;
}
