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
		SIMPLE_PARAMETER
	};

	SemanticRecord(void);
	virtual ~SemanticRecord(void);

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
};

