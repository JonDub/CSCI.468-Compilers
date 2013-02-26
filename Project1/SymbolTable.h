#pragma once

#include "Tokens.h";
#include "Record.h";
#include <string>
#include <unordered_map>
#include <vector>

class SymbolTable
{
public:
	SymbolTable(void);
	~SymbolTable(void);

	// Insert a record into the top most table in the vector
	int InsertRecord(Record);

	// Search the top most table in the vector, then its parent, and so on
	Record LookupRecord(std::string name, Token token);	
	
	// Always creates a table at the top of the vector 
	bool CreateTable();

	// Always remove the top most table in the vecotr
	bool RemoveTable();

private:	
	// Our table structure that contains the records
	struct Table {
		std::unordered_map <int, Record> records;
	};	

	// Vector of tables, each one in the scope of its parent
	std::vector < Table > tables;

};

