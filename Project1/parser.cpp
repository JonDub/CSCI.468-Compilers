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
			  Block();
			  Match(MP_PERIOD);
			  break;
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
	case: //ProgramHeading  --> "program" ProgramIdentifier, rule #3 
		{
			ProgramHeading();
			Match(MP_PROGRAM);
			ProgramIdentifier();
			break;
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
	case MP_VAR: // when MP_VAR Block -> "var" VariableDeclarationPart ";" VariableDeclarationTail, rule #4
		{
			Match(MP_VAR);
			VariableDeclarationPart();
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
		case: MP_VAR //VariableDeclarationPart -> "var" VariableDeclaration ";" VariableDeclarationTail, rule #5
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
	}

	// precondition: (not sure what this should be, but is necessary)
	// postcondition: (not sure what this should be, but is necessary)
	parser::VariableDeclarationTail()
	{
		switch(lookahead)
		{
		case: MP_IDENTIFIER // VariableDeclarationTail  --> VariableDeclaration ";" VariableDeclarationTail, rule #6
			  {
				  variableDeclaration();
				  Match(MP_SCOLON);
				  VariableDeclarationTail();
				  break();
			  }
			  // VVV This will be filled in after some more careful analysis of the grammar VVV
		case: ?? // VariableDeclarationTail -> e, rule #7
			  {
				  break;
			  }
			  // ^^^ This will be filled in after some more careful analysis of the grammar ^^^
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
		case: MP_IDENTIFIER // VariableDeclaration -> Identifierlist ":" Type , rule #8
			  {
				  Identifierlist();
				  Match(MP_COLON);
				  Type();
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
	parser::Type()
	{
		switch(lookahead)
		{
		case: MP_INTEGER_LIT //Type -> "Integer", rule #9
			  {
				  Match(MP_INTEGER_LIT);
				  break;
			  }
		case: MP_FLOAT_LIT //Type -> "Float", rule #10
			  {
				  Match(MP_FLOAT_LIT);
				  break;
			  }
			  // I can't find Boolean in the list of tokens even though it's indicated as a token in the grammar
		case: boolean //Type -> "Boolean", rule #11
			  {
				  Match(boolean);
				  break;
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
		case: MP_PROCEDURE //ProcedureAndFunctionDeclarationPart -> ProcedureDeclaration ProcedureAndFunctionDeclarationPart, rule #12 
			  {
				  ProcedureDeclaration();
				  ProcedureAndFunctionDeclarationPart();
				  break;
			  }
		case: MP_FUNCTION //ProcedureAndFunctionDeclarationPart -> FunctionDeclaration ProcedureAndFunctionDeclarationPart, rule #13
			  {
				  FunctionDeclaration();
				  ProcedureAndFunctionDeclarationPart();
				  break;
			  }
		case: ?? //ProcedureAndFunctionDeclarationPart -> e, rule #14
			  {
				  break;
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
		case: MP_PROCEDURE //ProcedureHeading ";" Block ";", rule #15
			  {
				  ProcedureHeading();
				  Match(MP_SCOLON);
				  Block();
				  Match(MP_SCOLON);
				  break;
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
		case: MP_PROCEDURE //FunctionDeclaration -> FunctionHeading ";" Block ";", rule #16
			  {
				  Functionheading();
				  Match(MP_SCOLON);
				  Block();
				  Match(MP_SCOLON);
				  break;
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
		case: MP_PROCEDURE //ProcedureHeading -> "procedure" procedureIdentifier OptionalFormalParameterList, rule #17
			  {
				  Match(MP_PROCEDURE);
				  procedureIdentifier();
				  OptionalFormalParameterList();
				  break;
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
		case: MP_FUNCTION //FunctionHeading -> "function" functionIdentifier OptionalFormalParameterList ":" Type, rule #18
			  {
				  Match(MP_FUNCTION);
				  functionIdentifier();
				  OptionalFormalParameterList();
				  Match(MP_COLON);
				  Type();
				  break;
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
		case: MP_LPAREN //OptionalFormalParameterList -> "(" FormalParameterSection FormalParameterSectionTail ")", rule #19
			  {
				  Match(MP_LPAREN);
				  FormalParameterSection();
				  FormalParameterSectionTail();
				  Match(MP_RPAREN);
				  break;
			  }
		case: ?? //OptionalFormalParameterList -> e, rule #20
			  {
				  break;
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
		case: MP_SCOLON //FormalParameterSectionTail -> ";" FormalParameterSection FormalParameterSectionTail , rule #21
			  {
				  Match(MP_SCOLON);
				  FormalParameterSection();
				  FormalParameterSectionTail();
				  break;
			  }
		case: ?? //FormalParameterSectionTail -> e, rule #22
			  {
				  break;
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
		case: MP_VAR// FormalParameterSection -> VariableParameterSection, rule #24
			  {
				  VariableParameterSection();
				  break;
			  }
		case: MP_IDENTIFIER// FormalParameterSection -> ValueParameterSection, rule #23 
			  {
				  ValueParameterSection();
				  break;
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
		case: MP_IDENTIFIER// ValueParameterSection -> IdentifierList ":" Type, rule #25
			  {
				  IdentifierList();
				  Match(MP_COLON);
				  Type();
				  break;
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
		case: MP_VAR// VariableParameterSection -> "var" IdentifierList ":" Type, rule #26
			  {
				  Match(MP_VAR);
				  IdentifierList();
				  Match(MP_COLON);
				  Type();
				  break;
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
		case: MP_BEGIN //StatementPart -> CompoundStatement, rule #27
			  {
				  CompoundStatement();
				  break;
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
		case: MP_BEGIN //CompoundStatement -> "begin" StatementSequence "end", rule #28
			  {
				  Match(MP_BEGIN);
				  StatementSequence();
				  Match(MP_END);
				  break;
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
		case: MP_BEGIN //StatementSequence -> Statement StatementTail, rule #29
			  {
				  Statement();
				  StatementTail();
				  break;
			  }
		case: MP_READ //StatementSequence -> Statement StatementTail, rule #29
			  {
				  Statement();
				  StatementTail();
				  break;
			  }
		case: MP_WRITE //StatementSequence -> Statement StatementTail, rule #29
			  {
				  Statement();
				  StatementTail();
				  break;
			  }
		case: MP_IF //StatementSequence -> Statement StatementTail, rule #29
			  {
				  Statement();
				  StatementTail();
				  break;
			  }
		case: MP_REPEAT //StatementSequence -> Statement StatementTail, rule #29
			  {
				  Statement();
				  StatementTail();
				  break;
			  }
		case: MP_IDENTIFIER //StatementSequence -> Statement StatementTail, rule #29
			  {
				  Statement();
				  StatementTail();
				  break;
			  }
		case: MP_FOR //StatementSequence -> Statement StatementTail, rule #29
			  {
				  Statement();
				  StatementTail();
				  break;
			  }
		case: MP_WHILE //StatementSequence -> Statement StatementTail, rule #29
			  {
				  Statement();
				  StatementTail();
				  break;
			  }
		case: ??
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
		case: MP_SCOLON//StatementTail -> ";" Statement StatementTail , rule #30
			  {
				  Match(MP_SCOLON);
				  Statement();
				  StatementTail();
				  break;
			  }
		case: ??// StatementTail -> e, rule #31
			  {
				  break;
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
		case: ?? //Statement -> EmptyStatement, rule #32
			  {
				  EmptyStatement();
				  break;
			  }
		case: MP_BEGIN //Statement -> CompoundStatement, rule #33
			  {
				  CompoundStatement();
				  break;
			  }
		case: MP_READ //Statement -> ReadStatement, rule #34
			  {
				  ReadStatement();
				  break;
			  }
		case: MP_WRITE //Statement -> WriteStatement, rule #35
			  {
				  WriteStatement();
				  break;
			  }
		case: MP_ASSIGN //Statement -> AssignmentStatement, rule #36
			  {
				  AssignmentStatement();
				  break;
			  }
		case: MP_IF //Statement -> IfStatement, rule #37
			  {
				  IfStatement();
				  break;
			  }
		case: MP_WHILE //Statement -> WhileStatement, rule #38
			  {
				  WhileStatement();
				  break;
			  }
		case: MP_REPEAT //Statement -> RepeatStatement, rule #39
			  {
				  RepeatStatement();
				  break;
			  }
		case: MP_FOR //Statement -> ForStatement, rule #40
			  {
				  ForStatement();
				  break;
			  }
		case: MP_IDENTIFIER //Statement -> ProcedureStatement, rule #41
			  {
				  ProcedureStatement();
				  break;
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
