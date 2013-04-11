#include "Record.h"


Record::Record(void)
{
	_token = MP_NULL;
	_name = "";
	_line = -1;
	_col = -1;
}

Record::Record(Token t, string n, int l, int c, int off)
{
	_token = t;
	_name = n;
	_line = l;
	_col = c;
	_offset = off;
}

Record::~Record(void)
{
}
