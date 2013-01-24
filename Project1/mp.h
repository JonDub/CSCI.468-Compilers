#pragma once

#include <stdio.h>

class mp
{
public:
	mp(const char*);
	virtual ~mp(void);

protected:
private:
	FILE* file;

	bool handleAlpa();
	bool handleNumberic();
	bool handleSymbol();
};

