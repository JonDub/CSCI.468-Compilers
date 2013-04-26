#include "SemanticRecord.h"


SemanticRecord::SemanticRecord(void)
{
}

SemanticRecord::~SemanticRecord(void)
{
}

void SemanticRecord::add(string s)
{
	//records.push_back(s);
}

string SemanticRecord::get()
{
	string v = records.back();
	records.pop_back();
	return v;
}

