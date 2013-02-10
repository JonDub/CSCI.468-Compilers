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

	bool openFile(const char*);
	string getToken();
	string getLexeme();
	unsigned int getLineNumber();
	unsigned int getColumnNumber();
	bool hasToken();

protected:
private:
	// Variables
	ifstream file;
	unsigned int lines;
	unsigned int cols;
	string token;
	string lexeme;
	string lookahead;
	unordered_map <string, string> tokens;

	// File Operations and Utilities
	char peek();
	char get();
	void seek(int);

	// FSA's
	string handleNumberic();
	string handleSymbol();	
	string handleComment();
	string handleString();
	string handleWord();
	
	bool isReservedWord(string);
};

