#pragma once

#include <unordered_map>
#include "Tokens.h"
#include "Record.h"
#include <string>
#include <vector>

using namespace std;

class SymbolTable
{
public:
	SymbolTable();
	~SymbolTable();

	// Insert a record into the top most table in the vector
	bool insertRecord(Token token, string name, Token type);

	// Search the top most table in the vector, then its parent, and so on
	// Search by name
	// Returns a pointer to the record of information, returns a NULL pointer otherwise
	Record* lookupRecord(string);	

	// Check to see if record is in table or not
	bool contains(string);

	// Always creates a table at the top of the vector 
	// Returns true if inserted, false if already in table and not inserted
	bool createTable();

	// Always remove the top most table in the vecotr
	// Returns true if table was removed
	bool removeTable();

private:
	// Table structure that contains the records for each scope
	struct Table {
		unordered_map < string , Record* > records;
		int offset;
	};	

	// Vector of tables, each one in the scope of its parent
	vector< Table* > tables;
};