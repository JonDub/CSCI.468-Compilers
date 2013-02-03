#pragma once

#include <unordered_map>
#include <string>
#include <stdio.h>
#include <fstream>
#include <ctype.h>
using namespace std;

class mp
{
public:
	mp(void);
	virtual ~mp(void);

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
	bool error;
	unordered_map < string, string> reservedWords;

	// File Operations and Utilities
	char peek();
	char get();
	void seek(int);

	// FSA's
	string handleAlpha();
	string handleNumberic();
	string handleSymbol();	
	string handleComment();
	string handleString();
	string handleWord();
	bool isReservedWord(string);
};

