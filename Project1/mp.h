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
	char getLexeme();
	int getLineNumber();
	int getColumnNumber();
	bool hasToken();

protected:
private:
	ifstream file;

	string handleAlpa();
	string handleNumberic();
	string handleSymbol();	
};

