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

	bool openFile(std::string);
	Token getToken();
	string getLexeme();
	unsigned int getLineNumber();
	unsigned int getColumnNumber();
	bool hasToken();

	string lexeme; // only for debugging

protected:
private:
	// Variables
	ifstream file;
	unsigned int lines;
	unsigned int cols;
	Token token;
	//string lexeme;
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

