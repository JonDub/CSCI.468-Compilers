// Driver for the application
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <string>
#include "Scanner.h"
#include "Parser.h"
#include "Tokens.h"
#include "SymbolTable.h"

using namespace std;

/*
*	FUNCTION PROTOTYPES
*/
int TestParser(string);	// int TestParser(int, char*[]);
int TestScanner(string);  // int TestScanner(int, char*[]);

bool Debug(int argc, char* argv[])
{
	// How to use the symbol table
	bool in,test;
	SymbolTable::Record* r;	
	SymbolTable* t = new SymbolTable();
	t->createTable();

	in = t->insertRecord("A",SymbolTable::KIND_VARIABLE,Token::MP_INTEGER_LIT);
	in = t->insertRecord("b",SymbolTable::KIND_VARIABLE,Token::MP_INTEGER_LIT);
	in = t->insertRecord("Cc",SymbolTable::KIND_VARIABLE,Token::MP_INTEGER_LIT);
	in = t->insertRecord("boo",SymbolTable::KIND_VARIABLE,Token::MP_INTEGER_LIT);
	
	
	test=t->contains("A", SymbolTable::KIND_VARIABLE);



	SemanticAnalyzer* sa = new SemanticAnalyzer(t);
	SemanticRecord* sr = new SemanticRecord();

	sr->add("A");
	sr->add("1");

	//return 0;
	
	// test parser
	string file = "Programs/test2.mp";
	Parser * p = new Parser(file);

	//TestScanner(file);
	//assert(p->Parse() == true);

	file = "Programs/lab10_program1.mp";
	p->setInputFile(file);
	//TestScanner(file);
	//assert(p->parse() == true);
	

	file = "Programs/lab10_program2.mp";
	p->setInputFile(file);
	//TestScanner(file);
	//assert(p->parse() == true);

	file = "Programs/lab10_program3.mp";
	p->setInputFile(file);
	//TestScanner(file); // added MP_WRITELN
	assert(p->parse() == true);
	return 0;

	file = "Programs/program1.up";
	p->setInputFile(file);
	//TestScanner(file);
	//assert(p->parse() == true);	// this test program has parameters going into the main program call. I don't know if this is legal or not. 

	file = "Programs/program2.up";
	p->setInputFile(file);
	//TestScanner(file);
	//assert(p->parse() == true);

	file = "Programs/program3.up";
	p->setInputFile(file);
	//TestScanner(file);
	assert(p->parse() == true);

	file = "Programs/program4.up";
	p->setInputFile(file);
	//TestScanner(file);
	assert(p->parse() == true);

	file = "Programs/program5.up";
	p->setInputFile(file);
	//TestScanner(file);
	//assert(p->parse() == true);

	return 0;
}

// Program Driver

int main (int argc, char* argv[])
{
	// USE THIS BRANCH
	//Debug(argc, &(*argv));
	TestParser( argv[1]);
	//TestScanner(argc, &(*argv));
}

int TestParser(string argv) // int TestParser(int argc, char* argv[])
{
	// name of the file to parse through
	std::string fName = argv;

	// create our parser and start getting things done
	Parser* parser = new Parser(fName);

	if (parser->parse())
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
		
		if (dispatcher->lexeme().size() > 45){
			string temp;
			temp = dispatcher->lexeme().substr(0,44);
			cout << temp;
		} else
			cout << dispatcher->lexeme();

		cout << setw(8) << dispatcher->line() << setw(8) << dispatcher->column() << endl;
	}
	cout << setw(15) << left << EnumToString(dispatcher->getToken()) << setw(45) << dispatcher->lexeme() 
		<< setw(8) << dispatcher->line() << setw(8) << dispatcher->column() << endl << endl;

	// press enter to exit
	printf("Done. Press Enter to exit.\n");
	cin.get();
	return 0;
}