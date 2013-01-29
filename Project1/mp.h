#pragma once

#include <stdio.h>
#include <fstream>
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

	// File Operations
	char peek();
	char get();
	void seek(int);

	// FSA's
	string handleAlpa();
	string handleNumberic();
	string handleSymbol();	
	string handleComment();
};

