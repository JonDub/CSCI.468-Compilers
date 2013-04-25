#pragma once

#include <unordered_map>
#include "Tokens.h"
#include <string>
#include <vector>

using namespace std;

class SymbolTable
{
public:
	SymbolTable();
	~SymbolTable();

	static enum Kind { 
		KIND_VARIABLE,
		KIND_FUNCTION,
		KIND_PROCEDURE
	};

	struct Record {
		string name;
		Kind kind;
		Token token;
		int offset;
		int line;
		int col;		
	};

	// Insert a record into the top most table in the vector
	bool insertRecord(string name, Kind kind, Token token = MP_NULL, int row = 0, int col = 0,int ofset=0);

	// Returns a pointer to the record of information, returns a NULL pointer otherwise
	Record* lookupRecord(string name, Kind kind, int table = -1);	
	Record* lookupRecord(int offset, int table = -1);

	// Check to see if record is in table or not
	bool contains(string name, Kind kind);

	// Always creates a table at the top of the vector 
	// Returns true if inserted, false if already in table and not inserted
	bool createTable();

	// Always remove the top most table in the vecotr
	// Returns true if table was removed
	bool removeTable();

	// Returns the size of the specified table, or the size of the topmost table if no index is specified
	int tableSize(int table = -1);

private:

	struct Table {
		vector< Record* > records;
	};

	vector< Table* > tables;

};