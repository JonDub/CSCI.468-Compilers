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


// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
bool Parser::SystemGoal()
{
	switch(lookahead)
	{
	case MP_PROGRAM: //SystemGoal --> Program eof, rule #1     
		  {
			  Program();
			  Match(MP_EOF);
			  cout << "The input program parses!";
			  return true;
		  }
	default: //everything else
		{
			Syntax_Error();
			return false;
		}
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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
	case MP_FUNCTION: // VariableDeclarationTail -> e, rule #7
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

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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


// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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
	case MP_BEGIN: //ProcedureAndFunctionDeclarationPart -> e, rule #14
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


// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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


// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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


// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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


// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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


// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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
	case MP_SCOLON: //OptionalFormalParameterList -> e, rule #20
	case MP_COLON: //OptionalFormalParameterList -> e, rule #20
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

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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
	case MP_RPAREN: //FormalParameterSectionTail -> e, rule #22
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

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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


// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::Statement()
{
	switch(lookahead)
	{
	case MP_END: //Statement -> EmptyStatement, rule #32
	case MP_SCOLON: //Statement -> EmptyStatement, rule #32
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

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::EmptyStatement()
{
}


// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ReadStatement()
{
	switch(lookahead)
	{
	case MP_READ:  // Rule# 43 	ReadStatement -> "read" "(" ReadParameter ReadParameterTail ")" 	read
		ReadParameter();
		ReadParameterTail();
		break;
	default:
		Syntax_Error();
		break;
	}
}


// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ReadParameterTail()
{
	switch(lookahead)
	{
	case MP_LPAREN:	// Rule# 45 	ReadParameterTail -> e 	
		break;
	case MP_COMMA:  // Rule# 44 	ReadParameterTail -> "," ReadParameter ReadParameterTail 	,
		Match(MP_COMMA);
		ReadParameter();
		ReadParameterTail();
		break;
	default:
		Syntax_Error();
		break;
	}
}


// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ReadParameter()
{
	switch(lookahead)
	{
	case MP_IDENTIFIER: // Rule# 46 	ReadParameter -> VariableIdentifier 	Identifier
		Match(MP_IDENTIFIER);
		break;
	default:
		Syntax_Error();
		break;
	}
}


// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::WriteStatement()
{
	switch(lookahead)
	{
	case MP_WRITE: // Rule# 47 	WriteStatement -> "write" "(" WriteParameter WriteParameterTail ")" 	write
		Match(MP_WRITE);
		Match(MP_LPAREN);
		WriteParameter();
		WriteParameterTail();
		Match(MP_RPAREN);
		break;
	default:
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::WriteParameterTail()
{
	switch(lookahead)
	{
	case MP_RPAREN: // Rule# 49 	WriteParameterTail -> e
		break;
	case MP_COMMA:  // Rule# 48 	WriteParameterTail -> "," WriteParameter 	,
		Match(MP_COMMA);
		WriteParameter();
		break;
	default:
		Syntax_Error();
		break;
	}
}


// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::WriteParameter()
{
	switch(lookahead)
	{
	case MP_PLUS: // Rule# 50 	WriteParameter -> OrdinalExpression 	+, -
	case MP_MINUS:  // Rule# 50 	WriteParameter -> OrdinalExpression 	+, -
		OrdinalExpression();
		break;
	default:
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::AssignmentStatement()
{
	switch (lookahead)
	{
	case MP_IDENTIFIER: // Rule# 52 	AssignmentStatement -> FunctionIdentifier ":=" Expression 	Identifier
		FunctionIdentifier();
		Match(MP_ASSIGN);
		Expression();
		break;
	default:
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::IfStatement()
{
	switch (lookahead)
	{
	case MP_IF: // Rule# 53 	IfStatement -> "if" BooleanExpression "then" Statement OptionalElsePart
		Match(MP_IF);
		BooleanExpression();
		Match(MP_THEN);
		Statement();
		OptionalElsePart();
		break;
	default:
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::OptionalElsePart()
{
	switch (lookahead)
	{
	case MP_IF: // Rule# 54 	OptionalElsePart -> "else" Statement 	else
		Match(MP_ELSE);
		Statement();
		break;
	default:
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::RepeatStatement()
{
	switch(lookahead)
	{
	case MP_REPEAT: // Rule# // 56 	RepeatStatement -> "repeat" StatementSequence "until" BooleanExpression
		Match(MP_REPEAT);
		StatementSequence();
		Match(MP_UNTIL);
		BooleanExpression();
		break;
	default:
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::WhileStatement()
{
	switch(lookahead)
	{
	case MP_REPEAT: // Rule# 57 	WhileStatement -> "while" BooleanExpression "do" Statement
		Match(MP_WHILE);
		BooleanExpression();
		Match(MP_DO);
		Statement();
		break;
	default:
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ForStatement()
{
	switch(lookahead)
	{
	case MP_REPEAT: // Rule# 58 	ForStatement -> "for" ControlVariable ":=" InitialValue StepValue FinalValue "do" Statement
		Match(MP_FOR);
		ControlVariable();
		Match(MP_ASSIGN);
		InitialValue();
		StepValue();
		FinalValue();
		Match(MP_DO);
		Statement();
		break;
	default:
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ControlVariable()
{
	switch(lookahead)
	{
	case MP_IDENTIFIER: // Rule# 59 	ControlVariable -> VariableIdentifier 	Identifier
		Match(MP_IDENTIFIER);
		break;
	default:
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::InitialValue()
{
	switch(lookahead)
	{
	case MP_MINUS:
	case MP_PLUS: // Rule# 60	InitialValue -> OrdinalExpression 	+, -
		  {
			  OrdinalExpression();
			  break;
		  }
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::StepValue()
{
	switch(lookahead)
	{
	case MP_DOWNTO: // Rule# 62 	StepValue -> "downto" 	  
		Match(MP_DOWNTO);
		break;
	case MP_TO: // Rule# 61 	StepValue -> "to" 	
		Match(MP_TO);
		break;
	default:
		Syntax_Error();
		break;
	}
}


// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::FinalValue()
{
	switch(lookahead)
	{
	case MP_PLUS: // Rule# 63 	FinalValue -> OrdinalExpression 	+, -
	case MP_MINUS: // Rule# 63 	FinalValue -> OrdinalExpression 	+, -
		OrdinalExpression();
		break;
	default:
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ProcedureStatement()
{
	switch (lookahead)
	{
	case MP_IDENTIFIER: // Rule# 64 	ProcedureStatement -> ProcedureIdentifier OptionalActualParameterList
		Match(MP_IDENTIFIER);
		OptionalActualParameterList();
		break;
	default:
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::OptionalActualParameterList()
{
	switch(lookahead)
	{
	case MP_LPAREN: // Rule# 65 	OptionalActualParameterList -> "(" ActualParameter ActualParameterTail ")" 
			{
				Match(MP_LPAREN);
				ActualParameter();
				ActualParameterTail();
				Match(MP_RPAREN);
				break;
			}
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}
// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ActualParameterTail()
{
	switch(lookahead)
	{
	case MP_COMMA: // Rule# 67 	ActualParameterTail -> "," ActualParameter ActualParameterTail 
			{
				Match(MP_COMMA);
				ActualParameter();
				ActualParameterTail();
				break;
			}
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ActualParameter()
{
	switch(lookahead)
	{
	case MP_PLUS:  // Rule# 69 	ActualParameter -> OrdinalExpression 	+,-
	case MP_MINUS: // Rule# 69 	ActualParameter -> OrdinalExpression 	+,-
			{
				OrdinalExpression();
				break;
			}
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::Expression()
{
	switch(lookahead)
	{
	case MP_MINUS:
	case MP_PLUS: // Expression -> SimpleExpression OptionalRelationalPart 	\+\, \-\	Rule# 70
		  {
			  SimpleExpression();
			  OptionalRelationalPart();
			  break;
		  }
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::OptionalRelationalPart()
{
	switch(lookahead)
	{
	case MP_EQUAL:
	case MP_LTHAN:
	case MP_GTHAN:
	case MP_LEQUAL:
	case MP_GEQUAL:
	case MP_NEQUAL: // Rule# 71 	OptionalRelationalPart -> RelationalOperator SimpleExpression  =, <, >, <=, >=, <>
		  {
			  RelationalOperator();
			  SimpleExpression();
			  break;
		  }
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::RelationalOperator()
{
	switch(lookahead)
	{
	case MP_EQUAL:	// Rule# 73 	RelationalOperator -> "="
		Match(MP_EQUAL);
		break;
	case MP_LTHAN: // Rule# 74 	RelationalOperator -> "<"
		Match(MP_LTHAN);
		break;
	case MP_GTHAN: // Rule# 75 	RelationalOperator -> ">"
		Match(MP_GTHAN);
		break;
	case MP_LEQUAL: // Rule# 76 	RelationalOperator -> "<=" 
		Match(MP_LEQUAL);
		break;
	case MP_GEQUAL: // Rule# 77 	RelationalOperator -> ">=" 
		Match(MP_GEQUAL);
		break;
	case MP_NEQUAL: // Rule# 78 	RelationalOperator -> "<>" 
		Match(MP_NEQUAL);
		break;
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::SimpleExpression()
{
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::TermTail()
{
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::OptionalSign()
{
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::AddingOperator()
{
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::MultiplyingOperator()
{
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::Factor()
{
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ProgramIdentifier()
{
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::VariableIdentifier()
{
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ProcedureIdentifier()
{
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::FunctionIdentifier()
{
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::BooleanExpression()
{
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::OrdinalExpression()
{
	switch(lookahead)
	{
	case MP_MINUS:
	case MP_PLUS: // OrdinalExpression -> Expression 	\+\, \-\	Rule# 105
		  {
			  Expression();
			  break;
		  }
	default: //everything else
		{
			Syntax_Error();
			break;
		}
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::IdentifierList()
{
}


// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
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
