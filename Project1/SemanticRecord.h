#pragma once

#include "Tokens.h"
#include "SymbolTable.h"

class SemanticRecord
{
public:
<<<<<<< HEAD
	static enum RecordType { 
		ASSIGNMENT
	};

=======
	static enum RecordKind { 
		ASSIGNMENT,
		DECLARATION,
		FUNCTION_CALL,
		PROCEDURE_CALL,
		PROGRAM,
		VARIABLE,
		SIMPLE_PARAMETER
	};

	//static enum RecordType {
	//	MP_INTEGER_LIT,
	//	MP_FIXED_LIT,
	//	MP_FLOAT_LIT, 
	//	MP_NULL	// This is questionable

	//};
>>>>>>> origin/SarahParserFollowChange
	SemanticRecord(void);
	virtual ~SemanticRecord(void);

	void add(string r);
	string get();
<<<<<<< HEAD
	RecordType type;

private:
	
=======
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

>>>>>>> origin/SarahParserFollowChange
	vector< string > records;
	
};

