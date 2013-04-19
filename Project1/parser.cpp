#include "Parser.h"

Parser::Parser(void)
{
}

Parser::Parser(string fName)
{
	// create our scanner and parse trees
	symbolTable = new SymbolTable();
	analyzer = new SemanticAnalyzer(symbolTable);
	scanner = new Scanner();
	parseTree = new ParseTree("parse_tree.txt");
	parseTree->ReadCFGRules("CFG_rules.txt");
	setInputFile(fName);	
}

void Parser::setInputFile(string fName)
{
	fileName = fName;
	if (scanner != NULL)
		delete scanner;

	scanner = new Scanner();
	parseTree->LogMessage("\nOpening file: " + fileName + "\n");

	if (!scanner->openFile(fileName))
		parseTree->LogMessage("Unable to open file.\n");
}

Parser::~Parser(void)
{
	delete scanner;
	delete analyzer;
	delete parseTree;
	delete symbolTable;
}

bool Parser::parse()
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
		parseTree->LogExpansion(1);
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
	symbolTable->createTable(); // table for main

	switch(lookahead)
	{
	case MP_PROGRAM: //Program --> ProgramHeading ";" Block ".", rule #2 
		parseTree->LogExpansion(2);
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
		parseTree->LogExpansion(3);
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
	case MP_VAR: // Rule# 4		Block -> VariableDeclarationPart 
		parseTree->LogExpansion(4);
		VariableDeclarationPart();
		ProcedureAndFunctionDeclarationPart();
		StatementPart();
		break;
	case MP_PROCEDURE: /// this is not strictly with the grammer but a procedure should not have to declare local variables //DEBUG - was commented out
	case MP_FUNCTION:
	case MP_BEGIN:
		ProcedureAndFunctionDeclarationPart();
		StatementPart();
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
		parseTree->LogExpansion(5);
		Match(MP_VAR);
		VariableDeclaration();
		Match(MP_SCOLON);
		VariableDeclarationTail();
		break;	
		//case :       // VariableDeclarationPart -> e		Rule# 108		// DEBUG - this rule is not in parser
		// break;
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
		parseTree->LogExpansion(6);
		VariableDeclaration();
		Match(MP_SCOLON);
		VariableDeclarationTail();
		break;
	case MP_PROCEDURE:
	case MP_BEGIN:
	case MP_FUNCTION: // VariableDeclarationTail -> e, rule #7
		parseTree->LogExpansion(7);
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
	int start = symbolTable->tableSize();
	Token type = MP_NULL;

	switch(lookahead)
	{
	case MP_IDENTIFIER: // VariableDeclaration -> Identifierlist ":" Type , rule #8
		parseTree->LogExpansion(8);
		IdentifierList();
		Match(MP_COLON);
		type = Type();
		break;
	default: //everything else
		Syntax_Error();
		break;
	}

	int end = symbolTable->tableSize();
	for (int i = start; i < end; i++)
	{
		SymbolTable::Record* rec = symbolTable->lookupRecord(i);
		if (rec != NULL)
			rec->token = type;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ProcedureAndFunctionDeclarationPart()
{
	switch(lookahead)
	{
	case MP_PROCEDURE: //ProcedureAndFunctionDeclarationPart -> ProcedureDeclaration ProcedureAndFunctionDeclarationPart, rule #9 
		parseTree->LogExpansion(9);
		ProcedureDeclaration();
		ProcedureAndFunctionDeclarationPart();
		break;
	case MP_FUNCTION: //ProcedureAndFunctionDeclarationPart -> FunctionDeclaration ProcedureAndFunctionDeclarationPart, rule #10
		parseTree->LogExpansion(10);
		FunctionDeclaration();
		ProcedureAndFunctionDeclarationPart();
		break;
	case MP_BEGIN: //ProcedureAndFunctionDeclarationPart -> e, rule #11
		parseTree->LogExpansion(11);
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
	case MP_PROCEDURE: //ProcedureHeading ";" Block ";", rule #12
		parseTree->LogExpansion(12);
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
	case MP_FUNCTION: //FunctionDeclaration -> FunctionHeading ";" Block ";", rule #13
		parseTree->LogExpansion(13);
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
	case MP_PROCEDURE: //ProcedureHeading -> "procedure" procedureIdentifier OptionalFormalParameterList, rule #16
		parseTree->LogExpansion(16);
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
	Token type;
	int offset;

	switch(lookahead)
	{
	case MP_FUNCTION: //FunctionHeading -> "function" functionIdentifier OptionalFormalParameterList ":" Type rule #15		// DEBUG - current grammer has changed?
		parseTree->LogExpansion(15);
		Match(MP_FUNCTION);

		// get offset where the last function was inserted
		offset = symbolTable->tableSize(0);
		FunctionIdentifier();

		OptionalFormalParameterList();
		//Match(MP_COLON);	//DEBUG
		Match(MP_RETURN);
		type = Type();
		break;
	default: //everything else
		Syntax_Error();
		break;
	}

	// update the last functin that was put into the table with its data return type
	SymbolTable::Record* r = symbolTable->lookupRecord(offset, 0);
	if (r != NULL)
	{
		r->token = type;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::OptionalFormalParameterList()
{
	switch(lookahead)
	{
	case MP_LPAREN: //OptionalFormalParameterList -> "(" FormalParameterSection FormalParameterSectionTail ")", rule #16
		parseTree->LogExpansion(16);
		Match(MP_LPAREN);
		FormalParameterSection();
		FormalParameterSectionTail();
		Match(MP_RPAREN);
		break;
	case MP_SCOLON: //OptionalFormalParameterList -> e, rule #17
	case MP_COLON: //OptionalFormalParameterList -> e, rule #17
		parseTree->LogExpansion(17);
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
	case MP_SCOLON: //FormalParameterSectionTail -> ";" FormalParameterSection FormalParameterSectionTail , rule #18
		parseTree->LogExpansion(18);
		Match(MP_SCOLON);
		FormalParameterSection();
		FormalParameterSectionTail();
		break;
	case MP_RPAREN: //FormalParameterSectionTail -> e, rule #19
		parseTree->LogExpansion(19);
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
	case MP_IDENTIFIER:// FormalParameterSection -> ValueParameterSection, rule #20 
		parseTree->LogExpansion(20);
		ValueParameterSection();
		break;
	case MP_VAR: // FormalParameterSection -> VariableParameterSection, rule #21
		parseTree->LogExpansion(21);
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
	Token type;
	int start = symbolTable->tableSize();

	switch(lookahead)
	{
	case MP_IDENTIFIER:// ValueParameterSection -> IdentifierList ":" Type, rule #22
		parseTree->LogExpansion(22);
		IdentifierList();
		Match(MP_COLON);
		//OptionalPassMode();
		type = Type();
		break;
	default: //everything else
		Syntax_Error();
		break;
	}

	int end = symbolTable->tableSize();
	for (int i = start; i < end; i++)
	{
		SymbolTable::Record* r = symbolTable->lookupRecord(i);
		if (r != NULL)
		{
			r->token = type;
		}
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::VariableParameterSection()
{
	Token type; 

	switch(lookahead)
	{
	case MP_VAR:// VariableParameterSection -> "var" IdentifierList ":" Type, rule #23
		parseTree->LogExpansion(23);
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
	case MP_BEGIN: //StatementPart -> CompoundStatement, rule #24
		parseTree->LogExpansion(24);
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
	case MP_BEGIN: //CompoundStatement -> "begin" StatementSequence "end", rule #25
		parseTree->LogExpansion(25);
		Match(MP_BEGIN);
		StatementSequence();
		Match(MP_END);
		break;
		//case MP_RETURN:	//added for function, no rul
		//	parseTree->LogMessage("No rule defined. Function immediately returns.");
		//	Match(MP_RETURN);
		//	StatementSequence();
		//	break;
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
	case MP_SCOLON: //StatementSequence -> Statement StatementTail, rule #26 {e}
	case MP_END:  //StatementSequence -> Statement StatementTail, rule #26 {e}
	case MP_BEGIN: //StatementSequence -> Statement StatementTail, rule #26
	case MP_READ: //StatementSequence -> Statement StatementTail, rule #26
	case MP_WRITE: //StatementSequence -> Statement StatementTail, rule #26
	case MP_WRITELN://added
	case MP_IF: //StatementSequence -> Statement StatementTail, rule #26
	case MP_REPEAT: //StatementSequence -> Statement StatementTail, rule #26
	case MP_IDENTIFIER: //StatementSequence -> Statement StatementTail, rule #26
	case MP_FOR: //StatementSequence -> Statement StatementTail, rule #26
	case MP_WHILE: //StatementSequence -> Statement StatementTail, rule #26
		//case MP_RETURN: //added for function
		parseTree->LogExpansion(26);
		Statement();
		StatementTail();
		break;
		//case MP_VAR:	//DEBUG
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
	case MP_SCOLON://StatementTail -> ";" Statement StatementTail , rule #27
		parseTree->LogExpansion(27);
		Match(MP_SCOLON);
		Statement();
		StatementTail();
		break;
	case MP_END://StatementTail -> e , rule #28
	case MP_UNTIL:   // This may not be correct repeat until statements may should be bracketed in begin end?
		parseTree->LogExpansion(28);
		break;
	case MP_RETURN:
		Match(MP_RETURN);
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
	record = new SemanticRecord(); // create new semantic record here

	switch(lookahead)
	{	
	case MP_END: //Statement -> EmptyStatement, rule #29
	case MP_SCOLON: //Statement -> EmptyStatement, rule #29
	case MP_UNTIL:
	case MP_ELSE:
		//case MP_RETURN: //added for function
		parseTree->LogExpansion(29);
		EmptyStatement();
		break;
	case MP_BEGIN: //Statement -> CompoundStatement, rule #30
		parseTree->LogExpansion(30);
		CompoundStatement();
		break;
	case MP_READ: //Statement -> ReadStatement, rule #31
		parseTree->LogExpansion(31);
		ReadStatement();
		break;
	case MP_WRITE: //Statement -> WriteStatement, rule #32
	case MP_WRITELN://added
		parseTree->LogExpansion(32);
		WriteStatement();
		break;
	case MP_IDENTIFIER: //Statement -> AssignmentStatement, rule #33 //Unless should be a procedure or function
		parseTree->LogExpansion(33);
		AssignmentStatement();
		break;
	case MP_IF: //Statement -> IfStatement, rule #34
		parseTree->LogExpansion(34);
		IfStatement();
		break;
	case MP_WHILE: //Statement -> WhileStatement, rule #35
		parseTree->LogExpansion(35);
		WhileStatement();
		break;
	case MP_REPEAT: //Statement -> RepeatStatement, rule #36
		parseTree->LogExpansion(36);
		RepeatStatement();
		break;
	case MP_FOR: //Statement -> ForStatement, rule #37
		parseTree->LogExpansion(37);
		ForStatement();
		break;
		//case MP_IDENTIFIER: //Statement -> ProcedureStatement, rule #38			// DEBUG - this case block is commented out but has a rule for it
		//	parseTree->LogExpansion(38);
		//	ProcedureStatement();
		//	break;
	default: //everything else
		Syntax_Error();
		break;
	}

	delete record;
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::EmptyStatement()
{
	switch(lookahead)
	{
	case MP_SCOLON:
	case MP_END:  // EmptyStatement -> e Rule #38 
	case MP_UNTIL:
	case MP_ELSE:
		//case MP_RETURN: //added for function
		parseTree->LogExpansion(38);
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
	case MP_READ:  // ReadStatement -> "read" "(" ReadParameter ReadParameterTail ")"    Rule #40
		parseTree->LogExpansion(40);
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
	case MP_COMMA:  // ReadParameterTail -> "," ReadParameter ReadParameterTail		Rule# 42
		parseTree->LogExpansion(42);
		Match(MP_COMMA);
		ReadParameter();
		ReadParameterTail();
		break;
	case MP_RPAREN:	// ReadParameterTail -> e 		Rule# 42
		parseTree->LogExpansion(42);
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
	case MP_IDENTIFIER: // ReadParameter -> VariableIdentifier		Rule# 43
		parseTree->LogExpansion(43);
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
	string v = scanner->lexeme();

	switch(lookahead)
	{
	case MP_WRITELN://added		// WriteStatement -> "writeln" "(" WriteParameter WriteParameterTail ")"		Rule# 111
		parseTree->LogExpansion(44);
		Match(MP_WRITELN);
		Match(MP_LPAREN);
		WriteParameter();		
		WriteParameterTail();
		Match(MP_RPAREN);
		break;
	case MP_WRITE: // WriteStatement -> "write" "(" WriteParameter WriteParameterTail ")"	Rule# 44
		parseTree->LogExpansion(44);
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
	case MP_COMMA:  // WriteParameterTail -> "," WriteParameter		Rule# 45
		parseTree->LogExpansion(45);
		Match(MP_COMMA);
		WriteParameter();
		WriteParameterTail();	// DEBUG - this was not here but in the cfg
		break;
	case MP_RPAREN: // WriteParameterTail -> e		Rule# 46
		parseTree->LogExpansion(46);
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
	string v = scanner->lexeme();

	switch(lookahead)
	{
	case MP_PLUS:
	case MP_MINUS:  // WriteParameter -> OrdinalExpression		Rule# 47
	case MP_UNSIGNEDINTEGER:
	case MP_INTEGER_LIT:
	case MP_STRING:	// added
	case MP_IDENTIFIER: //added
		parseTree->LogExpansion(47);
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
	string v = scanner->lexeme();

	switch (lookahead)
	{
	case MP_IDENTIFIER: // AssignmentStatement -> VariableIdentifier ":=" Expression		Rule# 48
		parseTree->LogExpansion(48);		
		VariableIdentifier();
		Match(MP_ASSIGN);
		Expression();
		break;
		//	DEBUG - see rule 49, need to find follow set and add rule
	default:
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::IfStatement()
{
	switch (lookahead)
	{
	case MP_IF: // IfStatement -> "if" BooleanExpression "then" Statement OptionalElsePart		Rule# 50
		parseTree->LogExpansion(50);
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
	case MP_ELSE: // OptionalElsePart -> "else" Statement		Rule# 51
		parseTree->LogExpansion(51);
		Match(MP_ELSE);
		Statement();
		break;
		//case MP_ELSE CONFLICT POSSIBLE
	case MP_END:
	case MP_SCOLON: // OptionalElsePart -> e	Rule #52
		parseTree->LogExpansion(52);
		break;
	default:
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::RepeatStatement()
{
	switch(lookahead)
	{
	case MP_REPEAT: // RepeatStatement -> "repeat" StatementSequence "until" BooleanExpression		Rule# 53
		parseTree->LogExpansion(53);
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
	case MP_WHILE: // WhileStatement -> "while" BooleanExpression "do" Statement		Rule# 54
		parseTree->LogExpansion(54);
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
	case MP_FOR: // "for" ControlVariable ":=" InitialValue StepValue FinalValue "do" Statement		Rule# 55
		parseTree->LogExpansion(55);
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
	case MP_IDENTIFIER: // ControlVariable -> VariableIdentifier Identifier		Rule# 56
		parseTree->LogExpansion(56);
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
	case MP_PLUS: // InitialValue -> OrdinalExpression		Rule# 57
	case MP_INTEGER_LIT:
	case MP_UNSIGNEDINTEGER:
		parseTree->LogExpansion(57);
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
	case MP_TO: // StepValue -> "to"		Rule# 58
		parseTree->LogExpansion(58);
		Match(MP_TO);
		break;
	case MP_DOWNTO: // StepValue -> "downto" 	 Rule# 59
		parseTree->LogExpansion(59);
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
	case MP_MINUS: // FinalValue -> OrdinalExpression	Rule# 60
	case MP_INTEGER_LIT:
	case MP_UNSIGNEDINTEGER:
		parseTree->LogExpansion(60);
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
	case MP_IDENTIFIER: // ProcedureStatement -> ProcedureIdentifier OptionalActualParameterList		Rule# 61
		parseTree->LogExpansion(61);
		Match(MP_IDENTIFIER);
		OptionalActualParameterList();
		break;
	default:
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::OptionalActualParameterList()
{
	switch(lookahead)
	{
	case MP_LPAREN: // OptionalActualParameterList -> "(" ActualParameter ActualParameterTail ")"		Rule# 62
		parseTree->LogExpansion(62);
		Match(MP_LPAREN);
		ActualParameter();
		ActualParameterTail();
		Match(MP_RPAREN);
		break;
	case MP_END:
	case MP_IDENTIFIER: // OptionalActualParameterList -> e		Rule # 63
		parseTree->LogExpansion(63);
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
	case MP_COMMA: // ActualParameterTail -> "," ActualParameter ActualParameterTail Rule #64
		parseTree->LogExpansion(64);
		Match(MP_COMMA);
		ActualParameter();
		ActualParameterTail();
		break;
	case MP_RPAREN: // ActualParameterTail -> e		Rule #65
		parseTree->LogExpansion(65);
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
	case MP_MINUS: // ActualParameter -> OrdinalExpression 	Rule# 66
		parseTree->LogExpansion(66);
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
	case MP_IDENTIFIER:
	case MP_INTEGER_LIT:
	case MP_UNSIGNEDINTEGER:
	case MP_MINUS:
	case MP_PLUS: // Expression -> SimpleExpression OptionalRelationalPart 	Rule# 67
	case MP_STRING: // added for strings
		parseTree->LogExpansion(67);
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
	case MP_NEQUAL: // OptionalRelationalPart -> RelationalOperator SimpleExpression	Rule #68
		parseTree->LogExpansion(68);
		RelationalOperator();
		SimpleExpression();
		break;
	case MP_SCOLON:
	case MP_END:
	case MP_THEN:
	case MP_ELSE:
	case MP_RPAREN:
	case MP_TO:
	case MP_DOWNTO:
	case MP_DO: // OptionalRelationalPart -> e		Rule #69
	case MP_COMMA: //added
		parseTree->LogExpansion(69);
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
	case MP_EQUAL:	// RelationalOperator -> "="		Rule# 70
		parseTree->LogExpansion(70);
		Match(MP_EQUAL);
		break;
	case MP_LTHAN: // RelationalOperator -> "<"		Rule# 71
		parseTree->LogExpansion(71);
		Match(MP_LTHAN);
		break;
	case MP_GTHAN: // RelationalOperator -> ">"		Rule# 72
		parseTree->LogExpansion(72);
		Match(MP_GTHAN);
		break;
	case MP_LEQUAL: // RelationalOperator -> "<="		Rule# 73
		parseTree->LogExpansion(73);
		Match(MP_LEQUAL);
		break;
	case MP_GEQUAL: // RelationalOperator -> ">="	Rule# 74
		parseTree->LogExpansion(74);
		Match(MP_GEQUAL);
		break;
	case MP_NEQUAL: // RelationalOperator -> "<>"	Rule# 75
		parseTree->LogExpansion(75);
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
	string v = scanner->lexeme();

	switch(lookahead)
	{
	case MP_IDENTIFIER:
	case MP_UNSIGNEDINTEGER:
	case MP_INTEGER_LIT:
	case MP_MINUS:
	case MP_PLUS: // SimpleExpression -> OptionalSign Term TermTail  +,-		Rule# 76
	case MP_STRING: // added
		parseTree->LogExpansion(76);
		OptionalSign();
		Term();
		TermTail();
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
	case MP_MINUS:// TermTail -> AddingOperator Term TermTail  	Rule# 77
		parseTree->LogExpansion(77);
		AddingOperator();
		Term();
		TermTail();
	case MP_END:
	case MP_SCOLON: // TermTail -> {e} 		Rule# 78
	case MP_EQUAL:
	case MP_LTHAN:
	case MP_GTHAN:
	case MP_GEQUAL:
	case MP_LEQUAL:
	case MP_DO:
	case MP_THEN:
	case MP_RPAREN:
	case MP_TO:
	case MP_DOWNTO:
	case MP_ELSE:
	case MP_COMMA: //added
		parseTree->LogExpansion(78);
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
	case MP_PLUS: // OptionalSign -> "+"	Rule #79
		parseTree->LogExpansion(79);
		Match(MP_PLUS);
		break;
	case MP_MINUS: // OptionalSign -> "-"	Rule #80
		parseTree->LogExpansion(80);
		Match(MP_MINUS);
		break;
	case MP_IDENTIFIER:
	case MP_NOT:
	case MP_UNSIGNEDINTEGER:
	case MP_INTEGER_LIT:
	case MP_RPAREN:
	case MP_LPAREN: // OptionalSign -> {e}	Rule #81
	case MP_STRING: //added
		parseTree->LogExpansion(81);
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
	case MP_PLUS: // AddingOperator -> "+"  	Rule# 82
		parseTree->LogExpansion(82);
		Match(MP_PLUS);
		break;
	case MP_MINUS: // AddingOperator -> "-"  	Rule# 83
		parseTree->LogExpansion(83);
		Match(MP_MINUS);
		break;
	case MP_OR: // AddingOperator -> "or" 	Rule# 84
		parseTree->LogExpansion(84);
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
	string v = scanner->lexeme();

	switch(lookahead)
	{
	case MP_UNSIGNEDINTEGER:
	case MP_INTEGER_LIT:
	case MP_NOT:
	case MP_IDENTIFIER: // Term -> Factor FactorTail  	Rule# 85
	case MP_STRING: //added
		parseTree->LogExpansion(85);
		Factor();
		FactorTail();
		//TermTail();		// DEBUG - this is not in the grammar
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
	case MP_AND: // FactorTail -> MultiplyingOperator Factor FactorTail  	Rule# 86
		parseTree->LogExpansion(86);
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
	case MP_SCOLON:
	case MP_END:
	case MP_DO:
	case MP_THEN:
	case MP_ELSE:
	case MP_TO:
	case MP_DOWNTO:
	case MP_NEQUAL: // FactorTail -> {e}	Rule# 87
	case MP_STRING: //added
	case MP_COMMA: //added
		parseTree->LogExpansion(87);
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
	case MP_TIMES: // MultiplyingOperator -> "*"  	Rule# 88
		parseTree->LogExpansion(88);
		Match(MP_TIMES);
		break;
	case MP_DIV: // MultiplyingOperator -> "div"  	Rule# 89
		parseTree->LogExpansion(89);
		Match(MP_DIV);
		break;
	case MP_MOD: // MultiplyingOperator -> "mod"  	Rule# 90
		parseTree->LogExpansion(90);
		Match(MP_MOD);
		break;
	case MP_AND: // MultiplyingOperator -> "and"  	Rule# 91
		parseTree->LogExpansion(91);
		Match(MP_AND);
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
	string v = scanner->lexeme();

	switch(lookahead)
	{
	case MP_UNSIGNEDINTEGER: // Factor -> UnsignedInteger  	Rule# 92
		parseTree->LogExpansion(92);
		Match(MP_UNSIGNEDINTEGER);
		break;
		//////////////////////// Conflict 96, 99
	case MP_IDENTIFIER:		// Factor -> VariableIdentifier		Rule # 93
		parseTree->LogExpansion(96);
		VariableIdentifier();
		break;
	case MP_NOT: // "not" Factor  	Rule# 94
		parseTree->LogExpansion(94);
		Match(MP_NOT);
		Factor();
		break;
	case MP_INTEGER_LIT: // Factor -> UnsignedInteger  	Rule# 95		// DEBUG - conflict
		parseTree->LogExpansion(95);
		Match(MP_INTEGER_LIT);
		break;
	case MP_LPAREN: // Factor -> "(" Expression ")"  	Rule# 95
		parseTree->LogExpansion(95);
		Match(MP_LPAREN);
		Expression();
		Match(MP_RPAREN);
		break;
		//////////////////////// Conflict 96, 99
	case MP_STRING:	//added
		parseTree->LogExpansion(114);
		Match(MP_STRING);
		break;
	default: //everything else
		Syntax_Error();
		break;
	}

	// set right side of record
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ProgramIdentifier()
{
	switch(lookahead)
	{
	case MP_IDENTIFIER: // ProgramIdentifier -> Identifier  	Rule# 97
		parseTree->LogExpansion(97);
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
	string v = scanner->lexeme();

	switch(lookahead)
	{
	case MP_IDENTIFIER: // VariableIdentifier -> Identifier  	Rule# 98
		parseTree->LogExpansion(98);
		symbolTable->insertRecord(scanner->lexeme(), SymbolTable::KIND_VARIABLE, scanner->token(), scanner->line(), scanner->column());
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
	case MP_IDENTIFIER: // ProcedureIdentifier -> Identifier 	Rule# 99
		parseTree->LogExpansion(99);

		// create table for new procedure, create new scope table for that procedure
		symbolTable->insertRecord(scanner->lexeme(), SymbolTable::KIND_PROCEDURE, MP_NULL, scanner->line(), scanner->column());
		symbolTable->createTable();
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
	case MP_IDENTIFIER: // FunctionIdentifier -> Identifier 	Rule# 100
		parseTree->LogExpansion(100);

		// add the function to the symbol table
		symbolTable->insertRecord(scanner->lexeme(), SymbolTable::KIND_FUNCTION, scanner->token(), scanner->line(), scanner->column());
		symbolTable->createTable();

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
	case MP_INTEGER_LIT:
	case MP_NOT:
	case MP_IDENTIFIER: // BooleanExpression -> Expression 	Rule# 101
		parseTree->LogExpansion(101);
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
	case MP_INTEGER_LIT:
		//case MP_RPAREN:
	case MP_LPAREN:
	case MP_PLUS: // OrdinalExpression -> Expression	Rule# 102
	case MP_STRING: // added
		parseTree->LogExpansion(102);
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
	case MP_IDENTIFIER: // IdentifierList -> Identifier IdentifierTail Rule# 103
		parseTree->LogExpansion(103);
		symbolTable->insertRecord(scanner->lexeme(), SymbolTable::KIND_VARIABLE, scanner->token(), scanner->line(), scanner->column());
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
	case MP_COMMA: // IdentifierTail -> "," Identifier IdentifierTail  		Rule# 104
		parseTree->LogExpansion(104);
		Match(MP_COMMA);
		symbolTable->insertRecord(scanner->lexeme(), SymbolTable::KIND_VARIABLE, scanner->token(), scanner->line(), scanner->column());
		Match(MP_IDENTIFIER);		
		IdentifierTail();
		break;
	case MP_COLON: // IdentifierTail -> e  		Rule# 105
		parseTree->LogExpansion(105);
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}


// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
Token Parser::Type()
{
	switch(lookahead)
	{
	case MP_INTEGER_LIT: //Type -> "Integer", rule #107
		parseTree->LogExpansion(107);
		Match(MP_INTEGER_LIT);
		// insert into symbol table here
		return MP_INTEGER_LIT;
	case MP_FLOAT_LIT: //Type -> "Float", rule #108
		parseTree->LogExpansion(108);
		Match(MP_FLOAT_LIT);
		return MP_FLOAT_LIT;
	case MP_STRING:	// Type -> "String", rule #109
		parseTree->LogExpansion(109);
		Match(MP_STRING);
		return MP_STRING;
	case MP_BOOLEAN: //Type -> "Boolean", rule #110
		parseTree->LogExpansion(110);
		Match(MP_BOOLEAN);
		return MP_BOOLEAN;
	case MP_REAL:	// Type -> "Float"  (aka Real)		Rule# 108		//added to support real data type (float)
		parseTree->LogExpansion(108);
		Match(MP_REAL);
		return MP_REAL;
	case MP_IN:	//added to support in parameters
		parseTree->LogMessage("No rule defined. In parameter matched.");
		Match(MP_IN);
		return Type();
	case MP_CHARACTER:	// added to support character data type		
		parseTree->LogMessage("No rule defined. Character data type matched.");
		Match(MP_CHARACTER);
		return MP_CHARACTER;
	default: //everything else
		Syntax_Error();
		return MP_NULL;
	}
}



// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::Match(Token token)
{
	// puts a token on the tree
	// gets the next lookahead
	if (token == lookahead)
	{
		parseTree->LogMessage("      Matched: " + EnumToString(token) + " = " + scanner->lexeme());
		currentLexeme = scanner->lexeme();
		currentToken = scanner->token();
		lookahead = scanner->getToken();
	}
	else
		Syntax_Error(token);
}

void Parser::Syntax_Error(Token expected)
{
	//stops everything and gives a meaningful error message 
	string msg = "";
	msg.append("\nFile: " + fileName + ": \nSyntax error found on line " + to_string(scanner->line()) + ", column " + to_string(scanner->column()) + 
		".\n    Expected " + EnumToString(expected) + " but found " + EnumToString(lookahead) + ".\n    Next token: " + EnumToString(scanner->getToken())
		+ "\n    Next Lexeme: " + scanner->lexeme());
	cout << msg;
	parseTree->LogMessage(msg);
	throw -1;
}
