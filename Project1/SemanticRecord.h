#pragma once

#include "Tokens.h"

class SemanticRecord
{
public:
	SemanticRecord(void);
	virtual ~SemanticRecord(void);

private:
	static enum RecordType { 
		ASSIGNMENT
	} Type;
};

