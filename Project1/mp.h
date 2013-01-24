#pragma once

#include <stdio.h>

class mp
{
public:
	mp(void);
	virtual ~mp(void);

	bool openFile(const char*);
	bool getToken();
	char getLexeme();
	int getLineNumber();
	int getColumnNumber();

protected:
private:
	FILE* file;

	bool handleAlpa();
	bool handleNumberic();
	bool handleSymbol();

	
};

