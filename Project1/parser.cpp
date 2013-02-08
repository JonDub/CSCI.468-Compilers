// Driver for the parser
#include <iostream>
#include <iomanip>
#include "mp.h"
using namespace std;

// note, use tokens as defined in http://www.cs.montana.edu/ross/classes/spring2008/cs450/pages/resources/tokens.html

//deliberately left empty for now
parser::parser(){}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::SystemGoal()
{
	switch(lookahead)
	{
	case: MP_PROGRAM //SystemGoal --> Program eof, rule #1     
	{
		Program();
		Match("MP_EOF");
		break();
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::Program()
{
	switch(lookahead)
	{
	case: MP_PROGRAM //Program --> ProgramHeading ";" Block ".", rule #2 
	{
		ProgramHeading();
		Match(MP_SCOLON);
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::ProgramHeading()
{
	switch(lookahead)
	{
	case: _
	{
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

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
			Match(MP_VAR);
			variableDeclaration();
			Match(MP_SCOLON);
			VariableDeclarationTail();
			break();
		}
		default: //everything else
		{
			Error();
			break;
		}
}

	// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::VariableDeclarationPart()
{
	switch(lookahead)
	{
	case: _
	{
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::VariableDeclarationTail()
{
	switch(lookahead)
	{
	case: _
	{
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::VariableDeclaration()
{
	switch(lookahead)
	{
	case: _
	{
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::Type()
{
	switch(lookahead)
	{
	case: _
	{
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::ProcedureAndFunctionDeclarationPart()
{
	switch(lookahead)
	{
	case: _
	{
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::ProcedureDeclaration()
{
	switch(lookahead)
	{
	case: _
	{
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::FunctionDeclaration()
{
	switch(lookahead)
	{
	case: _
	{
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::ProcedureHeading()
{
	switch(lookahead)
	{
	case: _
	{
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::FunctionHeading()
{
	switch(lookahead)
	{
	case: _
	{
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::OptionalFormalParameterList()
{
	switch(lookahead)
	{
	case: _
	{
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::FormalParameterSectionTail()
{
	switch(lookahead)
	{
	case: _
	{
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::FormalParameterSection()
{
	switch(lookahead)
	{
	case: _
	{
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::ValueParameterSection()
{
	switch(lookahead)
	{
	case: _
	{
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::VariableParameterSection()
{
	switch(lookahead)
	{
	case: _
	{
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::StatementPart()
{
	switch(lookahead)
	{
	case: _
	{
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::CompoundStatement()
{
	switch(lookahead)
	{
	case: _
	{
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::StatementSequence()
{
	switch(lookahead)
	{
	case: _
	{
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::StatementTail()
{
	switch(lookahead)
	{
	case: _
	{
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
parser::Statement()
{
	switch(lookahead)
	{
	case: _
	{
	}
	default: //everything else
		{
			Error();
			break;
		}
	}
}

 

parser::match(enum token)//probably not correct syntax
{
	//puts a token on the tree
	//gets the next lookahead
}

parser::Error()
{
	//stops everything and gives a meaningful error message 
}
