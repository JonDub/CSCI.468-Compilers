// Driver for the application
#include <iostream>
#include <iomanip>
#include "Scanner.h"
#include "Parser.h"
#include "Tokens.h"
#include "SymbolTable.h"
#include "Record.h"

using namespace std;

/*
*	FUNCTION PROTOTYPES
*/
void Debug()
{
	SymbolTable* table = new SymbolTable();
	table->CreateTable();
	table->CreateTable();

	Record* rec = new Record();

	table->InsertRecord(MP_ASSIGN, "test", 2, 16);
	table->InsertRecord(MP_BEGIN, "test2", 22, 64);
	table->InsertRecord(MP_COMMENT, "test3", 21, 654);
	table->InsertRecord(MP_DO, "test4", 223, 698);
	table->InsertRecord(MP_GTHAN, "test5", 25, 648);
	table->InsertRecord(MP_ASSIGN, "test6", 26, 1586);
	table->InsertRecord(MP_ASSIGN, "test7", 29, 156);
	table->InsertRecord(MP_ASSIGN, "test8", 20, 625);


	table->LookupRecord(MP_ASSIGN, "test2");
	int j = 0;
}


int TestParser(int, char*);
int TestScanner(int, char*);

// Program Driver
int main ( int argc, char* argv[] )
{
	Debug();
}


int TestParser(int argc, char* argv[])
{
	// name of the file to parse through
	const char* fName = argv[1];

	// create our parser and start getting shit done
	Parser* parser = new Parser(fName);

	if (parser->Parse())
	{
		cout << "The input program parses!" << endl;
	} 
	
	// press enter to exit
	cout << endl << "Done. Press Enter to exit." << endl;
	cin.get();
	return 0;
}
// was used to test the scanner
int TestScanner(  int argc, char* argv[] )
{
	// name of the file to parse through
	const char* fName = argv[1];

	Scanner* dispatcher = new Scanner();
	dispatcher->openFile(fName);

	cout << setw(15) << left << "Token" << setw(45) << "Lexeme" << setw(8) << "Line #" << setw(8) << "Column #" << endl;
	while (dispatcher->hasToken())
	{
		// Keep on separate couts so that getToken() executes first
		cout << setw(15) << left << EnumToString(dispatcher->getToken());
		cout << setw(45);
		
		if (dispatcher->getLexeme().size() > 45){
			string temp;
			temp = dispatcher->getLexeme().substr(0,44);
			cout << temp;
		} else
			cout << dispatcher->getLexeme();

		cout << setw(8) << dispatcher->getLineNumber() << setw(8) << dispatcher->getColumnNumber() << endl;
	}
	cout << setw(15) << left << EnumToString(dispatcher->getToken()) << setw(45) << dispatcher->getLexeme() 
		<< setw(8) << dispatcher->getLineNumber() << setw(8) << dispatcher->getColumnNumber() << endl << endl;

	// press enter to exit
	printf("Done. Press Enter to exit.\n");
	cin.get();
	return 0;
}