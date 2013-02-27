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
	int InsertRecord(Token, string, int, int);

	// Search the top most table in the vector, then its parent, and so on
	Record* LookupRecord(Token, string);	

	// Always creates a table at the top of the vector 
	bool CreateTable();

	// Always remove the top most table in the vecotr
	bool RemoveTable();

private:

	/*struct PairHash
	{
		size_t operator()(const pair<Token, string> &pair) const
		{
			return static_cast<size_t>(pair.first)
				* static_cast<size_t>(Token::MP_WRITE)
				+ static_cast<size_t>(pair.second)
		}
	};*/

	// Our table structure that contains the records
	struct Table {
		unordered_map < Token , Record* > records;
		//unordered_map < pair< Token, string > , Record* > records;
	};	

	// Vector of tables, each one in the scope of its parent
	vector< Table* > tables;
};