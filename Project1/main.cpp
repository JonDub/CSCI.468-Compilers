// Driver for the application
#include <iostream>
#include <iomanip>
#include <string>
#include "mp.h"
using namespace std;


int main ( int argc, char* argv[] )
{
	// name of the file to parse through
	const char* fName = argv[1];

	mp* dispatcher = new mp();
	dispatcher->openFile(fName);

	cout << setw(30) << left << "Token" << setw(20) << "Line Number" << "Column Number" << endl;
	while (dispatcher->hasToken())
	{
		cout << setw(30) << left << dispatcher->getToken() << setw(20) << dispatcher->getLineNumber()
			<< dispatcher->getColumnNumber() << endl;
	}

	// press enter to exit
	printf("Done. Press Enter to exit.\n");
	std::cin.get();
	return 0;
}