#include "Record.h"


Record::Record(void)
{
	_token = MP_NULL;
	_name = "";
	_row = -1;
	_col = -1;
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
