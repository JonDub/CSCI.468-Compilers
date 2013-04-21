#include "SymbolTable.h"
#include "Tokens.h"







SymbolTable::SymbolTable()
{
	nextOffset=0;
	activationRecordSize=0;
	
}


SymbolTable::~SymbolTable(void)
{
	tables.clear();
}

int SymbolTable::getNextOffset()
{
	int tempOffset=nextOffset;
	nextOffset++;
	return tempOffset;

}

// Insert a record into the top most table in the vector
<<<<<<< HEAD
bool SymbolTable::insertRecord(string name, SymbolTable::Kind kind, Token token)
=======
bool SymbolTable::insertRecord(string name, SymbolTable::Kind kind, Token token, int row, int col)
>>>>>>> ade8d45438b09c021693e5aed9b3eafddbde03e5
{
	Record* r;

	switch (kind)
	{
	case SymbolTable::KIND_PROCEDURE:
	case SymbolTable::KIND_FUNCTION:
		// add the function to the main symbol table 0
<<<<<<< HEAD
		r = lookupRecord(name,kind);
=======
		r = lookupRecord(name, kind, 0);
>>>>>>> ade8d45438b09c021693e5aed9b3eafddbde03e5

		if (r == NULL)
		{
			r = new Record();
			Table* t = tables.at(0);
			//r->col = col;
			//r->line = row;
			r->name = name;
			r->token = token;
			r->kind = kind;
<<<<<<< HEAD
			//r->offset=nextOffset; nextOffset++;
=======
>>>>>>> ade8d45438b09c021693e5aed9b3eafddbde03e5
			t->records.push_back(r);
		}
		// do not put break here
	case SymbolTable::KIND_VARIABLE:
		r = lookupRecord(name, kind);

		if (r == NULL){
			Table* t = tables.back();
			r = new Record();
			r->name = name;
			r->token = token;
			r->kind = kind;
<<<<<<< HEAD
			r->offset= getNextOffset();
			t->records.push_back(r);
			activationRecordSize++;
=======
			t->records.push_back(r);
>>>>>>> ade8d45438b09c021693e5aed9b3eafddbde03e5
		}
		break;
	default:
		return false;
	}
	return true;
}

bool SymbolTable::contains(string s, Kind k)
{
	if (lookupRecord(s, k) == NULL) return false; else return true;
}

// Search the top most table in the vector, then its parent, and so on to the root table
<<<<<<< HEAD
SymbolTable::Record* SymbolTable::lookupRecord(string name, Kind kind,int table)
=======
SymbolTable::Record* SymbolTable::lookupRecord(string name, Kind kind, int table)
>>>>>>> ade8d45438b09c021693e5aed9b3eafddbde03e5
{
	// if a table number is specified then search that table, otherwise just search the top table
	if (table > -1 && table < tables.size())
	{
		Table* t = tables.at(table);

		// search the table in reverse (start at the end of table)
		for (int j = t->records.size() - 1; j >= 0; j--)
		{
<<<<<<< HEAD
			if ((strcmp(t->records.at(j)->name.c_str(), name.c_str()) == 0 ) /*&& (t->records.at(j)->kind == kind)*/ )
=======
			if ((strcmp(t->records.at(j)->name.c_str(), name.c_str()) == 0 ) && (t->records.at(j)->kind == kind) )
>>>>>>> ade8d45438b09c021693e5aed9b3eafddbde03e5
				return t->records.at(j);
		}
	}
	else // search through the top table in reverse order
	{
		Table* t = tables.back();

		for (int j = t->records.size() - 1; j >= 0; j--)
		{
<<<<<<< HEAD
			if ((strcmp(t->records.at(j)->name.c_str(), name.c_str()) == 0 ) /*&& (t->records.at(j)->kind == kind)*/ )
=======
			if ((strcmp(t->records.at(j)->name.c_str(), name.c_str()) == 0 ) && (t->records.at(j)->kind == kind) )
>>>>>>> ade8d45438b09c021693e5aed9b3eafddbde03e5
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

// Always remove the top most table in the vector
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

