#include "mp.h"


mp::mp()
{
	// constructor
}

mp::~mp(void)
{
	// destructor. release resources
	fclose(file);
}

bool mp::openFile(const char* fName)
{
	// Opens the file pointer for FSA's to use
	file = fopen(fName, "r");

	if (!file)
		return false;
	return true;
}

bool mp::getToken()
{
	return true;
};

char mp::getLexeme()
{
	return 'a';
};

int mp::getLineNumber()
{
	return 0;
};

int mp::getColumnNumber()
{
	return 0;
};