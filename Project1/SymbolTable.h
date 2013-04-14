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
		int line;
		int col;
		string name;
		Token token;
		Type type;
	};

	// Insert a record into the top most table in the vector
	bool insertRecord(Token token, string name, int row, int col, Type type);

	// Search the top most table in the vector, then its parent, and so on
	// Search by name
	// Returns a pointer to the record of information, returns a NULL pointer otherwise
	Record* lookupRecord(string name, Type type);	

	// Check to see if record is in table or not
	bool contains(string name, Type type);

	// Always creates a table at the top of the vector 
	// Returns true if inserted, false if already in table and not inserted
	bool createTable();

	// Always remove the top most table in the vecotr
	// Returns true if table was removed
	bool removeTable();

private:
	
	struct Table {
		vector< Record* > records;
	};

	vector< Table* > tables;
};