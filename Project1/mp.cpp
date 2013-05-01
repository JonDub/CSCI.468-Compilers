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
int TestParser(string);	
int TestScanner(string);

// Used for unit testing the components of the parser
bool Debug(int argc, char* argv[])
{	
	// Test parser
	string file = "";

	// C level
	file = "Programs/testsuite-spring2013/C/C_2writeTest.txt";
	file = "Programs/testsuite-spring2013/C/C_3nasty.txt";
	file = "Programs/testsuite-spring2013/C/C_4floats.txt";

	// B level
	file = "Programs/testsuite-spring2013/B/B_2forTest.txt";
	file = "Programs/testsuite-spring2013/B/B_3repeatTest.txt";
	file = "Programs/testsuite-spring2013/B/B_4simpleWhileTst.txt";
	file = "Programs/testsuite-spring2013/B/B_6combine.txt";
	file = "Programs/testsuite-spring2013/B/B_7harderWhileTest.txt";
	file = "Programs/testsuite-spring2013/B/B_8nastyIf.txt";

	Parser * p = new Parser(file);

	//TestScanner(file);
	//TestParser(file);
	p->parse();

	//file = "Programs/program1.up";
	//p->setInputFile(file);
	//TestScanner(file);
	//assert(p->parse() == true);

	return 0;
}

// Main Entry
int main (int argc, char* argv[])
{
	//Debug(argc, &(*argv));
	TestParser( argv[1]);
	//TestScanner(argc, &(*argv));
}

// Used to test the parser
int TestParser(string argv)
{
	// Name of the file to parse through
	std::string fName = argv;

	// Create our parser
	Parser* parser = new Parser(fName);

	if (parser->parse())
	{
		cout << "The input program parses!" << endl;
	} 

	// Press enter to exit
	cout << endl << "Done. Press Enter to exit." << endl;
	cin.get();
	return 0;
}

// Used to test the scanner
int TestScanner(  string argv ) // int TestScanner(  int argc, char* argv[] )
{
	// Name of the file to parse through
	std::string fName = argv;

	Scanner* dispatcher = new Scanner();
	dispatcher->openFile(fName);

	// Adjusted to print tokens to text file
	std::ofstream out("tokens.txt");
	std::streambuf *coutbuf = std::cout.rdbuf();	//save old buffer
	std::cout.rdbuf(out.rdbuf());					//redirect std::cout to tokens.txt


	// Adjusted to print in the proper order
	cout << setw(15) << left << "Token" << setw(8) << "Line #" << setw(10) << "Column #" 
		<< setw(45) << "Lexeme"<< endl;	 
	while (dispatcher->hasToken())
	{
		// Keep on separate couts so that getToken() executes first
		cout << setw(15) << left << EnumToString(dispatcher->getToken()); //set the token	
		cout << setw(8) << dispatcher->getLineNumber() << setw(10) 
			<< dispatcher->getColumnNumber();

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

	std::cout.rdbuf(coutbuf); // Reset to standard output

	// Press enter to exit
	printf("Done. Press Enter to exit.\n");
	cin.get();
	return 0;
}