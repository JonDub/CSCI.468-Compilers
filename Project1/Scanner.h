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
	Token getToken();				// Retrieves the next token
	string getLexeme();				// Returns the current lexeme
	Token token();					// Returns the current token
	unsigned int getLineNumber();	// Returns the current line counter
	unsigned int getColumnNumber();	// Returns the current column counter
	bool hasToken();

protected:
private:
	// Variables
	ifstream file;
	unsigned int lines;
	unsigned int cols;
	Token _token;
	string _lexeme;
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

