#pragma once

#include <unordered_map>
#include <string>
#include <fstream>
#include "Tokens.h"
using namespace std;

class Scanner
{
public:
	Scanner(void);
	virtual ~Scanner(void);

	bool openFile(string);
	Token getToken(); // retrieves the next token
	string getLexeme();	// returns the current lexeme
	Token token();	// returns the current token
	unsigned int getLineNumber(); // returns the current line counter
	unsigned int getColumnNumber();	// returns the current column counter
	bool hasToken();

	string _lexeme; // only for debugging

protected:
private:
	// Variables
	ifstream file;
	unsigned int lines;
	unsigned int cols;
	Token _token;
	//string _lexeme;
	Token lookahead;
	unordered_map <string, Token> tokens;

	// File Operations and Utilities
	char peek();
	char get();
	void seek(int);

	// FSA's
	Token handleNumberic();
	Token handleSymbol();	
	Token handleComment();
	Token handleString();
	Token handleWord();
	
	bool isReservedWord(string);
};

