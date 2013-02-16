#include "ParseTree.h"


ParseTree::ParseTree(std::string filename)
{
	// open the log file 
	outFile.open(filename, std::ofstream::out);
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
}


bool ParseTree::ReadCFGRules(std::string filename)
{
	// open our file of rules and read them in
	std::ifstream infile;
	infile.open(filename, std::ifstream::in);

	while (!infile.eof()){
		std::string line;
		std::getline(infile, line);
		rules.push_back(line);
	}	

	infile.close();
	return true;
}
