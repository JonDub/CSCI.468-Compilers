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
		return SystemGoal();
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
		Program();
		Match(MP_EOF);
		return true;
	default: //everything else
		Syntax_Error();
		return false;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::Program()
{
	switch(lookahead)
	{
	case MP_PROGRAM: //Program --> ProgramHeading ";" Block ".", rule #2 
		ProgramHeading();
		Match(MP_SCOLON);
		Block();
		Match(MP_PERIOD);
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ProgramHeading()
{
	switch(lookahead)
	{
	case MP_PROGRAM: //ProgramHeading  --> "program" ProgramIdentifier, rule #3 
		Match(MP_PROGRAM);
		ProgramIdentifier();
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::Block()
{
	switch (lookahead)
	{
	case MP_VAR: // when MP_VAR Block -> "var" VariableDeclarationPart ";" VariableDeclarationTail, rule #4
		Match(MP_VAR);
		VariableDeclarationPart();
		Match(MP_SCOLON);
		VariableDeclarationTail();
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::VariableDeclarationPart()
{
	switch(lookahead)
	{
	case MP_VAR: //VariableDeclarationPart -> "var" VariableDeclaration ";" VariableDeclarationTail, rule #5
		Match(MP_VAR);
		VariableDeclaration();
		Match(MP_SCOLON);
		VariableDeclarationTail();
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::VariableDeclarationTail()
{
	switch(lookahead)
	{
	case MP_IDENTIFIER: // VariableDeclarationTail  --> VariableDeclaration ";" VariableDeclarationTail, rule #6
		VariableDeclaration();
		Match(MP_SCOLON);
		VariableDeclarationTail();
		break;
	case MP_PROCEDURE:
	case MP_BEGIN:
	case MP_FUNCTION: // VariableDeclarationTail -> e, rule #7
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::VariableDeclaration()
{
	switch(lookahead)
	{
	case MP_IDENTIFIER: // VariableDeclaration -> Identifierlist ":" Type , rule #8
		IdentifierList();
		Match(MP_COLON);
		Type();
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::Type()
{
	switch(lookahead)
	{
	case MP_INTEGER_LIT: //Type -> "Integer", rule #9
		Match(MP_INTEGER_LIT);
		break;
	case MP_FLOAT_LIT: //Type -> "Float", rule #10
		Match(MP_FLOAT_LIT);
		break;
	case MP_BOOLEAN: //Type -> "Boolean", rule #11
			  Match(MP_BOOLEAN);
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ProcedureAndFunctionDeclarationPart()
{
	switch(lookahead)
	{
	case MP_PROCEDURE: //ProcedureAndFunctionDeclarationPart -> ProcedureDeclaration ProcedureAndFunctionDeclarationPart, rule #12 
		ProcedureDeclaration();
		ProcedureAndFunctionDeclarationPart();
		break;
	case MP_FUNCTION: //ProcedureAndFunctionDeclarationPart -> FunctionDeclaration ProcedureAndFunctionDeclarationPart, rule #13
		FunctionDeclaration();
		ProcedureAndFunctionDeclarationPart();
		break;
	case MP_BEGIN: //ProcedureAndFunctionDeclarationPart -> e, rule #14
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ProcedureDeclaration()
{
	switch(lookahead)
	{
	case MP_PROCEDURE: //ProcedureHeading ";" Block ";", rule #15
		ProcedureHeading();
		Match(MP_SCOLON);
		Block();
		Match(MP_SCOLON);
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::FunctionDeclaration()
{
	switch(lookahead)
	{
	case MP_FUNCTION: //FunctionDeclaration -> FunctionHeading ";" Block ";", rule #16
		FunctionHeading();
		Match(MP_SCOLON);
		Block();
		Match(MP_SCOLON);
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ProcedureHeading()
{
	switch(lookahead)
	{
	case MP_PROCEDURE: //ProcedureHeading -> "procedure" procedureIdentifier OptionalFormalParameterList, rule #17
		Match(MP_PROCEDURE);
		ProcedureIdentifier();
		OptionalFormalParameterList();
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::FunctionHeading()
{
	switch(lookahead)
	{
	case MP_FUNCTION: //FunctionHeading -> "function" functionIdentifier OptionalFormalParameterList ":" Type rule #18
		Match(MP_FUNCTION);
		FunctionIdentifier();
		OptionalFormalParameterList();
		Match(MP_COLON);
		Type();
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::OptionalFormalParameterList()
{
	switch(lookahead)
	{
	case MP_LPAREN: //OptionalFormalParameterList -> "(" FormalParameterSection FormalParameterSectionTail ")", rule #19
		Match(MP_LPAREN);
		FormalParameterSection();
		FormalParameterSectionTail();
		Match(MP_RPAREN);
		break;
	case MP_SCOLON: //OptionalFormalParameterList -> e, rule #20
	case MP_COLON: //OptionalFormalParameterList -> e, rule #20
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::FormalParameterSectionTail()
{
	switch(lookahead)
	{
	case MP_SCOLON: //FormalParameterSectionTail -> ";" FormalParameterSection FormalParameterSectionTail , rule #21
		Match(MP_SCOLON);
		FormalParameterSection();
		FormalParameterSectionTail();
		break;
	case MP_RPAREN: //FormalParameterSectionTail -> e, rule #22
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::FormalParameterSection()
{
	switch(lookahead)
	{
	case MP_IDENTIFIER:// FormalParameterSection -> ValueParameterSection, rule #23 
		ValueParameterSection();
		break;
	case MP_VAR: // FormalParameterSection -> VariableParameterSection, rule #24
		VariableParameterSection();
		break;

	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ValueParameterSection()
{
	switch(lookahead)
	{
	case MP_IDENTIFIER:// ValueParameterSection -> IdentifierList ":" Type, rule #25
		IdentifierList();
		Match(MP_COLON);
		Type();
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::VariableParameterSection()
{
	switch(lookahead)
	{
	case MP_VAR:// VariableParameterSection -> "var" IdentifierList ":" Type, rule #26
		Match(MP_VAR);
		IdentifierList();
		Match(MP_COLON);
		Type();
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::StatementPart()
{
	switch(lookahead)
	{
	case MP_BEGIN: //StatementPart -> CompoundStatement, rule #27
		CompoundStatement();
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::CompoundStatement()
{
	switch(lookahead)
	{
	case MP_BEGIN: //CompoundStatement -> "begin" StatementSequence "end", rule #28
		Match(MP_BEGIN);
		StatementSequence();
		Match(MP_END);
		break;
	default: //everything else
		Syntax_Error();
		break;
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
		Statement();
		StatementTail();
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::StatementTail()
{
	switch(lookahead)
	{
	case MP_SCOLON://StatementTail -> ";" Statement StatementTail , rule #30
		Match(MP_SCOLON);
		Statement();
		StatementTail();
		break;
	case MP_END://StatementTail -> e , rule #31 
		break;
	default: //everything else
		Syntax_Error();
		break;
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
		EmptyStatement();
		break;
	case MP_BEGIN: //Statement -> CompoundStatement, rule #33
		CompoundStatement();
		break;
	case MP_READ: //Statement -> ReadStatement, rule #34
		ReadStatement();
		break;
	case MP_WRITE: //Statement -> WriteStatement, rule #35
		WriteStatement();
		break;
	case MP_ASSIGN: //Statement -> AssignmentStatement, rule #36
		AssignmentStatement();
		break;
	case MP_IF: //Statement -> IfStatement, rule #37
		IfStatement();
		break;
	case MP_WHILE: //Statement -> WhileStatement, rule #38
		WhileStatement();
		break;
	case MP_REPEAT: //Statement -> RepeatStatement, rule #39
		RepeatStatement();
		break;
	case MP_FOR: //Statement -> ForStatement, rule #40
		ForStatement();
		break;
	case MP_IDENTIFIER: //Statement -> ProcedureStatement, rule #41
		ProcedureStatement();
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::EmptyStatement()
{
	switch(lookahead)
	{
	case MP_SCOLON:
	case MP_END:  // EmptyStatement -> e Rule #42 
		break;
	default:
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ReadStatement()
{
	switch(lookahead)
	{
	case MP_READ:  // Rule# 43 	ReadStatement -> "read" "(" ReadParameter ReadParameterTail ")" 
		Match(MP_READ);
		Match(MP_LPAREN);
		ReadParameter();
		ReadParameterTail();
		Match(MP_RPAREN);
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
	case MP_COMMA:  // Rule# 44 	ReadParameterTail -> "," ReadParameter ReadParameterTail
		Match(MP_COMMA);
		ReadParameter();
		ReadParameterTail();
		break;
	case MP_LPAREN:	// Rule# 45 	ReadParameterTail -> e 	
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
	case MP_IDENTIFIER: // Rule# 46 	ReadParameter -> VariableIdentifier
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
	case MP_WRITE: // Rule# 47 	WriteStatement -> "write" "(" WriteParameter WriteParameterTail ")"
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
	case MP_COMMA:  // Rule# 48 	WriteParameterTail -> "," WriteParameter
		Match(MP_COMMA);
		WriteParameter();
		break;
	case MP_RPAREN: // Rule# 49 	WriteParameterTail -> e
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
	case MP_PLUS:
	case MP_MINUS:  // Rule# 50 	WriteParameter -> OrdinalExpression
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
	case MP_IDENTIFIER: // Rule# 52 	AssignmentStatement -> FunctionIdentifier ":=" Expression
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
	case MP_IF: // Rule# 54 	OptionalElsePart -> "else" Statement
		Match(MP_ELSE);
		Statement();
		break;
		//case MP_ELSE CONFLICT POSSIBLE
	case MP_END:
	case MP_SCOLON: // OptionalElsePart -> e Rule #55
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
	case MP_REPEAT: // Rule# 56 	RepeatStatement -> "repeat" StatementSequence "until" BooleanExpression
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
	case MP_WHILE: // Rule# 57 	WhileStatement -> "while" BooleanExpression "do" Statement
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
	case MP_FOR: // Rule# 58 	"for" ControlVariable ":=" InitialValue StepValue FinalValue "do" Statement
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
	case MP_TO: //sure?
	case MP_DOWNTO: //sure?
	case MP_MINUS:
	case MP_PLUS: // Rule# 60	InitialValue -> OrdinalExpression
		OrdinalExpression();
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::StepValue()
{
	switch(lookahead)
	{
	case MP_TO: // Rule# 61 	StepValue -> "to" 
		Match(MP_TO);
		break;
	case MP_DOWNTO: // Rule# 62 	StepValue -> "downto" 	  
		Match(MP_DOWNTO);
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
	case MP_PLUS:
	case MP_MINUS: // FinalValue -> OrdinalExpression Rule# 63
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
		//had case MP_SCOLON -> rule 30, not sure
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
	case MP_LPAREN: //OptionalActualParameterList -> "(" ActualParameter ActualParameterTail ")" Rule# 65 
		Match(MP_LPAREN);
		ActualParameter();
		ActualParameterTail();
		Match(MP_RPAREN);
		break;
	case MP_END:
	case MP_IDENTIFIER: //OptionalActualParameterList -> e Rule #66
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ActualParameterTail()
{
	switch(lookahead)
	{
	case MP_COMMA: // ActualParameterTail -> "," ActualParameter ActualParameterTail Rule #67
		Match(MP_COMMA);
		ActualParameter();
		ActualParameterTail();
		break;
	case MP_RPAREN: // ActualParameterTail -> e Rule #68
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ActualParameter()
{
	switch(lookahead)
	{
	case MP_PLUS:
	case MP_MINUS: // ActualParameter -> OrdinalExpression 	Rule# 69
		OrdinalExpression();
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::Expression()
{
	switch(lookahead)
	{
	case MP_MINUS:
	case MP_PLUS: // Expression -> SimpleExpression OptionalRelationalPart 	Rule# 70
		SimpleExpression();
		OptionalRelationalPart();
		break;
	default: //everything else
		Syntax_Error();
		break;
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
	case MP_NEQUAL: //OptionalRelationalPart -> RelationalOperator SimpleExpression  Rule #71
		RelationalOperator();
		SimpleExpression();
		break;
	case MP_SCOLON:
	case MP_END:
	case MP_THEN:
	case MP_DO: //OptionalRelationalPart -> e Rule #72
		break;
	default: //everything else
		Syntax_Error();
		break;
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
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::SimpleExpression()
{
	switch(lookahead)
	{
	case MP_MINUS:
	case MP_PLUS: // Term -> Factor FactorTail  	Rule# 79
		Factor();
		FactorTail();
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::TermTail()
{
	switch(lookahead)
	{
	case MP_OR:
	case MP_PLUS:
	case MP_MINUS:// TermTail -> AddingOperator Term TermTail  	Rule# 80
		AddingOperator();
		Term();
		TermTail();
	case MP_END:
	case MP_SCOLON: // TermTail -> {e} 	Rule# 81
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::OptionalSign()
{
	switch(lookahead)
	{
	case MP_PLUS: // OptionalSign -> "+" Rule #82
		Match(MP_PLUS);
		break;
	case MP_MINUS: // OptionalSign -> "-" Rule #83
		Match(MP_MINUS);
		break;
	case MP_IDENTIFIER:
	case MP_NOT:
	case MP_UNSIGNEDINTEGER:
	case MP_RPAREN:
	case MP_LPAREN: // OptionalSign -> {e} Rule #84
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::AddingOperator()
{
	switch(lookahead)
	{
	case MP_PLUS: // AddingOperator -> "+"  	Rule# 85
		Match(MP_PLUS);
		break;
	case MP_MINUS: // AddingOperator -> "-"  	Rule# 86
		Match(MP_MINUS);
		break;
	case MP_OR: // AddingOperator -> "or" 	Rule# 87
		Match(MP_OR);
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::Term()
{
	switch(lookahead)
	{
	case MP_UNSIGNEDINTEGER:
	case MP_NOT:
	case MP_IDENTIFIER: // Term -> Factor FactorTail  	Rule# 88
		Factor();
		FactorTail();
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::FactorTail()
{
	switch(lookahead)
	{
	case MP_DIV:
	case MP_TIMES:
	case MP_MOD:
	case MP_AND: // FactorTail -> MultiplyingOperator Factor FactorTail  	Rule# 89
		MultiplyingOperator();
		Factor();
		FactorTail();
		break;
	case MP_OR:
	case MP_MINUS:
	case MP_PLUS:
	case MP_EQUAL:
	case MP_GEQUAL:
	case MP_LEQUAL:
	case MP_GTHAN:
	case MP_LTHAN:
	case MP_RPAREN:
	case MP_LPAREN:
	case MP_END:
	case MP_NEQUAL: // FactorTail -> {e}	Rule# 90
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::MultiplyingOperator()
{
	switch(lookahead)
	{
	case MP_AND: // MultiplyingOperator -> "and"  	Rule# 94
		Match(MP_AND);
		break;
	case MP_MOD: // MultiplyingOperator -> "mod"  	Rule# 93
		Match(MP_MOD);
		break;
	case MP_DIV: // MultiplyingOperator -> "div"  	Rule# 92
		Match(MP_DIV);
		break;
	case MP_TIMES: // MultiplyingOperator -> "*"  	Rule# 91
		Match(MP_TIMES);
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::Factor()
{
	switch(lookahead)
	{
	case MP_UNSIGNEDINTEGER: // Factor -> UnsignedInteger  	Rule# 95
		Match(MP_UNSIGNEDINTEGER);
		break;
		//////////////////////// Conflict 96, 99
	case MP_NOT: // "not" Factor  	Rule# 97
		Match(MP_NOT);
		Factor();
		break;
	case MP_LPAREN: // Factor -> "(" Expression ")"  	Rule# 98
		Match(MP_LPAREN);
		Expression();
		Match(MP_RPAREN);
		break;
		//////////////////////// Conflict 96, 99
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ProgramIdentifier()
{
	switch(lookahead)
	{
	case MP_IDENTIFIER: // ProgramIdentifier -> Identifier  	Rule# 100M
		Match(MP_IDENTIFIER);
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::VariableIdentifier()
{
	switch(lookahead)
	{
	case MP_IDENTIFIER: // VariableIdentifier -> Identifier  	Rule# 101
		Match(MP_IDENTIFIER);
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ProcedureIdentifier()
{
	switch(lookahead)
	{
	case MP_IDENTIFIER: // ProcedureIdentifier -> Identifier 	Rule# 102
		Match(MP_IDENTIFIER);
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::FunctionIdentifier()
{
	switch(lookahead)
	{
	case MP_IDENTIFIER: // FunctionIdentifier -> Identifier 	Rule# 103
		Match(MP_IDENTIFIER);
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::BooleanExpression()
{
	switch(lookahead)
	{
	case MP_LPAREN:
	case MP_RPAREN:
	case MP_PLUS:
	case MP_MINUS:
	case MP_UNSIGNEDINTEGER:
	case MP_NOT:
	case MP_IDENTIFIER: // BooleanExpression -> Expression 	Rule# 104
		Expression();
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::OrdinalExpression()
{
	switch(lookahead)
	{
	case MP_MINUS:
	case MP_IDENTIFIER:
	case MP_NOT:
	case MP_UNSIGNEDINTEGER:
	case MP_RPAREN:
	case MP_LPAREN:
	case MP_PLUS: // OrdinalExpression -> Expression	Rule# 105
		Expression();
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::IdentifierList()
{
	switch(lookahead)
	{
	case MP_IDENTIFIER: // IdentifierList -> Identifier IdentifierTail Rule# 106
		Match(MP_IDENTIFIER);
		IdentifierTail();
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::IdentifierTail()
{
	switch(lookahead)
	{
	case MP_COMMA: // IdentifierTail -> "," Identifier IdentifierTail  		Rule# 107
		Match(MP_COMMA);
		Match(MP_IDENTIFIER);
		IdentifierTail();
		break;
	case MP_COLON: // IdentifierTail -> e  		Rule# 108
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
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
		Syntax_Error(token);
}

void Parser::Syntax_Error(Token expected)
{
	//stops everything and gives a meaningful error message 
	cout << "Syntax error found on line " << scanner->getLineNumber() << ", column "
		<< scanner->getColumnNumber();

	if (expected != MP_NULL)
		cout << " Expected " << EnumToString(expected) << " but found " << EnumToString(lookahead);
	cout << endl;
	throw -1;
}
