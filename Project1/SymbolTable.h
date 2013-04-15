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

	static enum Type { 
		TYPE_VARIABLE,
		TYPE_FUNCTION,
		TYPE_PROCEDURE
	};

	struct Record {
		string name;
		Type type;
		Token token;
		int line;
		int col;		
	};

	// Insert a record into the top most table in the vector
	bool insertRecord(string name, Type type, Token token = MP_NULL, int row = 0, int col = 0);

	// Returns a pointer to the record of information, returns a NULL pointer otherwise
	Record* lookupRecord(string name, Type type, int table = -1);	
	Record* lookupRecord(int offset, int table = -1);

	// Check to see if record is in table or not
	bool contains(string name, Type type);

	// Always creates a table at the top of the vector 
	// Returns true if inserted, false if already in table and not inserted
	bool createTable();

	// Always remove the top most table in the vecotr
	// Returns true if table was removed
	bool removeTable();

	int tableSize(int table = -1);

private:

	struct Table {
		vector< Record* > records;
	};

	vector< Table* > tables;

};