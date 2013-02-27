#include "ParseTree.h"


ParseTree::ParseTree(string filename)
{
	// open the log file 
	outFile.open(filename, ofstream::out);
}


ParseTree::~ParseTree(void)
{
	outFile.close();
}


void ParseTree::LogExpansion(int rule)
{
	rule--;
	if (rule >= 0 && rule < rules.size())
		outFile << rules.at(rule) << '\n';
	else
		outFile << "Unknown rule number: " << rule << '\n';
}


bool ParseTree::ReadCFGRules(string filename)
{
	// open our file of rules and read them in
	ifstream infile;
	infile.open(filename, ifstream::in);

	while (!infile.eof()){
		string line;
		getline(infile, line);
		rules.push_back(line);
	}	

	infile.close();
	return true;
}
