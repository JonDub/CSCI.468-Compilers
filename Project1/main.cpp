// Driver for the application
#include <iostream>
#include <iomanip>
#include "Scanner.h"
using namespace std;


int main ( int argc, char* argv[] )
{
	// name of the file to parse through
	const char* fName = argv[1];

	Scanner* dispatcher = new Scanner();
	dispatcher->openFile(fName);

	cout << setw(15) << left << "Token" << setw(45) << "Lexeme" << setw(8) << "Line #" << setw(8) << "Column #" << endl;
	while (dispatcher->hasToken())
	{
		// Keep on separate couts so that getToken() executes first
		cout << setw(15) << left << dispatcher->getToken();
		cout << setw(45);
		
		if (dispatcher->getLexeme().size() > 45){
			string temp;
			temp = dispatcher->getLexeme().substr(0,44);
			cout << temp;
		} else
			cout << dispatcher->getLexeme();

		cout << setw(8) << dispatcher->getLineNumber() << setw(8) << dispatcher->getColumnNumber() << endl;
	}
	cout << setw(15) << left << dispatcher->getToken() << setw(45) << dispatcher->getLexeme() 
		<< setw(8) << dispatcher->getLineNumber() << setw(8) << dispatcher->getColumnNumber() << endl << endl;

	// press enter to exit
	printf("Done. Press Enter to exit.\n");
	std::cin.get();
	return 0;
}