#include "mp.h"
//#include <stdio.h>


mp::mp(const char* fName)
{
	// constructor. opens the file pointer for all FSA's to use
	FILE* file = fopen(fName, "r");
}


mp::~mp(void)
{
	// destructor. release resources
	fclose(file);
}
