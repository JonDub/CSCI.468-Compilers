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

<<<<<<< HEAD
=======
	struct Record {
		string name;
		Kind kind;
		Token token;
		int line;
		int col;		
>>>>>>> ade8d45438b09c021693e5aed9b3eafddbde03e5
	};

	struct Record {
		string name; //Might want to change this actually be lexeme to match Rocky's Pattern
		Kind kind;
		Token token; //Might want to change this actually be type to match Rocky's Pattern
		Mode mode;
		int offset;
			
	};
	// Get nextOffset value and increment it
	int getNextOffset();
	
	// Insert a record into the top most table in the vector
<<<<<<< HEAD
	bool insertRecord(string name, Kind kind, Token token); // insert a Var type
	bool insertRecord(string name, Kind kind, Token token, Mode mode);

	// Returns a pointer to the record of information, returns a NULL pointer otherwise
	Record* lookupRecord(string name,Kind kind,int table = -1);	
=======
	bool insertRecord(string name, Kind kind, Token token = MP_NULL, int row = 0, int col = 0);

	// Returns a pointer to the record of information, returns a NULL pointer otherwise
	Record* lookupRecord(string name, Kind kind, int table = -1);	
>>>>>>> ade8d45438b09c021693e5aed9b3eafddbde03e5
	Record* lookupRecord(int offset, int table = -1);

	// Check to see if record is in table or not
	bool contains(string name, Kind kind);

	// Always creates a table at the top of the vector 
	// Returns true if inserted, false if already in table and not inserted
	bool createTable();

	// Always remove the top most table in the vector
	// Returns true if table was removed
	bool removeTable();

	// Returns the size of the specified table, or the size of the topmost table if no index is specified
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