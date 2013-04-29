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
	// test parser
	string file = "C:\\Users\\dub\\Dropbox\\CSCI 468 Test Programs\\group 7\\b\\control.up";
	Parser * p = new Parser(file);

	//TestScanner(file);
	//TestParser(file);
	assert(p->parse() == true);
	
	//file = "Programs/lab10_program1.mp";
	//p->setInputFile(file);
	//TestScanner(file);
	//assert(p->parse() == true);
	
	//file = "Programs/lab10_program2.mp";
	//p->setInputFile(file);
	//TestScanner(file);
	//assert(p->parse() == true);

	//file = "Programs/lab10_program3.mp";
	//p->setInputFile(file);
	//TestScanner(file); // added MP_WRITELN
	//assert(p->parse() == true);
	//return 0;

	//file = "Programs/program1.up";
	//p->setInputFile(file);
	//TestScanner(file);
	//assert(p->parse() == true);	// this test program has parameters going into the main program call. I don't know if this is legal or not. 

	//file = "Programs/program2.up";
	//p->setInputFile(file);
	//TestScanner(file);
	//assert(p->parse() == true);

	//file = "Programs/program3.up";
	//p->setInputFile(file);
	//TestScanner(file);
	//assert(p->parse() == true);

	//file = "Programs/program4.up";
	//p->setInputFile(file);
	//TestScanner(file);
	//assert(p->parse() == true);

	//file = "Programs/program5.up";
	//p->setInputFile(file);
	//TestScanner(file);
	//assert(p->parse() == true);

	return 0;
}

// Program Driver

int main (int argc, char* argv[])
{
	// USE THIS BRANCH
	Debug(argc, &(*argv));
	//TestParser( argv[1]);
	//TestScanner(argc, &(*argv));
}

int TestParser(string argv) // int TestParser(int argc, char* argv[])
{
	// name of the file to parse through
	std::string fName = argv;

	// create our parser and start getting shit done
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

	// Adjusted to print tokens to text file
	std::ofstream out("tokens.txt");
    std::streambuf *coutbuf = std::cout.rdbuf();	//save old buffer
    std::cout.rdbuf(out.rdbuf());					//redirect std::cout to tokens.txt


	// Adjusted to print in the proper order
	cout << setw(15) << left << "Token" << setw(8) << "Line #" << setw(10) << "Column #" << setw(45) << "Lexeme"<< endl;	 
	while (dispatcher->hasToken())
	{
		 //Keep on separate couts so that getToken() executes first
		cout << setw(15) << left << EnumToString(dispatcher->getToken()); //set the token	
			cout << setw(8) << dispatcher->getLineNumber() << setw(10) << dispatcher->getColumnNumber();

		if (dispatcher->getLexeme().size() > 45){
			string temp;
			temp = dispatcher->getLexeme().substr(0,44);
			cout << temp << endl;

		} else
			cout << dispatcher->getLexeme()<< endl;
	}

	cout << setw(15) << left << EnumToString(dispatcher->getToken()) 
		<< setw(8) << dispatcher->getLineNumber() << setw(10) << dispatcher->getColumnNumber() 
		<< setw(45) << dispatcher->getLexeme() << endl << endl;

	std::cout.rdbuf(coutbuf);						//reset to standard output

	// press enter to exit
	printf("Done. Press Enter to exit.\n");
	cin.get();
	return 0;
}