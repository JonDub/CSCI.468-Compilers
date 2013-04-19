#pragma once

#include "Tokens.h"
#include "SymbolTable.h"

class SemanticRecord
{
public:
	static enum RecordType { 
		ASSIGNMENT
	};

	SemanticRecord(void);
	virtual ~SemanticRecord(void);

	void add(string r);
	string get();
	RecordType type;

private:
	
	vector< string > records;
	
};

