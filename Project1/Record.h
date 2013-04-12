#pragma once

#include "Tokens.h"
#include <string>

using namespace std;

class Record
{
public:
	Record(void);
	Record(Token, string, int, Token);
	~Record(void);

	int line(){ return _line; };
	int col(){ return _col; };
	int offset(){ return _offset; };
	string name(){ return _name; };
	Token token(){ return _token; };
	Token type(){ return _type; };

private:
	int _offset;
	int _line;
	int _col;
	string _name; // more or less the lexeme
	Token _token;
	Token _type;

};

