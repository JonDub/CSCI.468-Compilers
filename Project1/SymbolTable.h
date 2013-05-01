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

	static enum Mode {
		MODE_IN,
		MODE_OUT,
		MODE_REFERENCE
	};

	struct Record {
		string name;
		Kind kind;
		Token token;
		Mode mode;
		int offset;
	};

	// Get nextOffset value and increment it
	int getNextOffset();

	// Insert a record into the top most table in the vector
	bool insertRecord(string name, Kind kind, Token token); // insert a Var type
	bool insertRecord(string name, Kind kind, Token token, Mode mode);

	// Returns a pointer to the record of information, returns a NULL pointer otherwise
	Record* lookupRecord(string name,Kind kind,int table = -1);	

	// Returns a pointer to the record of information, returns a NULL pointer otherwise version 
	// used to modify records externally, not very good encapsulation
	Record* lookupRecord(int offset, int table = -1);

	// Check to see if record is in table or not
	bool contains(string name, Kind kind);

	// Always creates a table at the top of the vector 
	// Returns true if inserted, false if already in table and not inserted
	bool createTable();

	// Always remove the top most table in the vector
	// Returns true if table was removed
	bool removeTable();

	// Returns the size of the specified table, or the size of the topmost table if no index is 
	// specified
	int tableSize(int table = -1);

private:

	struct Table {
		vector< Record* > records;
	};

	vector< Table* > tables;
	int nextOffset;  //Where is the symbol table record on the stack
	int activationRecordSize; //Total size of symbol table memory
	int nestingLevel;
};