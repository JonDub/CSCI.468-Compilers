#pragma once

#include "Tokens.h"
#include "SymbolTable.h"

class SemanticRecord
{
public:
	static enum RecordKind { 
		ASSIGNMENT,
		DECLARATION,
		FUNCTION_CALL,
		PROCEDURE_CALL,
		PROGRAM,
		VARIABLE,
		PARAMETER
	};

	//static enum RecordType {
	//	MP_INTEGER_LIT,
	//	MP_FIXED_LIT,
	//	MP_FLOAT_LIT, 
	//	MP_NULL	// This is questionable

	//};
	SemanticRecord(void);
	virtual ~SemanticRecord(void);

	void add(string r);
	string get();
	//RecordType type;

	void setKind(RecordKind _kind)
	{
		kind=_kind;
	}
	void setType(Token _type)
	{
		type=_type;
	}

	RecordKind getKind( )
	{
		return kind;
	}
	Token getType()
	{
		return type;
	}
private:
	RecordKind kind;
	Token type;

	vector< string > records;
	
};

