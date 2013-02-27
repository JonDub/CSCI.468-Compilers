#include "Record.h"


Record::Record(void)
{
}

Record::Record(Token t, string n, int r, int c)
{
	_token = t;
	_name = n;
	_row = r;
	_col = c;
}

Record::~Record(void)
{
}
