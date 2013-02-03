// Driver for the application
#include <iostream>
#include <iomanip>
#include <string.h>
#include <string>
#include "mp.h"
using namespace std;


int main ( int argc, char* argv[] )
{
	// name of the file to parse through
	const char* fName = argv[1];

	mp* dispatcher = new mp();
	dispatcher->openFile(fName);

	cout << setw(15) << left << "Token" << setw(30) << "Lexeme" << setw(8) << "Line #" << setw(8) << "Column #" << endl;
	while (dispatcher->hasToken())
	{
		// Keep on separate couts so that getToken() executes first
		cout << setw(15) << left << dispatcher->getToken();
		cout << setw(30) << dispatcher->getLexeme() << setw(8) << dispatcher->getLineNumber() 
			 << setw(8) << dispatcher->getColumnNumber() << endl;
	}

	// press enter to exit
	printf("Done. Press Enter to exit.\n");
	std::cin.get();
	return 0;
}