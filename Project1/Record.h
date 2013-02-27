#pragma once

#include "Tokens.h"
#include <string>

using namespace std;

class Record
{
public:
	Record(void);
	Record(Token, string, int, int);
	~Record(void);

	int Row(){ return _row; };
	int Col(){ return _col; };
	string Lexeme(){ return _name; };
	Token token(){ return _token; };

private:
	int _row;
	int _col;
	string _name;
	Token _token;

};

