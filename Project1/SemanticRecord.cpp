#include "SemanticRecord.h"


SemanticRecord::SemanticRecord(void)
{
}

SemanticRecord::~SemanticRecord(void)
{
}

void SemanticRecord::add(string s)
{
<<<<<<< HEAD
	records.push_back(s);
=======
	//records.push_back(s);
>>>>>>> origin/SarahParserFollowChange
}

string SemanticRecord::get()
{
	string v = records.back();
	records.pop_back();
	return v;
}

