#pragma once

#include <unordered_map>;
#include "Tokens.h";
#include "Record.h";
#include <string>;
#include <vector>;

using namespace std;

class SymbolTable
{
public:
	SymbolTable();
	~SymbolTable();

	// Insert a record into the top most table in the vector
	bool InsertRecord(Token, string, int, int);

	// Search the top most table in the vector, then its parent, and so on
	// Search by name
	// Returns a pointer to the record of information, returns a NULL pointer otherwise
	Record* LookupRecord(string);	

	// Always creates a table at the top of the vector 
	// Returns true if inserted, false if already in table and not inserted
	bool CreateTable();

	// Always remove the top most table in the vecotr
	// Returns true if table was removed
	bool RemoveTable();

private:
	// Table structure that contains the records for each scope
	struct Table {
		unordered_map < string , Record* > records;
	};	

	// Vector of tables, each one in the scope of its parent
	vector< Table* > tables;
};