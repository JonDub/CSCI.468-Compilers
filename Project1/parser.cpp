#include "Parser.h"


Parser::Parser(void)
{
}

Parser::Parser(const char* fName)
{
	scanner = new Scanner();
	scanner->openFile(fName);
}

Parser::~Parser(void)
{
	delete scanner;
}

bool Parser::Parse()
{
	try
	{
		// start the parser. set the first lookahead
	lookahead = scanner->getToken();
	
	if (SystemGoal())
		return true;
	else 
		ParseError:
		return false;
	} 
	catch (int ex)
	{
		return false;
	}
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
bool Parser::SystemGoal()
{
	switch(lookahead)
	{
	case MP_PROGRAM: //SystemGoal --> Program eof, rule #1     
		  {
			  Program();
			  Match(MP_EOF);
			  return true;
		  }
	default: //everything else
		{
			Syntax_Error();
			return false;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::Program()
{
	switch(lookahead)
	{
	case MP_PROGRAM: //Program --> ProgramHeading ";" Block ".", rule #2 
		  {
			  ProgramHeading();
			  Match(MP_SCOLON);
			  Block();
			  Match(MP_PERIOD);
			  break;
		  }
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::ProgramHeading()
{
	switch(lookahead)
	{
	case MP_PROGRAM: //ProgramHeading  --> "program" ProgramIdentifier, rule #3 
		{
			Match(MP_PROGRAM);
			ProgramIdentifier();
			break;
		}
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}

// example for Block. 
// here "lookahead" is the lookahead token. We should enumerate the tokens so that they can be used in switch statements
// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::Block()
{
	switch (lookahead)
	{
	case MP_VAR: // when MP_VAR Block -> "var" VariableDeclarationPart ";" VariableDeclarationTail, rule #4
		{
			Match(MP_VAR);
			VariableDeclarationPart();
			Match(MP_SCOLON);
			VariableDeclarationTail();
			break;
		}
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::VariableDeclarationPart()
{
	switch(lookahead)
	{
	case MP_VAR: //VariableDeclarationPart -> "var" VariableDeclaration ";" VariableDeclarationTail, rule #5
		  {
			  Match(MP_VAR);
			  VariableDeclaration();
			  Match(MP_SCOLON);
			  VariableDeclarationTail();
			  break;
		  }
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::VariableDeclarationTail()
{
	switch(lookahead)
	{
	case MP_IDENTIFIER: // VariableDeclarationTail  --> VariableDeclaration ";" VariableDeclarationTail, rule #6
		  {
			  VariableDeclaration();
			  Match(MP_SCOLON);
			  VariableDeclarationTail();
			  break;
		  }
	case: MP_FUNCTION // VariableDeclarationTail -> e, rule #7
		  {
			  break;
		  }
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::VariableDeclaration()
{
	switch(lookahead)
	{
	case MP_IDENTIFIER: // VariableDeclaration -> Identifierlist ":" Type , rule #8
		  {
			  IdentifierList();
			  Match(MP_COLON);
			  Type();
			  break;
		  }
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::Type()
{
	switch(lookahead)
	{
	case MP_INTEGER_LIT: //Type -> "Integer", rule #9
		  {
			  Match(MP_INTEGER_LIT);
			  break;
		  }
	case MP_FLOAT_LIT: //Type -> "Float", rule #10
		  {
			  Match(MP_FLOAT_LIT);
			  break;
		  }
		  // I can't find Boolean in the list of tokens even though it's indicated as a token in the grammar
		  // TODO: Define boolean if in the grammar or not
	//case: BOOLEAN //Type -> "Boolean", rule #11
	//	  {
	//		  Match(boolean);
	//		  break;
	//	  }
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::ProcedureAndFunctionDeclarationPart()
{
	switch(lookahead)
	{
	case MP_PROCEDURE: //ProcedureAndFunctionDeclarationPart -> ProcedureDeclaration ProcedureAndFunctionDeclarationPart, rule #12 
		  {
			  ProcedureDeclaration();
			  ProcedureAndFunctionDeclarationPart();
			  break;
		  }
	case MP_FUNCTION: //ProcedureAndFunctionDeclarationPart -> FunctionDeclaration ProcedureAndFunctionDeclarationPart, rule #13
		  {
			  FunctionDeclaration();
			  ProcedureAndFunctionDeclarationPart();
			  break;
		  }
	case: MP_BEGIN //ProcedureAndFunctionDeclarationPart -> e, rule #14
		  {
			  break;
		  }
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::ProcedureDeclaration()
{
	switch(lookahead)
	{
	case MP_PROCEDURE: //ProcedureHeading ";" Block ";", rule #15
			{
				ProcedureHeading();
				Match(MP_SCOLON);
				Block();
				Match(MP_SCOLON);
				break;
			}
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::FunctionDeclaration()
{
	switch(lookahead)
	{
	case MP_PROCEDURE: //FunctionDeclaration -> FunctionHeading ";" Block ";", rule #16
		  {
			  FunctionHeading();
			  Match(MP_SCOLON);
			  Block();
			  Match(MP_SCOLON);
			  break;
		  }
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::ProcedureHeading()
{
	switch(lookahead)
	{
	case MP_PROCEDURE: //ProcedureHeading -> "procedure" procedureIdentifier OptionalFormalParameterList, rule #17
		  {
			  Match(MP_PROCEDURE);
			  ProcedureIdentifier();
			  OptionalFormalParameterList();
			  break;
		  }
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::FunctionHeading()
{
	switch(lookahead)
	{
	case MP_FUNCTION: //FunctionHeading -> "function" functionIdentifier OptionalFormalParameterList ":" Type, rule #18
		  {
			  Match(MP_FUNCTION);
			  FunctionIdentifier();
			  OptionalFormalParameterList();
			  Match(MP_COLON);
			  Type();
			  break;
		  }
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::OptionalFormalParameterList()
{
	switch(lookahead)
	{
	case MP_LPAREN: //OptionalFormalParameterList -> "(" FormalParameterSection FormalParameterSectionTail ")", rule #19
			{
				Match(MP_LPAREN);
				FormalParameterSection();
				FormalParameterSectionTail();
				Match(MP_RPAREN);
				break;
			}
	case: MP_SCOLON //OptionalFormalParameterList -> e, rule #20
	case: MP_COLON //OptionalFormalParameterList -> e, rule #20
			{
				break;
			}
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::FormalParameterSectionTail()
{
	switch(lookahead)
	{
	case MP_SCOLON: //FormalParameterSectionTail -> ";" FormalParameterSection FormalParameterSectionTail , rule #21
			{
				Match(MP_SCOLON);
				FormalParameterSection();
				FormalParameterSectionTail();
				break;
			}
	case: MP_RPAREN //FormalParameterSectionTail -> e, rule #22
			{
				break;
			}
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::FormalParameterSection()
{
	switch(lookahead)
	{
	case MP_VAR: // FormalParameterSection -> VariableParameterSection, rule #24
		  {
			  VariableParameterSection();
			  break;
		  }
	case MP_IDENTIFIER:// FormalParameterSection -> ValueParameterSection, rule #23 
		  {
			  ValueParameterSection();
			  break;
		  }
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::ValueParameterSection()
{
	switch(lookahead)
	{
	case MP_IDENTIFIER:// ValueParameterSection -> IdentifierList ":" Type, rule #25
		  {
			  IdentifierList();
			  Match(MP_COLON);
			  Type();
			  break;
		  }
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::VariableParameterSection()
{
	switch(lookahead)
	{
	case MP_VAR:// VariableParameterSection -> "var" IdentifierList ":" Type, rule #26
		  {
			  Match(MP_VAR);
			  IdentifierList();
			  Match(MP_COLON);
			  Type();
			  break;
		  }
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}

// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::StatementPart()
{
	switch(lookahead)
	{
	case MP_BEGIN: //StatementPart -> CompoundStatement, rule #27
		  {
			  CompoundStatement();
			  break;
		  }
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::CompoundStatement()
{
	switch(lookahead)
	{
	case MP_BEGIN: //CompoundStatement -> "begin" StatementSequence "end", rule #28
		  {
			  Match(MP_BEGIN);
			  StatementSequence();
			  Match(MP_END);
			  break;
		  }
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::StatementSequence()
{
	switch(lookahead)
	{
	case MP_SCOLON: //StatementSequence -> Statement StatementTail, rule #29 {e}
	case MP_END:  //StatementSequence -> Statement StatementTail, rule #29 {e}
	case MP_BEGIN: //StatementSequence -> Statement StatementTail, rule #29
	case MP_READ: //StatementSequence -> Statement StatementTail, rule #29
	case MP_WRITE: //StatementSequence -> Statement StatementTail, rule #29
	case MP_IF: //StatementSequence -> Statement StatementTail, rule #29
	case MP_REPEAT: //StatementSequence -> Statement StatementTail, rule #29
	case MP_IDENTIFIER: //StatementSequence -> Statement StatementTail, rule #29
	case MP_FOR: //StatementSequence -> Statement StatementTail, rule #29
	case MP_WHILE: //StatementSequence -> Statement StatementTail, rule #29
		  {
			  Statement();
			  StatementTail();
			  break;
		  }
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::StatementTail()
{
	switch(lookahead)
	{
	
	case MP_SCOLON://StatementTail -> ";" Statement StatementTail , rule #30
		  {
			  Match(MP_SCOLON);
			  Statement();
			  StatementTail();
			  break;
		  }
	case MP_END://StatementTail -> e , rule #31 
		{
			break;
		}
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::Statement()
{
	switch(lookahead)
	{
	case: MP_END //Statement -> EmptyStatement, rule #32
	case: MP_SCOLON //Statement -> EmptyStatement, rule #32
		  {
			  EmptyStatement();
			  break;
		  }
	case MP_BEGIN: //Statement -> CompoundStatement, rule #33
		  {
			  CompoundStatement();
			  break;
		  }
	case MP_READ: //Statement -> ReadStatement, rule #34
		  {
			  ReadStatement();
			  break;
		  }
	case MP_WRITE: //Statement -> WriteStatement, rule #35
		  {
			  WriteStatement();
			  break;
		  }
	case MP_ASSIGN: //Statement -> AssignmentStatement, rule #36
		  {
			  AssignmentStatement();
			  break;
		  }
	case MP_IF: //Statement -> IfStatement, rule #37
		  {
			  IfStatement();
			  break;
		  }
	case MP_WHILE: //Statement -> WhileStatement, rule #38
		  {
			  WhileStatement();
			  break;
		  }
	case MP_REPEAT: //Statement -> RepeatStatement, rule #39
		  {
			  RepeatStatement();
			  break;
		  }
	case MP_FOR: //Statement -> ForStatement, rule #40
		  {
			  ForStatement();
			  break;
		  }
	case MP_IDENTIFIER: //Statement -> ProcedureStatement, rule #41
		  {
			  ProcedureStatement();
			  break;
		  }
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::AssignmentStatement()
{
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::ForStatement()
{
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::FunctionIdentifier()
{
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::IdentifierList()
{
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::IfStatement()
{
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::ProcedureIdentifier()
{
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::ProcedureStatement()
{
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::ProgramIdentifier()
{
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::ReadStatement()
{
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::WriteStatement()
{
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::RepeatStatement()
{
}


// precondition: (not sure what this should be, but is necessary)
// postcondition: (not sure what this should be, but is necessary)
void Parser::WhileStatement()
{
}

void Parser::Match(Token token)
{
	// puts a token on the tree
	// gets the next lookahead
	if (token == lookahead)
		lookahead = scanner->getToken();
	else
		Syntax_Error();
}


void Parser::Syntax_Error()
{
	//stops everything and gives a meaningful error message 
	cout << "Syntax error found on line " << scanner->getLineNumber() << ", column "
		<< scanner->getColumnNumber() << endl;
	throw -1;
}
