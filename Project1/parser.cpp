#include "Parser.h"


Parser::Parser(void)
{
}

Parser::Parser(string fName)
{
	// create our scanner and parse trees
	symbolTable = new SymbolTable();
	/*analyzer = new SemanticAnalyzer(symbolTable);*/
	scanner = new Scanner();
	parseTree = new ParseTree("parse_tree.txt");
	parseTree->ReadCFGRules("CFG_rules.txt");
	setInputFile(fName);
	caller = new SemanticRecord();
	//analyzer= new SemanticAnalyzer("program.txt");
	labelCount=0;
	// open the ir file
	irFilename = "uCode.pas";
	irFile.open(irFilename, ofstream::out);
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
	irFile.close();

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

		// generate begin assembly
		Gen_Assembly("MOV SP D0");
		Program();
		Match(MP_EOF);

		// generate end assembly
		Gen_Assembly("MOV D0 SP");
		Gen_Assembly("HLT");
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
		caller->setKind(SemanticRecord::PROGRAM);
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
	case MP_VAR: // Rule# 4		Block -> VariableDeclarationPart ProcedureAndFunctionDeclarationPart StatementPart 
		parseTree->LogExpansion(4);
		VariableDeclarationPart();
		ProcedureAndFunctionDeclarationPart();
		StatementPart();
		break;
	case MP_PROCEDURE: // Block -> ProcedureAndFunctionDeclarationPart when VariableDeclarationPart -> e
	case MP_FUNCTION: // Block -> ProcedureAndFunctionDeclarationPart when VariableDeclarationPart -> e
	case MP_BEGIN: // Block -> StatementPart when ProcedureAndFunctionDeclarationPart and VariableDeclarationPart -> e
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
	case MP_PROCEDURE:       // VariableDeclarationPart -> e		Rule# 106	
	case MP_FUNCTION:       // VariableDeclarationPart -> e		Rule# 106
	case MP_BEGIN:       // VariableDeclarationPart -> e		Rule# 106
		parseTree->LogExpansion(106);
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
	caller->setKind(SemanticRecord::DECLARATION);

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

	// update records in symbol table with data type
	int end = symbolTable->tableSize();
	int diff = symbolTable->tableSize() - start;
	for (int i = start; i < end; i++)
	{
		SymbolTable::Record* rec = symbolTable->lookupRecord(i);
		if (rec != NULL)
			rec->token = type; 
	}

	//add differencing offset for variable
	Gen_Assembly("ADD SP #" + to_string(diff) + " SP");
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
	/*SymbolTable::Record* r = symbolTable->lookupRecord(offset, 0);
	if (r != NULL)
	{
		r->token = type;
	}*/
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
	case MP_INTEGER_LIT:
	case MP_FLOAT_LIT:
	case MP_FIXED_LIT:
	case MP_REAL:
	case MP_STRING:
	case MP_BOOLEAN:
	case MP_SCOLON: //OptionalFormalParameterList -> e, rule #17
	//case MP_COLON: //OptionalFormalParameterList -> e, rule #17
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

	/*int end = symbolTable->tableSize();
	for (int i = start; i < end; i++)
	{
		SymbolTable::Record* r = symbolTable->lookupRecord(i);
		if (r != NULL)
		{
			r->token = type;
		}
	}*/

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
	//case MP_SCOLON: //StatementSequence -> Statement StatementTail, rule #26 {e}
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
	case MP_UNTIL:   // This is correct do not comment again
		parseTree->LogExpansion(28);
		break;
	//case MP_RETURN:
	//	Match(MP_RETURN);
	//	break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::Statement()
{
	SemanticRecord* expressionRec = new SemanticRecord();
	switch(lookahead)
	{	
	case MP_END: //Statement -> EmptyStatement, rule #29
	case MP_SCOLON: //Statement -> EmptyStatement, rule #29
	//case MP_UNTIL:
	//case MP_ELSE:
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
		WriteStatement(expressionRec);
		break;
	case MP_IDENTIFIER: //Statement -> AssignmentStatement, rule #33 //Unless should be a procedure or function
		parseTree->LogExpansion(33);
		AssignmentStatement(expressionRec);
		break;
	case MP_IF: //Statement -> IfStatement, rule #34
		parseTree->LogExpansion(34);
		IfStatement(expressionRec);
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
		ForStatement(expressionRec);
		break;
		//case MP_IDENTIFIER: //Statement -> ProcedureStatement, rule #38			// DEBUG - this case block is commented out but has a rule for it
		//	parseTree->LogExpansion(38);
		//	ProcedureStatement();
		//	break;
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
	case MP_END:  // EmptyStatement -> e Rule #38 
	//case MP_UNTIL:
	//case MP_ELSE:
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
	case MP_READ:  {// ReadStatement -> "read" "(" ReadParameter ReadParameterTail ")"    Rule #40
		parseTree->LogExpansion(40);
		Match(MP_READ);
		Match(MP_LPAREN);
		
		// generate read assembly based on the variables data type
		SymbolTable::Record* tempRecord = symbolTable->lookupRecord(scanner->getLexeme(), SymbolTable::KIND_VARIABLE, 0);
		
		if (tempRecord->token == MP_FLOAT_LIT || tempRecord->token == MP_FIXED_LIT)
		{
			Gen_Assembly("RDF D9		; Read Var " + tempRecord->name);
			Gen_Assembly("MOV D9 " + to_string(tempRecord->offset) + "(D0)");
		} 
		else if (tempRecord->token == MP_STRING)
		{
			Gen_Assembly("RDS D9		; Read Var " + tempRecord->name);
			Gen_Assembly("MOV D9 " + to_string(tempRecord->offset) + "(D0)");
		}
		else  if (tempRecord->token == MP_INTEGER_LIT)
		{
			Gen_Assembly("RD D9			; Read Var " + tempRecord->name);
			Gen_Assembly("MOV D9 " + to_string(tempRecord->offset) + "(D0)");
		}
		
		ReadParameter();
		ReadParameterTail();
		Match(MP_RPAREN);
		break;
	}
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
	case MP_COMMA: {  // ReadParameterTail -> "," ReadParameter ReadParameterTail		Rule# 42
		parseTree->LogExpansion(42);
		Match(MP_COMMA);

		// generate read assembly based on data type
		SymbolTable::Record* tempRecord = symbolTable->lookupRecord(scanner->getLexeme(), SymbolTable::KIND_VARIABLE, 0);
		if (tempRecord->token == MP_FLOAT_LIT || tempRecord->token == MP_FIXED_LIT)
		{
			Gen_Assembly("RDF D9		; Read Var " + tempRecord->name);
			Gen_Assembly("MOV D9 " + to_string(tempRecord->offset) + "(D0)");
		} 
		else if (tempRecord->token == MP_STRING)
		{
			Gen_Assembly("RDS D9		; Read Var " + tempRecord->name);
			Gen_Assembly("MOV D9 " + to_string(tempRecord->offset) + "(D0)");
		}
		else  if (tempRecord->token == MP_INTEGER_LIT)
		{
			Gen_Assembly("RD D9		; Read Var " + tempRecord->name);
			Gen_Assembly("MOV D9 " + to_string(tempRecord->offset) + "(D0)");
		}

		ReadParameter();
		ReadParameterTail();
		break;
	}
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
void Parser::WriteStatement(SemanticRecord* &expressionRec)
{
	string v = scanner->getLexeme();

	switch(lookahead)
	{
	case MP_WRITELN://added		// WriteStatement -> "writeln" "(" WriteParameter WriteParameterTail ")"		Rule# 111
		parseTree->LogExpansion(44);
		caller->setType(MP_WRITELN);
		Match(MP_WRITELN);
		Match(MP_LPAREN);
		WriteParameter(expressionRec);		
		WriteParameterTail(expressionRec);
		Match(MP_RPAREN);
		break;
	case MP_WRITE: // WriteStatement -> "write" "(" WriteParameter WriteParameterTail ")"	Rule# 44
		parseTree->LogExpansion(44);
		caller->setType(MP_WRITE);
		Match(MP_WRITE);
		Match(MP_LPAREN);
		WriteParameter(expressionRec);		
		WriteParameterTail(expressionRec);
		Match(MP_RPAREN);
		break;
	default:
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::WriteParameterTail(SemanticRecord* &expressionRec)
{
	switch(lookahead)
	{
	case MP_COMMA:  // WriteParameterTail -> "," WriteParameter		Rule# 45
		parseTree->LogExpansion(45);
		Match(MP_COMMA);
		WriteParameter(expressionRec);
		WriteParameterTail(expressionRec);	// DEBUG - this was not here but in the cfg
		break;
	case MP_RPAREN: // WriteParameterTail -> e		Rule# 46
		parseTree->LogExpansion(46);

		if (caller->getType() == MP_WRITELN)
		{
			Gen_Assembly("WRTLN #\"\"");
		}
		break;
	default:
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::WriteParameter(SemanticRecord* &expressionRec)
{
	string v = scanner->getLexeme();

	switch(lookahead)
	{
	case MP_PLUS:				// WriteParameter -> OrdinalExpression		Rule# 47, when OrdinalExpression starts with an OptionalSign
	case MP_MINUS:				// WriteParameter -> OrdinalExpression		Rule# 47, when OrdinalExpression starts with an OptionalSign
	case MP_INTEGER_LIT:		// WriteParameter -> OrdinalExpression		Rule# 47, OptionalSign -> e and OrdinalExpression starts with Term (-> factor -> UnsignedInteger))
	case MP_UNSIGNEDINTEGER:	// WriteParameter -> OrdinalExpression		Rule# 47, OptionalSign -> e and OrdinalExpression starts with Term (-> factor -> UnsignedInteger))
	case MP_NOT:				// WriteParameter -> OrdinalExpression		Rule# 47, OptionalSign -> e and OrdinalExpression starts with Term (-> factor -> "not" factor))
	case MP_LPAREN:				// WriteParameter -> OrdinalExpression		Rule# 47, OptionalSign -> e and OrdinalExpression starts with Term (-> factor -> "(" expression ")"))
	case MP_FLOAT_LIT:			// WriteParameter -> OrdinalExpression		Rule# 47, OptionalSign -> e and OrdinalExpression starts with Term (-> factor -> unsignedFloat))
	case MP_FIXED_LIT:			// WriteParameter -> OrdinalExpression		Rule# 47, OptionalSign -> e and OrdinalExpression starts with Term (-> factor -> unsignedFloat))
	case MP_STRING:				// WriteParameter -> OrdinalExpression		Rule# 47, OptionalSign -> e and OrdinalExpression starts with Term (-> factor -> stringLiteral))
	case MP_TRUE:				// WriteParameter -> OrdinalExpression		Rule# 47, OptionalSign -> e and OrdinalExpression starts with Term (-> factor -> "true"))
	case MP_FALSE:				// WriteParameter -> OrdinalExpression		Rule# 47, OptionalSign -> e and OrdinalExpression starts with Term (-> factor -> "false"))
		Gen_Assembly("WRT #\"" + scanner->getLexeme() + "\"");
		//Gen_Assembly("WRT #\"" + scanner->getLexeme().substr(1, scanner->getLexeme().length()-2) + "\"");
		Match(MP_STRING);
		break;
	case MP_IDENTIFIER:{			// WriteParameter -> OrdinalExpression		Rule# 47, OptionalSign -> e and OrdinalExpression starts with Term (-> factor -> VariableIdentifier | FunctionIdentifier))
		// lookup identifier from the symbol table and make assembly for it	
		SymbolTable::Record* r = symbolTable->lookupRecord(scanner->getLexeme(), SymbolTable::KIND_VARIABLE);
		Gen_Assembly("WRT " + to_string(r->offset) + "(D0)");
		Match(MP_IDENTIFIER);
		break;
	}
	default:
		parseTree->LogExpansion(47);
		OrdinalExpression(expressionRec);
		Gen_Assembly("WRTS");
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::AssignmentStatement(SemanticRecord* &expressionRec)
{
	SymbolTable::Record* assignmentRecord = symbolTable->lookupRecord(scanner->getLexeme(), SymbolTable::KIND_VARIABLE, 0);
	
	switch (lookahead)
	{
	case MP_IDENTIFIER: // AssignmentStatement -> VariableIdentifier ":=" Expression		Rule# 48
		parseTree->LogExpansion(48);	
		
		caller->setKind(SemanticRecord::ASSIGNMENT);
		VariableIdentifier();
		Match(MP_ASSIGN);
		Expression(expressionRec);

		// pop top value back into variable
		if (expressionRec->getType()==assignmentRecord->token)
		{
			Gen_Assembly("POP " + to_string(assignmentRecord->offset) + "(D0)");
		}
		else
			if (assignmentRecord->token==MP_FLOAT_LIT)
			{
				Gen_Assembly("CASTF ;result does not match assignemnt variable type cast to float");
				Gen_Assembly("POP " + to_string(assignmentRecord->offset) + "(D0)");
			}
		else
			if (assignmentRecord->token==MP_INTEGER_LIT)
			{
				Gen_Assembly("CASTI ; result does not match assignemnt variable type cast to integer");
				Gen_Assembly("POP " + to_string(assignmentRecord->offset) + "(D0)");
			}

		break;
		//	DEBUG - see rule 49, need to find follow set and add rule
	default:
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::IfStatement(SemanticRecord* &expressionRec)
{
	string ifFalseLabel;
	
	
	switch (lookahead)
	{
	case MP_IF: // IfStatement -> "if" BooleanExpression "then" Statement OptionalElsePart		Rule# 50
		parseTree->LogExpansion(50);
		
		caller->setKind(SemanticRecord::SIMPLE_PARAMETER);
		Match(MP_IF);
		ifFalseLabel=LabelMaker();
		BooleanExpression();
		// The Result of booleanExpression should be the top of the stack
		Gen_Assembly("BRFS "+ifFalseLabel);
		Match(MP_THEN);
		Statement();
		Gen_Assembly(ifFalseLabel);
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
		{
		parseTree->LogExpansion(53);
		string repeatStartLabel;
		
		Match(MP_REPEAT);
		repeatStartLabel=LabelMaker();
		Gen_Assembly(repeatStartLabel);
		StatementSequence();
		Match(MP_UNTIL);
		BooleanExpression();
		Gen_Assembly("BRTS "+ repeatStartLabel); //If the repeat test is false we just fall through no need for another label
		break;
		}
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
		{
		parseTree->LogExpansion(54);
		string whileTestLabel;
		string whileFalseLabel;
		Match(MP_WHILE);
		whileTestLabel=LabelMaker();
		whileFalseLabel=LabelMaker();
		Gen_Assembly(whileTestLabel);
		BooleanExpression();
		// The result of BooleanExpression is on the top of the stack
		
		Match(MP_DO);
		
		Gen_Assembly("BRFS "+ whileFalseLabel);
		Statement();
		Gen_Assembly("BR "+ whileTestLabel);
		Gen_Assembly(whileFalseLabel);
		break;
		}
	default:
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::ForStatement(SemanticRecord* &expressionRec)
{
	
	switch(lookahead)
	{
	case MP_FOR: // "for" ControlVariable ":=" InitialValue StepValue FinalValue "do" Statement		Rule# 55
		{
		string forTestLabel;
		string forFalseLabel;
		Token stepType;
		parseTree->LogExpansion(55);
		Match(MP_FOR);
		// I think we can only have int lits here so commented this bit 
		//caller->setKind(SemanticRecord::ASSIGNMENT);
		//SymbolTable::Record* assignmentRecord = symbolTable->lookupRecord(scanner->getLexeme(), SymbolTable::KIND_VARIABLE, 0);
		ControlVariable();
		
		Match(MP_ASSIGN);
		InitialValue(expressionRec);
			
	/*	if (expressionRec->getType()==assignmentRecord->token)
		{
			Gen_Assembly("POP " + to_string(assignmentRecord->offset) + "(D0)");
		}
		else
			if (assignmentRecord->token==MP_FLOAT_LIT)
			{
				Gen_Assembly("CASTF ; expression result does not match assignemnt variable type cast to float");
				Gen_Assembly("POP " + to_string(assignmentRecord->offset) + "(D0)");
			}
		else

			if (assignmentRecord->token==MP_INTEGER_LIT)
			{
				Gen_Assembly("CASTI ; expression result does not match assignemnt variable type cast to integer");
				Gen_Assembly("POP " + to_string(assignmentRecord->offset) + "(D0)");
			}*/
		stepType=StepValue();
		FinalValue(expressionRec);
		
		
		Match(MP_DO);
		forTestLabel=LabelMaker();
		forFalseLabel=LabelMaker();
		Gen_Assembly(forTestLabel);
		
			//Test for condition
			if (stepType==MP_TO)
			{Gen_Assembly("BGT -2(SP) -1(SP) "+ forFalseLabel);}
			else 
			{Gen_Assembly("BLT -2(SP) -1(SP) "+ forFalseLabel);}
		

		Statement();
		
			if (stepType==MP_TO)
			{
				//increment the control variable
				Gen_Assembly("PUSH #1");
				Gen_Assembly("PUSH -2(SP)");
				Gen_Assembly("ADDS");
				Gen_Assembly("POP -2(SP)");
				
			}
			else 
			{
				//decrement the control variable
				Gen_Assembly("PUSH #1");
				Gen_Assembly("NEGS");
				Gen_Assembly("PUSH -2(SP)");
				Gen_Assembly("ADDS");
				Gen_Assembly("POP -2(SP)");
				
			}
		// return to start of for loop
		Gen_Assembly("BR "+ forTestLabel);
		// Place loop exit label
		Gen_Assembly(forFalseLabel);
		break;
		}
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
void Parser::InitialValue(SemanticRecord* &expressionRec)
{
	switch(lookahead)
	{	
	case MP_PLUS:				// InitialValue -> OrdinalExpression		Rule# 57
	case MP_MINUS:				// See WriteParameter for explaination of cases
	case MP_INTEGER_LIT:
	case MP_UNSIGNEDINTEGER:
	case MP_IDENTIFIER:
	case MP_NOT:
	case MP_LPAREN:
	case MP_FLOAT_LIT:	
	case MP_FIXED_LIT:
	case MP_STRING:
	case MP_TRUE:
	case MP_FALSE:
		parseTree->LogExpansion(57);
		OrdinalExpression(expressionRec);
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
Token Parser::StepValue()
{
	Token stepType;
	switch(lookahead)
	{
	case MP_TO: // StepValue -> "to"		Rule# 58
		parseTree->LogExpansion(58);
		Match(MP_TO);
		stepType=MP_TO;
		break;
	case MP_DOWNTO: // StepValue -> "downto" 	 Rule# 59
		parseTree->LogExpansion(59);
		Match(MP_DOWNTO);
		stepType=MP_DOWNTO;
		break;
	default:
		Syntax_Error();
		break;
	}
	return stepType;
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::FinalValue(SemanticRecord* &expressionRec)
{
	switch(lookahead)
	{
	case MP_PLUS:				// FinalValue -> OrdinalExpression		Rule# 60
	case MP_MINUS:				// See WriteParameter for explaination of cases
	case MP_INTEGER_LIT:
	case MP_UNSIGNEDINTEGER:
	case MP_IDENTIFIER:
	case MP_NOT:
	case MP_LPAREN:
	case MP_FLOAT_LIT:	
	case MP_FIXED_LIT:
	case MP_STRING:
	case MP_TRUE:
	case MP_FALSE:
		parseTree->LogExpansion(60);
		OrdinalExpression(expressionRec);
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
	case MP_SCOLON: // OptionalActualParameterList -> e		Rule # 63
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
	case MP_PLUS:				// ActualParameter -> OrdinalExpression 	Rule# 66
	case MP_MINUS:				// See WriteParameter for explaination of cases
	case MP_INTEGER_LIT:
	case MP_UNSIGNEDINTEGER:
	case MP_IDENTIFIER:
	case MP_NOT:
	case MP_LPAREN:
	case MP_FLOAT_LIT:	
	case MP_FIXED_LIT:
	case MP_STRING:
	case MP_TRUE:
	case MP_FALSE: 
		parseTree->LogExpansion(66);
		//OrdinalExpression(expressionRec);
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::Expression(SemanticRecord* &expressionRec)
{
	switch(lookahead)
	{
	case MP_PLUS:				// Expression -> SimpleExpression OptionalRelationalPart 	Rule# 67
	case MP_MINUS:				// See WriteParameter for explaination of cases
	case MP_INTEGER_LIT:
	case MP_UNSIGNEDINTEGER:
	case MP_IDENTIFIER:
	case MP_NOT:
	case MP_LPAREN:
	case MP_FLOAT_LIT:	
	case MP_FIXED_LIT:
	case MP_STRING:
	case MP_TRUE:
	case MP_FALSE: 
		parseTree->LogExpansion(67);
		SimpleExpression(expressionRec);
		OptionalRelationalPart(expressionRec);
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::OptionalRelationalPart(SemanticRecord* &expressionRec)
{
	SemanticRecord* rightExpressionRec = new SemanticRecord();
	switch(lookahead)
	{
	case MP_EQUAL:
		parseTree->LogExpansion(68);
		RelationalOperator();
		SimpleExpression(rightExpressionRec);
		if (expressionRec->getType()==MP_INTEGER_LIT )
			if (rightExpressionRec->getType()==MP_INTEGER_LIT)
				Gen_Assembly("CMPEQS");  
			else
				{
					Gen_Assembly("CASTI ;righthandside result is a float left hand side is an int so cast");
					Gen_Assembly("CMPEQS");
			}
		else
			if (rightExpressionRec->getType()==MP_FLOAT_LIT || rightExpressionRec->getType()==MP_FIXED_LIT)
				Gen_Assembly("CMPEQSF");  
			else
				{
					Gen_Assembly("CASTF ;righthandside result is an int left hand side is an float so cast");
					Gen_Assembly("CMPEQSF");
			}
		break;
	case MP_LTHAN:
		parseTree->LogExpansion(68);
		RelationalOperator();
		SimpleExpression(rightExpressionRec);
		if (expressionRec->getType()==MP_INTEGER_LIT )
			if (rightExpressionRec->getType()==MP_INTEGER_LIT)
				Gen_Assembly("CMPLTS");  
			else
				{
					Gen_Assembly("CASTI ;righthandside result is a float left hand side is an int so cast");
					Gen_Assembly("CMPLTS");
			}
		else
			if (rightExpressionRec->getType()==MP_FLOAT_LIT || rightExpressionRec->getType()==MP_FIXED_LIT)
				Gen_Assembly("CMPLTSF");  
			else
				{
					Gen_Assembly("CASTF ;righthandside result is an int left hand side is an float so cast");
					Gen_Assembly("CMPLTSF");
			}
		break;
	case MP_GTHAN:
		parseTree->LogExpansion(68);
		RelationalOperator();
		SimpleExpression(rightExpressionRec);
		if (expressionRec->getType()==MP_INTEGER_LIT )
			if (rightExpressionRec->getType()==MP_INTEGER_LIT)
				Gen_Assembly("CMPGTS");  
			else
				{
					Gen_Assembly("CASTI ;righthandside result is a float left hand side is an int so cast");
					Gen_Assembly("CMPGTS");
			}
		else
			if (rightExpressionRec->getType()==MP_FLOAT_LIT || rightExpressionRec->getType()==MP_FIXED_LIT)
				Gen_Assembly("CMPGTSF");  
			else
				{
					Gen_Assembly("CASTF ;righthandside result is an int left hand side is an float so cast");
					Gen_Assembly("CMPGTSF");
			}
		break;
	case MP_LEQUAL:
		parseTree->LogExpansion(68);
		RelationalOperator();
		SimpleExpression(rightExpressionRec);
		if (expressionRec->getType()==MP_INTEGER_LIT )
			if (rightExpressionRec->getType()==MP_INTEGER_LIT)
				Gen_Assembly("CMPLES");  
			else
				{
					Gen_Assembly("CASTI ;righthandside result is a float left hand side is an int so cast");
					Gen_Assembly("CMPLES");
			}
		else
			if (rightExpressionRec->getType()==MP_FLOAT_LIT || rightExpressionRec->getType()==MP_FIXED_LIT)
				Gen_Assembly("CMPLESF");  
			else
				{
					Gen_Assembly("CASTF ;righthandside result is an int left hand side is an float so cast");
					Gen_Assembly("CMPLESF");
			}
		break;
	case MP_GEQUAL:
		parseTree->LogExpansion(68);
		RelationalOperator();
		SimpleExpression(rightExpressionRec);
		if (expressionRec->getType()==MP_INTEGER_LIT )
			if (rightExpressionRec->getType()==MP_INTEGER_LIT)
				Gen_Assembly("CMPGES");  
			else
				{
					Gen_Assembly("CASTI ;righthandside result is a float left hand side is an int so cast");
					Gen_Assembly("CMPGES");
			}
		else
			if (rightExpressionRec->getType()==MP_FLOAT_LIT || rightExpressionRec->getType()==MP_FIXED_LIT)
				Gen_Assembly("CMPGESF");  
			else
				{
					Gen_Assembly("CASTF ;righthandside result is an int left hand side is an float so cast");
					Gen_Assembly("CMPGESF");
			}
	case MP_NEQUAL: // OptionalRelationalPart -> RelationalOperator SimpleExpression	Rule #68
		parseTree->LogExpansion(68);
		RelationalOperator();
		SimpleExpression(rightExpressionRec);
		if (expressionRec->getType()==MP_INTEGER_LIT )
			if (rightExpressionRec->getType()==MP_INTEGER_LIT)
				Gen_Assembly("CMPNES");  
			else
				{
					Gen_Assembly("CASTI ;righthandside result is a float left hand side is an int so cast");
					Gen_Assembly("CMPNES");
			}
		else
			if (rightExpressionRec->getType()==MP_FLOAT_LIT || rightExpressionRec->getType()==MP_FIXED_LIT)
				Gen_Assembly("CMPNESF");  
			else
				{
					Gen_Assembly("CASTF ;righthandside result is an int left hand side is an float so cast");
					Gen_Assembly("CMPNESF");
			}
		break;
	case MP_SCOLON:
	case MP_END:
	case MP_THEN:
	case MP_ELSE:
	case MP_RPAREN:
	case MP_TO:
	case MP_DOWNTO:
	case MP_DO: // OptionalRelationalPart -> e		Rule #69
	//case MP_COMMA: //added
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
void Parser::SimpleExpression(SemanticRecord* &expressionRec)
{
	string v = scanner->getLexeme();
	SemanticRecord* termRec = new SemanticRecord();
	SemanticRecord* termTailRec = new SemanticRecord();
	SemanticRecord* resultRec = new SemanticRecord();
	switch(lookahead)
	{
	case MP_PLUS:				// SimpleExpression -> Optional Sign Term TermTail	Rule# 76
	case MP_MINUS:				// See WriteParameter for explaination of cases
	case MP_INTEGER_LIT:
	case MP_UNSIGNEDINTEGER:
	case MP_IDENTIFIER:
	case MP_NOT:
	case MP_LPAREN:
	case MP_FLOAT_LIT:	
	case MP_FIXED_LIT:
	case MP_STRING:
	case MP_TRUE:
	case MP_FALSE: 
    
		parseTree->LogExpansion(76);
		OptionalSign();
		Term(termRec);

		termTailRec=termRec;
		TermTail(termTailRec);
		expressionRec=termTailRec;
		break;		
	default: //everything else
		Syntax_Error();
		break;
	}

	
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::TermTail(SemanticRecord* &termTailRec)
{
	SemanticRecord* termRec = new SemanticRecord();
	SemanticRecord* resultRec = new SemanticRecord();
	switch(lookahead)
	{	
	case MP_OR:
		parseTree->LogExpansion(77);
		AddingOperator();
		Term(termRec);
		TermTail(termTailRec);
		Gen_Assembly("ORS");
		break;
	case MP_PLUS:
		parseTree->LogExpansion(77);
		AddingOperator();
		Term(termRec);

		
		// check the -1(SP) and -2(SP) to see if they are the same types
		if (termTailRec->getType() == MP_INTEGER_LIT)
		{
			if (termRec->getType() == MP_INTEGER_LIT)
			{
				// same types, just push and adds
				Gen_Assembly("ADDS");
				termTailRec->setType(MP_INTEGER_LIT);
			} 
			else if (termRec->getType() == MP_FIXED_LIT || termRec->getType() == MP_FLOAT_LIT)
			{
				// now we have to push the -2(SP) to top of stack, cast it, then push back to -2(SP)
				Gen_Assembly("PUSH -2(SP)	; Casting from int to float");
				Gen_Assembly("CASTSF");
				Gen_Assembly("POP -2(SP)	; Cast done");
				Gen_Assembly("ADDSF");
				termTailRec->setType(MP_FLOAT_LIT);
			}
		}
		else if (termTailRec->getType() == MP_FIXED_LIT || termTailRec->getType() == MP_FLOAT_LIT)
		{
			if (termRec->getType() == MP_FIXED_LIT || termRec->getType() == MP_FLOAT_LIT)
			{
				// same types
				Gen_Assembly("ADDSF");
				termTailRec->setType(MP_FLOAT_LIT);
			} 
			else if (termRec->getType() == MP_INTEGER_LIT)
			{
				// now we have to push the -2(SP) to top of stack, cast it, then push back to -2(SP)
				Gen_Assembly("PUSH -2(SP)	; Casting from float to int");
				Gen_Assembly("CASTSI");
				Gen_Assembly("POP -2(SP)	; Cast done");
				Gen_Assembly("ADDS");
				termTailRec->setType(MP_INTEGER_LIT);
			}
		}

		TermTail(termTailRec);
		break;
	case MP_MINUS: {// TermTail -> AddingOperator Term TermTail  	Rule# 77
		parseTree->LogExpansion(77);
		AddingOperator();
		Term(termRec);

		// check the -1(SP) and -2(SP) to see if they are the same types
		if (termTailRec->getType() == MP_INTEGER_LIT)
		{
			if (termRec->getType() == MP_INTEGER_LIT)
			{
				// same types, just push and adds
				Gen_Assembly("SUBS");
				termTailRec->setType(MP_INTEGER_LIT);
			} 
			else if (termRec->getType() == MP_FIXED_LIT || termRec->getType() == MP_FLOAT_LIT)
			{
				// now we have to push the -2(SP) to top of stack, cast it, then push back to -2(SP)
				Gen_Assembly("PUSH -2(SP)	; Casting from int to float");
				Gen_Assembly("CASTSF");
				Gen_Assembly("POP -2(SP)	; Cast done");
				Gen_Assembly("SUBSF			;1593");
				termTailRec->setType(MP_FLOAT_LIT);
			}
		}
		else if (termTailRec->getType() == MP_FIXED_LIT || termTailRec->getType() == MP_FLOAT_LIT)
		{
			if (termRec->getType() == MP_FIXED_LIT || termRec->getType() == MP_FLOAT_LIT)
			{
				// same types
				Gen_Assembly("SUBSF			;1603");
				termTailRec->setType(MP_FLOAT_LIT);
			} 
			else if (termRec->getType() == MP_INTEGER_LIT)
			{
				// now we have to push the -2(SP) to top of stack, cast it, then push back to -2(SP)
				Gen_Assembly("PUSH -2(SP)	; Casting from int to float");
				Gen_Assembly("CASTSI");
				Gen_Assembly("POP -2(SP)	; Cast done");
				Gen_Assembly("SUBS");
				termTailRec->setType(MP_INTEGER_LIT);
			}
		}

		TermTail(termTailRec);
		break;
	}
	case MP_RPAREN: // TermTail -> {e} 		Rule# 78
	case MP_END:
	case MP_SCOLON: 
	case MP_EQUAL:
	case MP_LTHAN:
	case MP_GTHAN:
	case MP_GEQUAL:
	case MP_LEQUAL:
	case MP_DO:
	case MP_THEN:
	//case MP_LPAREN:
	case MP_TO:  //Do not comment required for for loops
	case MP_DOWNTO:
	case MP_ELSE:
	case MP_MOD:
	//case MP_COMMA: //added
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
		negativeFlag=true;
		break;
	case MP_IDENTIFIER:
	case MP_NOT:
	case MP_UNSIGNEDINTEGER:
	case MP_INTEGER_LIT:
	case MP_FIXED_LIT:
	case MP_FLOAT_LIT:
	//case MP_RPAREN:
	case MP_LPAREN: // OptionalSign -> {e}	Rule #81
	case MP_TRUE:
	case MP_FALSE:
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
void Parser::Term(SemanticRecord* &termRec)
{
	//SemanticRecord* termRec = new SemanticRecord();

	switch(lookahead)
	{
	case MP_UNSIGNEDINTEGER:
	case MP_INTEGER_LIT:
	case MP_FIXED_LIT:
	case MP_FLOAT_LIT:
	case MP_TRUE:
	case MP_FALSE:
	case MP_NOT:
	case MP_IDENTIFIER: // Term -> Factor FactorTail  	Rule# 85
	case MP_STRING: //added
	case MP_LPAREN:
		parseTree->LogExpansion(85);
		Factor(termRec);
		FactorTail(termRec);
		//TermTail();		// DEBUG - this is not in the grammar
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::FactorTail(SemanticRecord* &termTailRec)
{
	SemanticRecord* termRec = new SemanticRecord();

	switch(lookahead)
	{
	case MP_DIVF:
		parseTree->LogExpansion(86);
		MultiplyingOperator();
		Factor(termRec);
		if (termTailRec->getType() == MP_INTEGER_LIT)
		{
			if (termRec->getType() == MP_FLOAT_LIT || termRec->getType() == MP_FIXED_LIT)
				{
					// now we have to push the -2(SP) to top of stack, cast it, then push back to -2(SP)
					Gen_Assembly("PUSH -2(SP)	; Casting from int to float");
					Gen_Assembly("CASTSF");
					Gen_Assembly("POP -2(SP)");
					Gen_Assembly("DIVSF");
					termTailRec->setType(MP_FLOAT_LIT);
				}
		}
		else if (termTailRec->getType() == MP_FLOAT_LIT || termTailRec->getType() == MP_FIXED_LIT)
		{
			if (termRec->getType() == MP_FLOAT_LIT || termRec->getType() == MP_FIXED_LIT)
			{
				Gen_Assembly("DIVSF");
				termTailRec->setType(MP_FLOAT_LIT);
			}
			else if (termRec->getType() == MP_INTEGER_LIT)
			{
				// now we have to push -2(D0) and cast, then move back to -2(SP)
				Gen_Assembly("PUSH -1(SP)	;Float divison  Casting from int to float");
				Gen_Assembly("CASTSF");
				Gen_Assembly("POP -1(SP)");
				Gen_Assembly("DIVSF");
				termTailRec->setType(MP_FLOAT_LIT);
			}
		} 
		FactorTail(termRec);
		break;
	case MP_DIV:
		parseTree->LogExpansion(86);
		MultiplyingOperator();
		Factor(termRec);
		
		// check previous and check next to see if we need to cast
		// rec is previous record, r, is the current record
		if (termTailRec->getType() == MP_INTEGER_LIT) //Need an error here if trying to use a float
		{
			if (termRec->getType() == MP_INTEGER_LIT)
			{
				Gen_Assembly("DIVS");
				termTailRec->setType(MP_INTEGER_LIT);
			}
			else if (termRec->getType() == MP_FLOAT_LIT || termRec->getType() == MP_FIXED_LIT)
				{
					// now we have to push the -2(SP) to top of stack, cast it, then push back to -2(SP)
					Gen_Assembly("PUSH -1(SP)	; DIV cast float to int");
					Gen_Assembly("CASTSI");
					Gen_Assembly("POP -1(SP)");
					Gen_Assembly("DIVS");
					termTailRec->setType(MP_INTEGER_LIT);
				}
				
		}
		else if (termTailRec->getType() == MP_FLOAT_LIT || termTailRec->getType() == MP_FIXED_LIT)
		{
			if (termRec->getType() == MP_FLOAT_LIT || termRec->getType() == MP_FIXED_LIT)
			{
				Gen_Assembly("PUSH -1(SP)	; DIV cast float to int");
				Gen_Assembly("CASTSI");
				Gen_Assembly("POP -1(SP)");
				Gen_Assembly("PUSH -2(SP)	; DIV cast float to int");
				Gen_Assembly("CASTSI");
				Gen_Assembly("POP -2(SP)");
				Gen_Assembly("DIVS");
				termTailRec->setType(MP_INTEGER_LIT);
			}
			else 
			{
				Gen_Assembly("PUSH -2(SP)	; DIV cast float to int");
				Gen_Assembly("CASTSI");
				Gen_Assembly("POP -2(SP)");
				Gen_Assembly("DIVS");
				termTailRec->setType(MP_INTEGER_LIT);
			}
		} 
		
		FactorTail(termRec);
		break;
	case MP_TIMES:
		parseTree->LogExpansion(86);
		MultiplyingOperator();

		Factor(termRec);

		// check previous and check next to see if we need to cast
		// rec is previous record, r, is the current record
		if (termTailRec->getType() == MP_INTEGER_LIT)
		{
			if (termRec->getType() == MP_INTEGER_LIT)
			{
				Gen_Assembly("MULS");
			}
			else if (termRec->getType() == MP_FLOAT_LIT || termRec->getType() == MP_FIXED_LIT)
			{
				// now we have to push the -2(SP) to top of stack, cast it, then push back to -2(SP)
				Gen_Assembly("PUSH -2(SP)	; Casting from int to float");
				Gen_Assembly("CASTSF");
				Gen_Assembly("POP -2(SP)");
				Gen_Assembly("MULSF");
				termTailRec->setType(MP_FLOAT_LIT);
			}
				
		}
		else if (termTailRec->getType() == MP_FLOAT_LIT || termTailRec->getType() == MP_FIXED_LIT)
		{
			if (termRec->getType() == MP_FLOAT_LIT || termRec->getType() == MP_FIXED_LIT)
			{
				Gen_Assembly("MULSF");
			}
			else if (termRec->getType() == MP_INTEGER_LIT)
			{
				// now we have to push -2(D0) and cast, then move back to -2(SP)
				Gen_Assembly("PUSH -2(SP)	; Casting from float to int");
				Gen_Assembly("CASTSI");
				Gen_Assembly("POP -2(SP)");
				Gen_Assembly("MULS");
				termTailRec->setType(MP_INTEGER_LIT);
			}
		} 

		FactorTail(termRec);
		break;
	case MP_MOD:
		parseTree->LogExpansion(86);
		MultiplyingOperator();
		Factor(termRec);

		// check previous and check next to see if we need to cast
		// rec is previous record, r, is the current record
		if (termTailRec->getType() == MP_INTEGER_LIT)
		{
			if (termRec->getType() == MP_INTEGER_LIT)
			{
				Gen_Assembly("MODS");
				termTailRec->setType(MP_INTEGER_LIT);
			}
			else if (termRec->getType() == MP_FLOAT_LIT || termRec->getType() == MP_FIXED_LIT) //I don't think this should be allowed if I am reading the grammer right
			{
				
				Gen_Assembly("PUSH -1(SP)	; MOD only on ints float to int");
				Gen_Assembly("CASTSI");
				Gen_Assembly("POP -1(SP)");
				Gen_Assembly("MODS");
				termTailRec->setType(MP_INTEGER_LIT);
			}
				
		}

		else if (termTailRec->getType() == MP_FLOAT_LIT || termTailRec->getType() == MP_FIXED_LIT)
		{
			if (termRec->getType() == MP_INTEGER_LIT|| termTailRec->getType() == MP_FIXED_LIT)
			{
				Gen_Assembly("PUSH -1(SP)	; MOD only on ints float to int");
				Gen_Assembly("CASTSI");
				Gen_Assembly("POP -1(SP)");
				Gen_Assembly("PUSH -2(SP)	; MOD only on ints float to int");
				Gen_Assembly("CASTSI");
				Gen_Assembly("POP -2(SP)");
				Gen_Assembly("MODS");
				termTailRec->setType(MP_INTEGER_LIT);
			}
			else  
			{
				Gen_Assembly("PUSH -2(SP)	; MOD only on ints float to int");
				Gen_Assembly("CASTSI");
				Gen_Assembly("POP -2(SP)");
				Gen_Assembly("MODS");
				termTailRec->setType(MP_INTEGER_LIT);
			} 
		
			
		}
				
		FactorTail(termRec);
		break;
	case MP_AND: // FactorTail -> MultiplyingOperator Factor FactorTail  	Rule# 86
		parseTree->LogExpansion(86);
		MultiplyingOperator();
		Factor(termRec);
		
		Gen_Assembly("ANDS");

		//if (caller->getType()==MP_INTEGER_LIT) // Need to look at this block not sure you can even and a float?
		//{
		//		Gen_Assembly("ANDS");
		//}
		//else if (caller->getType()==MP_FLOAT_LIT || caller->getType() == MP_FIXED_LIT)
		//{
		//		Gen_Assembly("ANDS");
		//}
		
		FactorTail(termRec);
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
	case MP_TO:  // Must include here do not comment
	case MP_DOWNTO:
	case MP_NEQUAL: // FactorTail -> {e}	Rule# 87
	//case MP_STRING: //added
	//case MP_COMMA: //added
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
	case MP_DIVF: // MultiplyingOperator -> "/"  	Rule# 112
		parseTree->LogExpansion(112);
		Match(MP_DIVF);
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::Factor(SemanticRecord* &termTailRec)
{
	switch(lookahead)
	{
	case MP_UNSIGNEDINTEGER: // Factor -> UnsignedInteger  	Rule# 92
		parseTree->LogExpansion(92);
		Match(MP_UNSIGNEDINTEGER);
		break;
		//////////////////////// Conflict 96, 99
	case MP_INTEGER_LIT: // Factor -> UnsignedInteger  	Rule# 95		// DEBUG - conflict
		parseTree->LogExpansion(95);
		termTailRec->setType(MP_INTEGER_LIT);
		Gen_Assembly("PUSH #" + scanner->getLexeme());
		if (negativeFlag==true) {Gen_Assembly("NEG -1(S)P -1(SP) ; optional sign negative");negativeFlag=false;}

		Match(MP_INTEGER_LIT);
		break;
	case MP_IDENTIFIER:	{// Factor -> VariableIdentifier		Rule # 93
		// lookup factor in symbol table
		SymbolTable::Record* tempRecord = symbolTable->lookupRecord(scanner->getLexeme(),SymbolTable::KIND_VARIABLE,0);
		parseTree->LogExpansion(96);
	
		// set type of record to the factor type
		termTailRec->setType(tempRecord->token);
		Gen_Assembly("PUSH " + to_string(tempRecord->offset) + "(D0)	; " + tempRecord->name);

		VariableIdentifier();
		break;
	}
	case MP_NOT: // "not" Factor  	Rule# 94
		parseTree->LogExpansion(94);
		Match(MP_NOT);
		Factor(termTailRec);
		break;
	
	case MP_FIXED_LIT: // Factor -> FIXED_LIT  	Rule# 95		// DEBUG - conflict
		parseTree->LogExpansion(95);


		termTailRec->setType(MP_FIXED_LIT);
		Gen_Assembly("PUSH #" + scanner->getLexeme());
		if (negativeFlag==true) {Gen_Assembly("NEGF -1(S)P -1(SP) ; optional sign negative");negativeFlag=false;}

		Match(MP_FIXED_LIT);
		break;	
	case MP_FLOAT_LIT:
		parseTree->LogExpansion(95);


		termTailRec->setType(MP_FLOAT_LIT);
		Gen_Assembly("PUSH #" + scanner->getLexeme());
		if (negativeFlag==true) {Gen_Assembly("NEGF -1(S)P -1(SP) ; optional sign negative");negativeFlag=false;}

		Match(MP_FLOAT_LIT);
		break;
	case MP_LPAREN: // Factor -> "(" Expression ")"  	Rule# 95
		parseTree->LogExpansion(95);
		Match(MP_LPAREN);
		Expression(termTailRec);
		Match(MP_RPAREN);
		break;
		//////////////////////// Conflict 96, 99
	case MP_STRING: // Factor -> String Literal Rule # 114
		parseTree->LogExpansion(114);
		Match(MP_STRING);
		break;
	case MP_TRUE: // Factor -> "true" Rule # 115
		parseTree->LogExpansion(115);
		Match(MP_TRUE);
		break;
	case MP_FALSE: // Factor -> "false" Rule # 116
		parseTree->LogExpansion(116);
		Match(MP_FALSE);
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
	string v = scanner->getLexeme();

	switch(lookahead)
	{
	case MP_IDENTIFIER: // VariableIdentifier -> Identifier  	Rule# 98
		parseTree->LogExpansion(98);

		// We need some more information about how we got to this point 

		if (caller->getKind()==SemanticRecord::DECLARATION) // We are declaring variables so add them to the symbol table
		{
			symbolTable->insertRecord(scanner->getLexeme(), SymbolTable::KIND_VARIABLE, scanner->token()/*, scanner->getLineNumber(), scanner->getColumnNumber()*/);
		}
		else if (caller->getKind()==SemanticRecord::ASSIGNMENT) // We are at the start of an assignment statement get the variables type and add it to the caller semantic record
		{
			SymbolTable::Record* rec=symbolTable->lookupRecord(v, SymbolTable::KIND_VARIABLE, 0);
			
			caller->setType(rec->token);
		}
		// This is only being set up for control statements so far
		else if (caller->getKind()==SemanticRecord::SIMPLE_PARAMETER) // variable in control statement
		{
			SymbolTable::Record* rec=symbolTable->lookupRecord(v, SymbolTable::KIND_VARIABLE, 0);
			
			caller->setType(rec->token);
		}

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
		symbolTable->insertRecord(scanner->getLexeme(), SymbolTable::KIND_FUNCTION, scanner->token()/*, scanner->getLineNumber(), scanner->getColumnNumber()*/);
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
	SemanticRecord* expressionRec = new SemanticRecord();
	switch(lookahead)
	{
	case MP_PLUS:				// BooleanExpression -> Expression Expression (-> SimpleExpression OptionalRelationalPart)	Rule# 101
	case MP_MINUS:				// See WriteParameter for explaination of cases
	case MP_INTEGER_LIT:
	case MP_UNSIGNEDINTEGER:
	case MP_IDENTIFIER:
	case MP_NOT:
	case MP_LPAREN:
	case MP_FLOAT_LIT:	
	case MP_FIXED_LIT:
	case MP_STRING:
	case MP_TRUE:
	case MP_FALSE:  
		parseTree->LogExpansion(101);
		Expression(expressionRec);
		break;
	default: //everything else
		Syntax_Error();
		break;
	}
}

// precondition: (lookahead is a valid token)
// postcondition: (method applies rules correctly)
void Parser::OrdinalExpression(SemanticRecord* &expressionRec)
{
	switch(lookahead)
	{
	case MP_PLUS:				// OrdinalExpression -> Expression Expression (-> SimpleExpression OptionalRelationalPart)	Rule# 102
	case MP_MINUS:				// See WriteParameter for explaination of cases
	case MP_INTEGER_LIT:
	case MP_UNSIGNEDINTEGER:
	case MP_IDENTIFIER:
	case MP_NOT:
	case MP_LPAREN:
	case MP_FLOAT_LIT:	
	case MP_FIXED_LIT:
	case MP_STRING:
	case MP_TRUE:
	case MP_FALSE: 
		parseTree->LogExpansion(102);
		Expression(expressionRec);
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

		// add new record into symbol table
		symbolTable->insertRecord(scanner->getLexeme(), SymbolTable::KIND_VARIABLE, scanner->token()/*, scanner->getLineNumber(), scanner->getColumnNumber()*/);
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

		// add new record into symbol table
		symbolTable->insertRecord(scanner->getLexeme(), SymbolTable::KIND_VARIABLE, scanner->token()/*, scanner->getLineNumber(), scanner->getColumnNumber()*/);
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
		return MP_INTEGER_LIT;
	case MP_FLOAT_LIT: //Type -> "Float", rule #108
		parseTree->LogExpansion(108);
		Match(MP_FLOAT_LIT);
		return MP_FLOAT_LIT;
	case MP_FIXED_LIT: //Type -> "Float", rule #108
		parseTree->LogExpansion(108);
		Match(MP_FIXED_LIT);
		return MP_FIXED_LIT;
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
		parseTree->LogMessage("      Matched: " + EnumToString(token) + " = " + scanner->getLexeme());
		currentLexeme = scanner->getLexeme();
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
	msg.append("\nFile: " + fileName + ": \nSyntax error found on line " + to_string(scanner->getLineNumber()) + ", column " + to_string(scanner->getColumnNumber()) + 
		".\n    Expected " + EnumToString(expected) + " but found " + EnumToString(lookahead) + ".\n    Next token: " + EnumToString(scanner->getToken())
		+ "\n    Next Lexeme: " + scanner->getLexeme());
	cout << msg;
	parseTree->LogMessage(msg);
	throw -1;
}

void Parser::Gen_Assembly(string s)
{
	irFile << s << endl;
	//printf(s.c_str());
	//printf("\n");
}

string Parser::LabelMaker()
{
	string newLabel;
	labelCount++;
	char tempBuffer [33];
	itoa(labelCount-1, tempBuffer,10);
	newLabel= "L";
	newLabel.append(tempBuffer) ;
	
	return (newLabel);

}