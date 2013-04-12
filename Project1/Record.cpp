#include "Record.h"


Record::Record(void)
{
	_token = MP_NULL;
	_name = "";
	_line = -1;
	_col = -1;
}

Record::Record(Token t, string n, int off, Token type)
{
	_token = t;
	_name = n;
	_line = 0;
	_col = 0;
	_offset = off;
	_type = type;
}

Record::~Record(void)
{
}
