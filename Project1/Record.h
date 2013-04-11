#pragma once

#include "Tokens.h"
#include <string>

using namespace std;

class Record
{
public:
	Record(void);
	Record(Token, string, int, int, int);
	~Record(void);

	int line(){ return _line; };
	int col(){ return _col; };
	int offset(){ return _offset; };
	string name(){ return _name; };
	Token token(){ return _token; };

private:
	int _offset;
	int _line;
	int _col;
	string _name; // more or less the lexeme
	Token _token;

};

