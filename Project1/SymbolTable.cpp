#include "SymbolTable.h"


SymbolTable::SymbolTable(void)
{
}


SymbolTable::~SymbolTable(void)
{
	tables.clear();
}


// Insert a record into the top most table in the vector
bool SymbolTable::insertRecord(string name, SymbolTable::Type type, Token token, int row, int col)
{
	Record* r;

	switch (type)
	{
	case SymbolTable::TYPE_PROCEDURE:
	case SymbolTable::TYPE_FUNCTION:
		// add the function to the main symbol table 0
		r = lookupRecord(name, type, 0);

		if (r == NULL)
		{
			r = new Record();
			Table* t = tables.at(0);
			r->col = col;
			r->line = row;
			r->name = name;
			r->token = token;
			r->type = type;
			t->records.push_back(r);
		}
		// do not put break here
	case SymbolTable::TYPE_VARIABLE:
		r = lookupRecord(name, type);

		if (r == NULL){
			Table* t = tables.back();
			r = new Record();
			r->col = col;
			r->line = row;
			r->name = name;
			r->token = token;
			r->type = type;
			t->records.push_back(r);
			return true;
		}
		break;
	default:
		return false;
	}
	return true;
}

bool SymbolTable::contains(string s, Type t)
{
	if (lookupRecord(s, t) == NULL) return false; else return true;
}

// Search the top most table in the vector, then its parent, and so on to the root table
SymbolTable::Record* SymbolTable::lookupRecord(string name, Type type, int table)
{
	// if a table number is specified then search that table, otherwise just search the top table
	if (table > -1 && table < tables.size())
	{
		Table* t = tables.at(table);

		// search the table in reverse (start at the end of table)
		for (int j = t->records.size() - 1; j >= 0; j--)
		{
			if ((strcmp(t->records.at(j)->name.c_str(), name.c_str()) == 0 ) && (t->records.at(j)->type == type) )
				return t->records.at(j);
		}
	}
	else // search through the top table in reverse order
	{
		Table* t = tables.back();

		for (int j = t->records.size() - 1; j >= 0; j--)
		{
			if ((strcmp(t->records.at(j)->name.c_str(), name.c_str()) == 0 ) && (t->records.at(j)->type == type) )
				return t->records.at(j);
		}
	}
	return NULL;
}


SymbolTable::Record* SymbolTable::lookupRecord(int offset, int table)
{
	// if a table number is specified then search that table, otherwise just search the top table
	if (table != -1)
	{
		if (table < tables.size() && table >= 0)
		{
			if (offset < tables.at(table)->records.size() && offset >= 0)
			{
				return tables.at(table)->records.at(offset);
			}
		}
	}
	else 
	{
		if (offset < tables.back()->records.size() && offset >= 0)
		{
			return tables.back()->records.at(offset);
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


int SymbolTable::tableSize(int t)
{
	// get the size of the topmost table or a specific table
	if (t > -1 && t < tables.size())
	{
		return tables.at(t)->records.size();
	} 
	else 
	{
		return tables.back()->records.size();
	}
}