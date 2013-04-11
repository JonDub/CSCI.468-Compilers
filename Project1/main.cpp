// Driver for the application
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <string>
#include "Scanner.h"
#include "Parser.h"
#include "Tokens.h"
#include "SymbolTable.h"
#include "Record.h"

using namespace std;

/*
*	FUNCTION PROTOTYPES
*/
int TestParser(string);	// int TestParser(int, char*[]);
int TestScanner(string);  // int TestScanner(int, char*[]);

bool Debug(int argc, char* argv[])
{
	// How to use the symbol table
	/*bool in;
	Record* r;	
	SymbolTable* table = new SymbolTable();
	table->CreateTable();
	in = table->InsertRecord(MP_ASSIGN, "test2", 2, 16);
	assert(in == true);
	r = table->LookupRecord("test7");*/


	// test parser
	string file = "Programs/test2.mp";
	Parser * p = new Parser(file);

	//TestScanner(file);
	assert(p->Parse() == true);

	file = "Programs/lab10_program1.mp";
	p->SetInputFile(file);
	//TestScanner(file);
	assert(p->Parse() == true);

	file = "Programs/lab10_program2.mp";
	p->SetInputFile(file);
	//TestScanner(file);
	assert(p->Parse() == true);

	file = "Programs/lab10_program3.mp";
	p->SetInputFile(file);
	//TestScanner(file); // added MP_WRITELN
	assert(p->Parse() == true);

	file = "Programs/program1.up";
	p->SetInputFile(file);
	//TestScanner(file);
	//assert(p->Parse() == true);	// this test program has parameters going into the main program call. I don't know if this is legal or not. 

	file = "Programs/program2.up";
	p->SetInputFile(file);
	//TestScanner(file);
	assert(p->Parse() == true);

	file = "Programs/program3.up";
	p->SetInputFile(file);
	//TestScanner(file);
	assert(p->Parse() == true);

	file = "Programs/program4.up";
	p->SetInputFile(file);
	//TestScanner(file);
	//assert(p->Parse() == true);

	file = "Programs/program5.up";
	p->SetInputFile(file);
	//TestScanner(file);
	//assert(p->Parse() == true);

	return 0;
}

// Program Driver
int main ( int argc, char* argv[] )
{
	// USE THIS BRANCH
	Debug(argc, &(*argv));
	//TestParser(argc, &(*argv));
	//TestScanner(argc, &(*argv));
}

int TestParser(string argv) // int TestParser(int argc, char* argv[])
{
	// name of the file to parse through
	std::string fName = argv;

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
int TestScanner(  string argv ) // int TestScanner(  int argc, char* argv[] )
{
	// name of the file to parse through
	std::string fName = argv;

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