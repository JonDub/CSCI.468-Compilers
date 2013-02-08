// Driver for the parser
#include <iostream>
#include <iomanip>
#include "mp.h"
using namespace std;

//deliberately left empty for now
parser::parser(){}


// example for Block. 
// here "lookahead" is the lookahead token. We should enumerate the tokens so that they can be used in switch statements
// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::block()
{
	switch (lookahead)
	{
		case MP_VAR: // when MP_VAR Block -> "var" VariableDeclaration ";" VariableDeclarationTail, rule #4
		{
			Match("var");
			variableDeclaration();
			Match(";"); //maybe char not string?
			VariableDeclarationTail();
			break();
		}
		default: //everything else
		{
			Error();
			break;
		}
}

parser::match(string token)
{
	//puts a token on the tree
	//gets the next lookahead
}

parser::Error()
{
	//stops everything and gives a meaningful error message 
}