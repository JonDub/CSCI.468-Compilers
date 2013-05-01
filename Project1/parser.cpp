#include "Parser.h"

Parser::Parser(void)
{
}

Parser::Parser(string fName)
{
	// Create our scanner and parse trees
	symbolTable = new SymbolTable();
	scanner = new Scanner();
	parseTree = new ParseTree("parse_tree.txt");
	parseTree->ReadCFGRules("CFG_rules.txt");
	setInputFile(fName);
	caller = new SemanticRecord();
	labelCount=0;

	// Open the ir file
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
	delete parseTree;
	delete symbolTable;
}

bool Parser::parse()
{
	try
	{
		// Start the parser. Set the first lookahead
		lookahead = scanner->getToken();		
		return SystemGoal();
	} 
	catch (int ex)
	{
		return false;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
bool Parser::SystemGoal()
{
	switch(lookahead)
	{
	// SystemGoal --> Program eof, rule #1    
	case MP_PROGRAM:  
		parseTree->LogExpansion(1);

		// Generate begin assembly
		Gen_Assembly("MOV SP D0\n");
		Program();
		Match(MP_EOF);

		// Generate end assembly
		Gen_Assembly("\nMOV D0 SP");
		Gen_Assembly("HLT");
		return true;

	default: // Everything else
		Syntax_Error();
		return false;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::Program()
{
	symbolTable->createTable(); // Table for main

	switch(lookahead)
	{
	// Program --> ProgramHeading ";" Block ".", rule #2 
	case MP_PROGRAM: 
		parseTree->LogExpansion(2);
		ProgramHeading();
		Match(MP_SCOLON);
		Block();
		Match(MP_PERIOD);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::ProgramHeading()
{
	switch(lookahead)
	{
	// ProgramHeading  --> "program" ProgramIdentifier, rule #3 
	case MP_PROGRAM: 
		parseTree->LogExpansion(3);
		Match(MP_PROGRAM);
		caller->setKind(SemanticRecord::PROGRAM);
		ProgramIdentifier();
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::Block()
{
	switch (lookahead)
	{
	// Block -> VariableDeclarationPart ProcedureAndFunctionDeclarationPart StatementPart, rule 4 
	case MP_VAR: 
		parseTree->LogExpansion(4);
		VariableDeclarationPart();
		ProcedureAndFunctionDeclarationPart();
		StatementPart();
		break;
	// Block -> ProcedureAndFunctionDeclarationPart when VariableDeclarationPart -> e
	case MP_PROCEDURE:
	case MP_FUNCTION: 
	// Block -> StatementPart when 
	// ProcedureAndFunctionDeclarationPart & VariableDeclarationPart -> e
	case MP_BEGIN: 
		ProcedureAndFunctionDeclarationPart();
		StatementPart();
		break;		

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::VariableDeclarationPart()
{
	switch(lookahead)
	{
	// VariableDeclarationPart -> "var" VariableDeclaration ";" VariableDeclarationTail, rule #5
	case MP_VAR: 
		parseTree->LogExpansion(5);
		Match(MP_VAR);		
		VariableDeclaration();
		Match(MP_SCOLON);
		VariableDeclarationTail();
		break;	
	// VariableDeclarationPart -> e, rule# 106	
	case MP_PROCEDURE:	
	case MP_FUNCTION:	
	case MP_BEGIN:		
		parseTree->LogExpansion(106);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::VariableDeclarationTail()
{
	switch(lookahead)
	{
	// VariableDeclarationTail  --> VariableDeclaration ";" VariableDeclarationTail, rule #6
	case MP_IDENTIFIER: 
		parseTree->LogExpansion(6);
		VariableDeclaration();
		Match(MP_SCOLON);
		VariableDeclarationTail();
		break;
	// VariableDeclarationTail -> e, rule #7
	case MP_PROCEDURE:
	case MP_BEGIN:
	case MP_FUNCTION: 
		parseTree->LogExpansion(7);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::VariableDeclaration()
{
	int start = symbolTable->tableSize();
	Token type = MP_NULL;
	caller->setKind(SemanticRecord::DECLARATION);

	switch(lookahead)
	{
	// VariableDeclaration -> Identifierlist ":" Type , rule #8
	case MP_IDENTIFIER: 
		parseTree->LogExpansion(8);
		IdentifierList();
		Match(MP_COLON);
		type = Type();
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}

	// Update records in symbol table with data type
	int end = symbolTable->tableSize();
	int diff = symbolTable->tableSize() - start;
	for (int i = start; i < end; i++)
	{
		SymbolTable::Record* rec = symbolTable->lookupRecord(i);
		if (rec != NULL)
			rec->token = type; 
	}

	// Add differencing offset for variable
	Gen_Assembly("ADD SP #" + to_string(diff) + " SP	; space for " + to_string(diff) + " variables of type " + EnumToString(type));
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::ProcedureAndFunctionDeclarationPart()
{
	switch(lookahead)
	{
	// ProcedureAndFunctionDeclarationPart -> ProcedureDeclaration 
	// ProcedureAndFunctionDeclarationPart, rule #9 
	case MP_PROCEDURE: 
		parseTree->LogExpansion(9);
		ProcedureDeclaration();
		ProcedureAndFunctionDeclarationPart();
		break;
	// ProcedureAndFunctionDeclarationPart -> FunctionDeclaration 
	// ProcedureAndFunctionDeclarationPart, rule #10
	case MP_FUNCTION: 
		parseTree->LogExpansion(10);
		FunctionDeclaration();
		ProcedureAndFunctionDeclarationPart();
		break;
	// ProcedureAndFunctionDeclarationPart -> e, rule #11
	case MP_BEGIN: 
		parseTree->LogExpansion(11);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}	
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::ProcedureDeclaration()
{
	switch(lookahead)
	{
	// ProcedureHeading ";" Block ";", rule #12
	case MP_PROCEDURE: 
		parseTree->LogExpansion(12);
		ProcedureHeading();
		Match(MP_SCOLON);
		Block();		
		Match(MP_SCOLON);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::FunctionDeclaration()
{
	switch(lookahead)
	{
	// FunctionDeclaration -> FunctionHeading ";" Block ";", rule #13
	case MP_FUNCTION: 
		parseTree->LogExpansion(13);
		FunctionHeading();
		Match(MP_SCOLON);
		Block();
		Match(MP_SCOLON);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::ProcedureHeading()
{
	switch(lookahead)
	{
	// ProcedureHeading -> "procedure" procedureIdentifier OptionalFormalParameterList, rule #16
	case MP_PROCEDURE: 
		parseTree->LogExpansion(16);
		Match(MP_PROCEDURE);
		ProcedureIdentifier();
		OptionalFormalParameterList();
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::FunctionHeading()
{
	Token type;
	int offset;

	switch(lookahead)
	{
	// FunctionHeading -> "function" functionIdentifier OptionalFormalParameterList ":" 
	// Type rule #15
	case MP_FUNCTION: 
		parseTree->LogExpansion(15);
		Match(MP_FUNCTION);

		// Get offset where the last function was inserted
		offset = symbolTable->tableSize(0);
		FunctionIdentifier();
		OptionalFormalParameterList();
		Match(MP_RETURN);
		type = Type();
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::OptionalFormalParameterList()
{
	switch(lookahead)
	{
	// OptionalFormalParameterList -> "(" FormalParameterSection FormalParameterSectionTail ")", 
	// rule #16
	case MP_LPAREN: 
		parseTree->LogExpansion(16);
		Match(MP_LPAREN);
		FormalParameterSection();
		FormalParameterSectionTail();
		Match(MP_RPAREN);
		break;
	// OptionalFormalParameterList -> e, rule #17
	case MP_INTEGER_LIT:
	case MP_FLOAT_LIT:
	case MP_FIXED_LIT:
	case MP_REAL:
	case MP_STRING:
	case MP_BOOLEAN:
	case MP_SCOLON: 
		parseTree->LogExpansion(17);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::FormalParameterSectionTail()
{
	switch(lookahead)
	{
	// FormalParameterSectionTail -> ";" FormalParameterSection FormalParameterSectionTail, 
	// rule #18
	case MP_SCOLON: 
		parseTree->LogExpansion(18);
		Match(MP_SCOLON);
		FormalParameterSection();
		FormalParameterSectionTail();
		break;
	// FormalParameterSectionTail -> e, rule #19
	case MP_RPAREN: 
		parseTree->LogExpansion(19);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::FormalParameterSection()
{
	switch(lookahead)
	{
	// FormalParameterSection -> ValueParameterSection, rule #20
	case MP_IDENTIFIER: 
		parseTree->LogExpansion(20);
		ValueParameterSection();
		break;
	// FormalParameterSection -> VariableParameterSection, rule #21
	case MP_VAR: 
		parseTree->LogExpansion(21);
		VariableParameterSection();
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::ValueParameterSection()
{
	Token type;
	int start = symbolTable->tableSize();

	switch(lookahead)
	{
	// ValueParameterSection -> IdentifierList ":" Type, rule #22
	case MP_IDENTIFIER:
		parseTree->LogExpansion(22);
		IdentifierList();
		Match(MP_COLON);
		type = Type();
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::VariableParameterSection()
{
	Token type; 

	switch(lookahead)
	{
	// VariableParameterSection -> "var" IdentifierList ":" Type, rule #23
	case MP_VAR:
		parseTree->LogExpansion(23);
		Match(MP_VAR);
		IdentifierList();
		Match(MP_COLON);
		type = Type();
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::StatementPart()
{
	switch(lookahead)
	{
	//StatementPart -> CompoundStatement, rule #24
	case MP_BEGIN: 
		parseTree->LogExpansion(24);
		CompoundStatement();
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::CompoundStatement()
{
	switch(lookahead)
	{
	//CompoundStatement -> "begin" StatementSequence "end", rule #25
	case MP_BEGIN: 
		parseTree->LogExpansion(25);
		Match(MP_BEGIN);
		StatementSequence();
		Match(MP_END);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::StatementSequence()
{

	switch(lookahead)
	{
	//StatementSequence -> Statement StatementTail, rule #26
	case MP_END:
	case MP_BEGIN:
	case MP_READ: 
	case MP_WRITE: 
	case MP_WRITELN:
	case MP_IF: 
	case MP_REPEAT:
	case MP_IDENTIFIER: 
	case MP_FOR: 
	case MP_WHILE: 
		parseTree->LogExpansion(26);
		Statement();
		StatementTail();
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::StatementTail()
{
	switch(lookahead)
	{
	// StatementTail -> ";" Statement StatementTail , rule #27
	case MP_SCOLON:
		parseTree->LogExpansion(27);
		Match(MP_SCOLON);
		Statement();
		StatementTail();
		break;
	// StatementTail -> e , rule #28
	case MP_END:
	case MP_UNTIL:
		parseTree->LogExpansion(28);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::Statement()
{
	SemanticRecord* expressionRec = new SemanticRecord();

	switch(lookahead)
	{	
	// Statement -> EmptyStatement, rule #29
	case MP_END: 
	case MP_SCOLON:
	case MP_UNTIL: // For Repeat ... Until
		parseTree->LogExpansion(29);
		EmptyStatement();
		break;
	// Statement -> CompoundStatement, rule #30
	case MP_BEGIN: 
		parseTree->LogExpansion(30);
		CompoundStatement();
		break;
	// Statement -> ReadStatement, rule #31
	case MP_READ: 
		parseTree->LogExpansion(31);
		ReadStatement();
		break;
	// Statement -> WriteStatement, rule #32
	case MP_WRITE: 
	case MP_WRITELN:
		parseTree->LogExpansion(32);
		WriteStatement(expressionRec);
		break;
	// Statement -> AssignmentStatement, rule #33, unless it should be a procedure or function
	case MP_IDENTIFIER: 
		parseTree->LogExpansion(33);
		AssignmentStatement(expressionRec);
		break;
	// Statement -> IfStatement, rule #34
	case MP_IF: 
		parseTree->LogExpansion(34);
		IfStatement(expressionRec);
		break;
	// Statement -> WhileStatement, rule #35
	case MP_WHILE: 
		parseTree->LogExpansion(35);
		WhileStatement();
		break;
	// Statement -> RepeatStatement, rule #36
	case MP_REPEAT: 
		parseTree->LogExpansion(36);
		RepeatStatement();
		break;
	// Statement -> ForStatement, rule #37
	case MP_FOR: 
		parseTree->LogExpansion(37);
		ForStatement(expressionRec);
		break;
	case MP_ELSE:
		OptionalElsePart();
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::EmptyStatement()
{
	switch(lookahead)
	{
	// EmptyStatement -> e Rule #38 
	case MP_SCOLON:
	case MP_END:  
	case MP_UNTIL:	// For Repeat... Until
		parseTree->LogExpansion(38);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::ReadStatement()
{
	switch(lookahead)
	{
	// ReadStatement -> "read" "(" ReadParameter ReadParameterTail ")"    Rule #40
	case MP_READ:  {
		parseTree->LogExpansion(40);
		Match(MP_READ);
		Match(MP_LPAREN);

		// Generate read assembly based on the variables data type
		SymbolTable::Record* tempRecord = symbolTable->lookupRecord(scanner->getLexeme(), 
			SymbolTable::KIND_VARIABLE, 0);

		// Read float/fixed variable
		if (tempRecord->token == MP_FLOAT_LIT || tempRecord->token == MP_FIXED_LIT) 
		{
			Gen_Assembly("RDF D9		; Read Var " + tempRecord->name);
			Gen_Assembly("MOV D9 " + to_string(tempRecord->offset) + "(D0)");
		} 
		else if (tempRecord->token == MP_STRING)	// Read string variable
		{
			Gen_Assembly("RDS D9		; Read Var " + tempRecord->name);
			Gen_Assembly("MOV D9 " + to_string(tempRecord->offset) + "(D0)");
		}
		else  if (tempRecord->token == MP_INTEGER_LIT)	// Read integer variable
		{
			Gen_Assembly("RD D9			; Read Var " + tempRecord->name);
			Gen_Assembly("MOV D9 " + to_string(tempRecord->offset) + "(D0)");
		}

		ReadParameter(); 
		ReadParameterTail(); 
		Match(MP_RPAREN);
		break;
				   }
	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::ReadParameterTail()
{
	switch(lookahead)
	{
	case MP_COMMA: {  // ReadParameterTail -> "," ReadParameter ReadParameterTail, rule# 42
		parseTree->LogExpansion(42);
		Match(MP_COMMA);

		// Generate read assembly based on data type
		SymbolTable::Record* tempRecord = symbolTable->lookupRecord(scanner->getLexeme(), 
			SymbolTable::KIND_VARIABLE, 0);
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
	// ReadParameterTail -> e , rule# 42
	case MP_RPAREN:	
		parseTree->LogExpansion(42);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::ReadParameter()
{
	switch(lookahead)
	{
	// ReadParameter -> VariableIdentifier, rule# 43
	case MP_IDENTIFIER: 
		parseTree->LogExpansion(43);
		Match(MP_IDENTIFIER);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::WriteStatement(SemanticRecord* expressionRec)
{
	string v = scanner->getLexeme();

	switch(lookahead)
	{
	// WriteStatement -> "writeln" "(" WriteParameter WriteParameterTail ")", rule# 111
	case MP_WRITELN:	
		parseTree->LogExpansion(44);
		caller->setType(MP_WRITELN);
		Match(MP_WRITELN);
		Match(MP_LPAREN);
		WriteParameter(expressionRec);		
		WriteParameterTail(expressionRec);
		Match(MP_RPAREN);
		Gen_Assembly("WRTLN #\"\"");
		break;
	// WriteStatement -> "write" "(" WriteParameter WriteParameterTail ")", rule# 44
	case MP_WRITE: 
		parseTree->LogExpansion(44);
		caller->setType(MP_WRITE);
		Match(MP_WRITE);
		Match(MP_LPAREN);
		WriteParameter(expressionRec);		
		WriteParameterTail(expressionRec);
		Match(MP_RPAREN);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::WriteParameterTail(SemanticRecord* expressionRec)
{
	switch(lookahead)
	{
	// WriteParameterTail -> "," WriteParameter, rule# 45
	case MP_COMMA:  
		parseTree->LogExpansion(45);
		Match(MP_COMMA);
		WriteParameter(expressionRec);
		WriteParameterTail(expressionRec);	
		break;
	// WriteParameterTail -> e, rule# 46
	case MP_RPAREN: 
		parseTree->LogExpansion(46);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::WriteParameter(SemanticRecord* expressionRec)
{
	switch(lookahead)
	{
	// WriteParameter -> OrdinalExpression, rule# 47
	case MP_PLUS:
	case MP_MINUS:
	case MP_INTEGER_LIT:
		OrdinalExpression(expressionRec);
		if (negativeFlag==true) 
		{
			Gen_Assembly("NEG -1(SP) -1(SP)		; optional sign negative");
			negativeFlag=false;
		}
		Gen_Assembly("WRTS");
		break;
	case MP_UNSIGNEDINTEGER:
	case MP_NOT:
	case MP_LPAREN:
		Match(MP_LPAREN);
		OrdinalExpression(expressionRec);
		if (negativeFlag==true) 
		{
			Gen_Assembly("NEG -1(SP) -1(SP)		; optional sign negative");
			negativeFlag=false;
		}
		Gen_Assembly("WRTS");
		Match(MP_RPAREN);
		break;
	case MP_FLOAT_LIT:
		OrdinalExpression(expressionRec);
		if (negativeFlag==true) 
		{
			Gen_Assembly("NEG -1(SP) -1(SP)		; optional sign negative");
			negativeFlag=false;
		}
		Gen_Assembly("WRTS");
		break;
	case MP_FIXED_LIT:
		OrdinalExpression(expressionRec);
		if (negativeFlag==true) 
		{
			Gen_Assembly("NEG -1(SP) -1(SP)		; optional sign negative");
			negativeFlag=false;
		}
		Gen_Assembly("WRTS");
		break;
	case MP_STRING:
		Gen_Assembly("WRT #\"" + scanner->getLexeme() + "\"");
		if (negativeFlag==true) 
		{
			Gen_Assembly("NEG -1(SP) -1(SP)		; optional sign negative");
			negativeFlag=false;
		}
		Match(MP_STRING);
		break;
	case MP_TRUE:
	case MP_FALSE:
	case MP_IDENTIFIER:{
		// Lookup identifier from the symbol table and generate assembly for it	
		OrdinalExpression(expressionRec);
		if (negativeFlag==true) 
		{
			Gen_Assembly("NEG -1(SP) -1(SP)		; optional sign negative");
			negativeFlag=false;
		}
		Gen_Assembly("WRTS");
		break;
					   }
	default: // Everything else
		parseTree->LogExpansion(47);
		OrdinalExpression(expressionRec);
		Gen_Assembly("WRTS");
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::AssignmentStatement(SemanticRecord* expressionRec)
{
	SymbolTable::Record* assignmentRecord = symbolTable->lookupRecord(scanner->getLexeme(), 
		SymbolTable::KIND_VARIABLE, 0);

	switch (lookahead)
	{
	// AssignmentStatement -> VariableIdentifier ":=" Expression, rule# 48
	case MP_IDENTIFIER: 
		parseTree->LogExpansion(48);	

		caller->setKind(SemanticRecord::ASSIGNMENT);
		VariableIdentifier();
		Match(MP_ASSIGN);
		expressionRec->setType(caller->getType()); 
		Expression(expressionRec);

		// Pop top value back into variable
		if (expressionRec->getType()==assignmentRecord->token)
		{
			Gen_Assembly("POP " + to_string(assignmentRecord->offset) + "(D0)");
		}
		else
		{
		  if (assignmentRecord->token==MP_FLOAT_LIT)
		  {
		    Gen_Assembly
		    ("CASTSF			; result does not match assignemnt variable type cast to float");
		    Gen_Assembly
		    ("POP " + to_string(assignmentRecord->offset) + "(D0)");
		  }
		  else if (assignmentRecord->token==MP_INTEGER_LIT)
		  {
		    Gen_Assembly
		    ("CASTSI			; result does not match assignemnt variable type cast to integer");
		    Gen_Assembly
			("POP " + to_string(assignmentRecord->offset) + "(D0)");
		  }
		  else if (assignmentRecord->token == MP_BOOLEAN)
		  {
		    Gen_Assembly
		    ("POP " + to_string(assignmentRecord->offset) + "(D0)			; boolean assign");
		  }
		}
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::IfStatement(SemanticRecord* expressionRec)
{
	string ifFalseLabel;
	string exitLabel;
	switch (lookahead)
	{
	// IfStatement -> "if" BooleanExpression "then" Statement OptionalElsePart, rule# 50
	case MP_IF: 
		parseTree->LogExpansion(50);

		caller->setKind(SemanticRecord::SIMPLE_PARAMETER);
		Match(MP_IF);
		ifFalseLabel = LabelMaker();
		exitLabel = LabelMaker();
		BooleanExpression();
		
		// The Result of booleanExpression should be the top of the stack
		Gen_Assembly("BRFS "+ ifFalseLabel + "		; branch if false");
		Match(MP_THEN);
		Statement();
		Gen_Assembly("BR "+ exitLabel + "		;after then statement branch exit label");
		Gen_Assembly("\n" + ifFalseLabel + ":		; if false");	// Drop if false label
		OptionalElsePart();
		Gen_Assembly("\n" + exitLabel + ":		; exit label ");	// Drop if exit label
		break;

	default: // Everything else
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::OptionalElsePart()
{
	switch (lookahead)
	{
	// OptionalElsePart -> "else" Statement, rule# 51
	case MP_ELSE: 
		parseTree->LogExpansion(51);
		Match(MP_ELSE);
		Statement();
		break;
	// OptionalElsePart -> e, rule #52
	case MP_END:
	case MP_SCOLON: 
		parseTree->LogExpansion(52);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::RepeatStatement()
{
	switch(lookahead)
	{
	// RepeatStatement -> "repeat" StatementSequence "until" BooleanExpression, rule# 53
	case MP_REPEAT: 
		{
			parseTree->LogExpansion(53);
			string repeatStartLabel;
			Match(MP_REPEAT);
			repeatStartLabel  = LabelMaker();
			// Drop label to start loop
			Gen_Assembly("\n" + repeatStartLabel + ":			; repeat start");	
			StatementSequence();
			Match(MP_UNTIL);
			BooleanExpression();
			//If the repeat test is false we just fall through no need for another label
			Gen_Assembly("\nBRFS "+ repeatStartLabel + "		; repeat test fail"); 
			break;
		}
	default:
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::WhileStatement()
{
	switch(lookahead)
	{
	// WhileStatement -> "while" BooleanExpression "do" Statement, rule# 54
	case MP_WHILE: 
		{
			parseTree->LogExpansion(54);
			string whileTestLabel;
			string whileFalseLabel;
			Match(MP_WHILE);
			whileTestLabel = LabelMaker();
			whileFalseLabel = LabelMaker();			
			Gen_Assembly("\n" + whileTestLabel + ":");	// Drop while test label
			BooleanExpression();
			// The result of BooleanExpression is on the top of the stack
			Match(MP_DO);
			Gen_Assembly("\nBRFS "+ whileFalseLabel + "		; while false");
			Statement();
			Gen_Assembly("\nBR "+ whileTestLabel + "		; while true");
			Gen_Assembly("\n" + whileFalseLabel + ":		; while false");
			break;
		}
	default:
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::ForStatement(SemanticRecord* expressionRec)
{

	switch(lookahead)
	{
	// "for" ControlVariable ":=" InitialValue StepValue FinalValue "do" Statement, rule# 55
	case MP_FOR: 
		{
			string forTestLabel;
			string forFalseLabel;
			Token stepType;
			parseTree->LogExpansion(55);
			Match(MP_FOR);
			SymbolTable::Record* assignmentRecord = symbolTable->lookupRecord(scanner->getLexeme(),
				SymbolTable::KIND_VARIABLE, 0);
			ControlVariable();
			Match(MP_ASSIGN);
			InitialValue(expressionRec);
			Gen_Assembly("POP " + to_string(assignmentRecord->offset) + "(D0)");
			stepType=StepValue();

			SymbolTable::Record* finalRecord = symbolTable->lookupRecord(scanner->getLexeme(),
				SymbolTable::KIND_VARIABLE, 0);
			FinalValue(expressionRec);

			string finalValue = to_string(finalRecord->offset) + "(D0)";
			Gen_Assembly("POP " + finalValue);
			Match(MP_DO);
			forTestLabel = LabelMaker();
			forFalseLabel = LabelMaker();
			Gen_Assembly("\n" + forTestLabel + ":");	// Drop test label

			//Test for condition
			if (stepType == MP_TO)
			{
				Gen_Assembly("BGT " + to_string(assignmentRecord->offset) + "(D0) " + finalValue +
					" "  + forFalseLabel + "		; test condition");
			}
			else 
			{
				Gen_Assembly("BLT " + to_string(assignmentRecord->offset) + "(D0) " + finalValue +
					" " + forFalseLabel + "		; test condition");
			}
			Statement();

			if (stepType==MP_TO)
			{
				// Increment the control variable
				Gen_Assembly("PUSH #1			; increment control variable");
				Gen_Assembly("PUSH " + to_string(assignmentRecord->offset) + "(D0)");
				Gen_Assembly("ADDS");
				Gen_Assembly
					("POP " + to_string(assignmentRecord->offset) + "(D0)		; increment done");

			}
			else 
			{
				// Decrement the control variable
				Gen_Assembly("PUSH #1			; decrement control variable");
				Gen_Assembly("NEGS");
				Gen_Assembly("PUSH " + to_string(assignmentRecord->offset) + "(D0)");
				Gen_Assembly("ADDS");
				Gen_Assembly
					("POP " + to_string(assignmentRecord->offset) + "(D0)		; decrement done");

			}
			// Return to start of for loop
			Gen_Assembly("BR "+ forTestLabel + "		; branch to loop start");
			// Place loop exit label
			Gen_Assembly("\n" + forFalseLabel + ":		; loop exit");
			break;
		}
	default:
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::ControlVariable()
{
	switch(lookahead)
	{
	// ControlVariable -> VariableIdentifier Identifier, rule# 56
	case MP_IDENTIFIER: 
		parseTree->LogExpansion(56);
		Match(MP_IDENTIFIER);
		break;
	default:
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::InitialValue(SemanticRecord* expressionRec)
{
	switch(lookahead)
	{	
	// InitialValue -> OrdinalExpression, rule# 57
	case MP_PLUS:				
	case MP_MINUS:			
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
	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
Token Parser::StepValue()
{
	Token stepType;
	switch(lookahead)
	{
	// StepValue -> "to", rule# 58
	case MP_TO: 
		parseTree->LogExpansion(58);
		Match(MP_TO);
		stepType=MP_TO;
		break;
	// StepValue -> "downto", rule# 59
	case MP_DOWNTO: 
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

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::FinalValue(SemanticRecord* expressionRec)
{
	switch(lookahead)
	{
	// FinalValue -> OrdinalExpression, rule# 60
	case MP_PLUS:				
	case MP_MINUS:				
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

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::ProcedureStatement()
{
	switch (lookahead)
	{
	// ProcedureStatement -> ProcedureIdentifier OptionalActualParameterList, rule# 61
	case MP_IDENTIFIER: 
		parseTree->LogExpansion(61);
		Match(MP_IDENTIFIER);
		OptionalActualParameterList();
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::OptionalActualParameterList()
{
	switch(lookahead)
	{
	// OptionalActualParameterList -> "(" ActualParameter ActualParameterTail ")", rule# 62
	case MP_LPAREN: 
		parseTree->LogExpansion(62);
		Match(MP_LPAREN);
		ActualParameter();
		ActualParameterTail();
		Match(MP_RPAREN);
		break;
	case MP_END:
	case MP_SCOLON: // OptionalActualParameterList -> e, rule # 63
		parseTree->LogExpansion(63);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::ActualParameterTail()
{
	switch(lookahead)
	{
	// ActualParameterTail -> "," ActualParameter ActualParameterTail Rule #64
	case MP_COMMA: 
		parseTree->LogExpansion(64);
		Match(MP_COMMA);
		ActualParameter();
		ActualParameterTail();
		break;
	// ActualParameterTail -> e, rule #65
	case MP_RPAREN: 
		parseTree->LogExpansion(65);
		break;
	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::ActualParameter()
{
	switch(lookahead)
	{
	// ActualParameter -> OrdinalExpression , rule# 66
	case MP_PLUS:				
	case MP_MINUS:
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
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::Expression(SemanticRecord* expressionRec)
{
	switch(lookahead)
	{
	// Expression -> SimpleExpression OptionalRelationalPart , rule# 67
	case MP_PLUS:				
	case MP_MINUS:				
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

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::OptionalRelationalPart(SemanticRecord* expressionRec)
{
	SemanticRecord* rightExpressionRec = new SemanticRecord();
	switch(lookahead)
	{
	case MP_EQUAL:
		parseTree->LogExpansion(68);
		RelationalOperator();
		SimpleExpression(rightExpressionRec);

		if (expressionRec->getType()==MP_INTEGER_LIT )
		{
			if (rightExpressionRec->getType()==MP_INTEGER_LIT)
			{
				Gen_Assembly("CMPEQS");  
			}
			else
			{
				Gen_Assembly("CASTSI			; righthandside result is a float, left hand side is an int so cast");
				Gen_Assembly("CMPEQS");
			}
		} 
		else if (expressionRec->getType()==MP_FIXED_LIT ||
			expressionRec->getType() == MP_FLOAT_LIT )
		{
			if (rightExpressionRec->getType()==MP_FLOAT_LIT ||
				rightExpressionRec->getType()==MP_FIXED_LIT)
			{
				Gen_Assembly("CMPEQSF");  
			}
			else
			{
				Gen_Assembly("CASTSF			;righthandside result is an int, left hand side is a float so cast");
				Gen_Assembly("CMPEQSF");
			}
		}
	else 
	{
		
		Gen_Assembly("; ERROR in operation " + EnumToString(lookahead));
	}
	break;
	case MP_LTHAN:
		parseTree->LogExpansion(68);
		RelationalOperator();
		SimpleExpression(rightExpressionRec);

		if (expressionRec->getType()==MP_INTEGER_LIT )
		{
			if (rightExpressionRec->getType()==MP_INTEGER_LIT)
			{
				Gen_Assembly("CMPLTS");  
			}
			else
			{
				Gen_Assembly("CASTSI			; righthandside result is a float, left hand side is an int so cast");
				Gen_Assembly("CMPLTS");
			}
		}
		else if (expressionRec->getType()==MP_FIXED_LIT ||
			expressionRec->getType() == MP_FLOAT_LIT)
		{
			if (rightExpressionRec->getType()==MP_FLOAT_LIT ||
				rightExpressionRec->getType()==MP_FIXED_LIT)
			{
				Gen_Assembly("CMPLTSF");  
			}
			else
			{
				Gen_Assembly("CASTSF			; righthandside result is an int left hand side is a float so cast");
				Gen_Assembly("CMPLTSF");
			}
		}
		else 
		{
			Gen_Assembly("; ERROR in operation " + EnumToString(lookahead));
		}
		break;
	case MP_GTHAN:
		parseTree->LogExpansion(68);
		RelationalOperator();
		SimpleExpression(rightExpressionRec);

		if (expressionRec->getType()==MP_INTEGER_LIT )
		{
			if (rightExpressionRec->getType()==MP_INTEGER_LIT)
			{
				Gen_Assembly("CMPGTS");  
			}
			else
			{
				Gen_Assembly("CASTSI			; righthandside result is a float, left hand side is an int so cast");
				Gen_Assembly("CMPGTS");
			}
		}
		else if (expressionRec->getType()==MP_FLOAT_LIT ||
			expressionRec->getType() == MP_FIXED_LIT)
		{
			if (rightExpressionRec->getType()==MP_FLOAT_LIT ||
				rightExpressionRec->getType()==MP_FIXED_LIT)
			{
				Gen_Assembly("CMPGTSF");  
			}
			else
			{
				Gen_Assembly("CASTSF			; righthandside result is an int, left hand side is an float so cast");
				Gen_Assembly("CMPGTSF");
			}
		}
		else 
		{
			Gen_Assembly("; ERROR in operation " + EnumToString(lookahead));
		}

		break;
	case MP_LEQUAL:
		parseTree->LogExpansion(68);
		RelationalOperator();
		SimpleExpression(rightExpressionRec);

		if (expressionRec->getType()==MP_INTEGER_LIT )
		{
			if (rightExpressionRec->getType()==MP_INTEGER_LIT)
			{
				Gen_Assembly("CMPLES");  
			}
			else
			{
				Gen_Assembly("CASTSI			; righthandside result is a float, left hand side is an int so cast");
				Gen_Assembly("CMPLES");
			}
		}
		else if (expressionRec->getType()==MP_FIXED_LIT ||
			expressionRec->getType() == MP_FLOAT_LIT )
		{
			if (rightExpressionRec->getType()==MP_FLOAT_LIT ||
				rightExpressionRec->getType()==MP_FIXED_LIT)
			{
				Gen_Assembly("CMPLESF");  
			}
			else
			{
				Gen_Assembly("CASTSF			; righthandside result is an int, left hand side is an float so cast");
				Gen_Assembly("CMPLESF");
			}
		}
		else 
		{
			Gen_Assembly("; ERROR in operation " + EnumToString(lookahead));
		}

		break;
	case MP_GEQUAL:
		parseTree->LogExpansion(68);
		RelationalOperator();
		SimpleExpression(rightExpressionRec);

		if (expressionRec->getType()==MP_INTEGER_LIT )
		{
			if (rightExpressionRec->getType()==MP_INTEGER_LIT)
				Gen_Assembly("CMPGES");  
			else
			{
				Gen_Assembly("CASTSI			; righthandside result is a float, left hand side is an int so cast");
				Gen_Assembly("CMPGES");
			}
		}
		else if (expressionRec->getType()==MP_FLOAT_LIT ||
			expressionRec->getType() == MP_FIXED_LIT)
		{
			if (rightExpressionRec->getType()==MP_FLOAT_LIT ||
				rightExpressionRec->getType()==MP_FIXED_LIT)
				Gen_Assembly("CMPGESF");  
			else
			{
				Gen_Assembly("CASTSF			; righthandside result is an int, left hand side is an float so cast");
				Gen_Assembly("CMPGESF");
			}
		}
		else 
		{
			Gen_Assembly("; ERROR in operation " + EnumToString(lookahead));
		}

		break;
	// OptionalRelationalPart -> RelationalOperator SimpleExpression, rule #68
	case MP_NEQUAL: 
		parseTree->LogExpansion(68);
		RelationalOperator();
		SimpleExpression(rightExpressionRec);

		if (expressionRec->getType()==MP_INTEGER_LIT )
		{
			if (rightExpressionRec->getType()==MP_INTEGER_LIT)
			{
				Gen_Assembly("CMPNES");  
			}
			else
			{
				Gen_Assembly("CASTSI			; righthandside result is a float, left hand side is an int so cast");
				Gen_Assembly("CMPNES");
			}
		}
		else if (expressionRec->getType()==MP_FIXED_LIT ||
			expressionRec->getType() == MP_FLOAT_LIT)
		{
			if (rightExpressionRec->getType()==MP_FLOAT_LIT |
				| rightExpressionRec->getType()==MP_FIXED_LIT)
			{
				Gen_Assembly("CMPNESF");  
			}
			else
			{
				Gen_Assembly("CASTSF			; righthandside result is an int, left hand side is an float so cast");
				Gen_Assembly("CMPNESF");
			}
		}
		else 
		{
			
			Gen_Assembly("; ERROR in operation " + EnumToString(lookahead));
		}
		break;
	// OptionalRelationalPart -> e, rule #69
	case MP_SCOLON:
	case MP_END:
	case MP_THEN:
	case MP_ELSE:
	case MP_RPAREN:
	case MP_TO:
	case MP_DOWNTO:
	case MP_DO: 
	case MP_COMMA:
		parseTree->LogExpansion(69);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::RelationalOperator()
{
	switch(lookahead)
	{
	// RelationalOperator -> "=", rule# 70
	case MP_EQUAL:	
		parseTree->LogExpansion(70);
		Match(MP_EQUAL);
		break;
	// RelationalOperator -> "<", rule# 71
	case MP_LTHAN: 
		parseTree->LogExpansion(71);
		Match(MP_LTHAN);
		break;
	// RelationalOperator -> ">", rule# 72
	case MP_GTHAN: 
		parseTree->LogExpansion(72);
		Match(MP_GTHAN);
		break;
	// RelationalOperator -> "<=", rule# 73
	case MP_LEQUAL: 
		parseTree->LogExpansion(73);
		Match(MP_LEQUAL);
		break;
	// RelationalOperator -> ">=", rule# 74
	case MP_GEQUAL: 
		parseTree->LogExpansion(74);
		Match(MP_GEQUAL);
		break;
	// RelationalOperator -> "<>", rule# 75
	case MP_NEQUAL: 
		parseTree->LogExpansion(75);
		Match(MP_NEQUAL);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::SimpleExpression(SemanticRecord* expressionRec)
{
	SemanticRecord* termRec = new SemanticRecord();
	SemanticRecord* termTailRec = new SemanticRecord();
	SemanticRecord* resultRec = new SemanticRecord();
	switch(lookahead)
	{
	// SimpleExpression -> Optional Sign Term TermTail, rule# 76
	case MP_PLUS:				
	case MP_MINUS:	
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
		expressionRec->setType(termTailRec->getType());//here
		break;		

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::TermTail(SemanticRecord* termTailRec)
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

		// Check the -1(SP) and -2(SP) to see if they are the same types
		if (termTailRec->getType() == MP_INTEGER_LIT)
		{
			if (termRec->getType() == MP_INTEGER_LIT)
			{
				// Same types, just push and adds
				Gen_Assembly("ADDS");
				termTailRec->setType(MP_INTEGER_LIT);
			} 
			else if (termRec->getType() == MP_FIXED_LIT || termRec->getType() == MP_FLOAT_LIT)
			{
				// Now push the -2(SP) to top of stack, cast it, then push back to -2(SP)
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
				// Same types
				Gen_Assembly("ADDSF");
				termTailRec->setType(MP_FLOAT_LIT);
			} 
			else if (termRec->getType() == MP_INTEGER_LIT)
			{
				// Now  push the -2(SP) to top of stack, cast it, then push back to -2(SP)
				Gen_Assembly("PUSH -2(SP)	; Casting from float to int");
				Gen_Assembly("CASTSI");
				Gen_Assembly("POP -2(SP)	; Cast done");
				Gen_Assembly("ADDS");
				termTailRec->setType(MP_INTEGER_LIT);
			}
		}
		TermTail(termTailRec);
		break;
	// TermTail -> AddingOperator Term TermTail  , rule# 77
	case MP_MINUS: {
		parseTree->LogExpansion(77);
		AddingOperator();
		Term(termRec);

		// Check the -1(SP) and -2(SP) to see if they are the same types
		if (termTailRec->getType() == MP_INTEGER_LIT)
		{
			if (termRec->getType() == MP_INTEGER_LIT)
			{
				// Same types, just push and adds
				Gen_Assembly("SUBS");
				termTailRec->setType(MP_INTEGER_LIT);
			} 
			else if (termRec->getType() == MP_FIXED_LIT || termRec->getType() == MP_FLOAT_LIT)
			{
				// Now push the -2(SP) to top of stack, cast it, then push back to -2(SP)
				Gen_Assembly("PUSH -2(SP)	; Casting from int to float");
				Gen_Assembly("CASTSF");
				Gen_Assembly("POP -2(SP)	; Cast done");
				Gen_Assembly("SUBSF");
				termTailRec->setType(MP_FLOAT_LIT);
			}
		}
		else if (termTailRec->getType() == MP_FIXED_LIT || termTailRec->getType() == MP_FLOAT_LIT)
		{
			if (termRec->getType() == MP_FIXED_LIT || termRec->getType() == MP_FLOAT_LIT)
			{
				// Same types
				Gen_Assembly("SUBSF");
				termTailRec->setType(MP_FLOAT_LIT);
			} 
			else if (termRec->getType() == MP_INTEGER_LIT)
			{
				// Now push the -2(SP) to top of stack, cast it, then push back to -2(SP)
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
	// TermTail -> {e} , rule# 78
	case MP_RPAREN: 
	case MP_END:
	case MP_SCOLON: 
	case MP_EQUAL:
	case MP_LTHAN:
	case MP_GTHAN:
	case MP_GEQUAL:
	case MP_LEQUAL:
	case MP_NEQUAL:
	case MP_DO:
	case MP_THEN:
	case MP_TO: 
	case MP_DOWNTO:
	case MP_ELSE:
	case MP_MOD:
	case MP_COMMA:
		parseTree->LogExpansion(78);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::OptionalSign()
{
	switch(lookahead)
	{
	// OptionalSign -> "+", rule #79
	case MP_PLUS: 
		parseTree->LogExpansion(79);
		Match(MP_PLUS);
		break;
	// OptionalSign -> "-", rule #80
	case MP_MINUS: 
		parseTree->LogExpansion(80);
		Match(MP_MINUS);
		negativeFlag=true;
		break;
	// OptionalSign -> {e}, rule #81
	case MP_IDENTIFIER:
	case MP_NOT:
	case MP_UNSIGNEDINTEGER:
	case MP_INTEGER_LIT:
	case MP_FIXED_LIT:
	case MP_FLOAT_LIT:
	case MP_LPAREN: 
	case MP_TRUE:
	case MP_FALSE:
	case MP_STRING:
		parseTree->LogExpansion(81);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::AddingOperator()
{
	switch(lookahead)
	{
	// AddingOperator -> "+"  , rule# 82
	case MP_PLUS: 
		parseTree->LogExpansion(82);
		Match(MP_PLUS);
		break;
	// AddingOperator -> "-"  , rule# 83
	case MP_MINUS: 
		parseTree->LogExpansion(83);
		Match(MP_MINUS);
		break;
	// AddingOperator -> "or" , rule# 84
	case MP_OR: 
		parseTree->LogExpansion(84);
		Match(MP_OR);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::Term(SemanticRecord* termRec)
{
	switch(lookahead)
	{
	// Term -> Factor FactorTail  , rule# 85
	case MP_UNSIGNEDINTEGER:
	case MP_INTEGER_LIT:
	case MP_FIXED_LIT:
	case MP_FLOAT_LIT:
	case MP_TRUE:
	case MP_FALSE:
	case MP_NOT:
	case MP_IDENTIFIER: 
	case MP_STRING:
	case MP_LPAREN:
		parseTree->LogExpansion(85);
		Factor(termRec);
		FactorTail(termRec);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::FactorTail(SemanticRecord* termTailRec)
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
				// Now push the -2(SP) to top of stack, cast it, then push back to -2(SP)
				Gen_Assembly("PUSH -2(SP)	; Casting from int to float");
				Gen_Assembly("CASTSF");
				Gen_Assembly("POP -2(SP)	; casting done");
				Gen_Assembly("DIVSF");
				termTailRec->setType(MP_FLOAT_LIT);
			}
			else 
			{
				// Now push the -2(SP) to top of stack, cast it, then push back to -2(SP)
				Gen_Assembly("PUSH -2(SP)	; Casting from int to float");
				Gen_Assembly("CASTSF");
				Gen_Assembly("POP -2(SP)	; casting done");
				Gen_Assembly("PUSH -1(SP)	; Casting from int to float");
				Gen_Assembly("CASTSF");
				Gen_Assembly("POP -1(SP)	; casting done");
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
				// Now push -2(D0) and cast, then move back to -2(SP)
				Gen_Assembly("PUSH -1(SP)	; Float divison  Casting from int to float");
				Gen_Assembly("CASTSF");
				Gen_Assembly("POP -1(SP)	; casting done");
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

		// Check previous and check next to see if we need to cast
		// Rec is previous record, r, is the current record
		if (termTailRec->getType() == MP_INTEGER_LIT) 
		{
			if (termRec->getType() == MP_INTEGER_LIT)
			{
				Gen_Assembly("DIVS");
				termTailRec->setType(MP_INTEGER_LIT);
			}
			else if (termRec->getType() == MP_FLOAT_LIT || termRec->getType() == MP_FIXED_LIT)
			{
				// Now push the -2(SP) to top of stack, cast it, then push back to -2(SP)
				Gen_Assembly("PUSH -1(SP)	; DIV cast float to int");
				Gen_Assembly("CASTSI");
				Gen_Assembly("POP -1(SP)	; cast done");
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

		// Check previous and check next to see if we need to cast
		// Rec is previous record, r, is the current record
		if (termTailRec->getType() == MP_INTEGER_LIT)
		{
			if (termRec->getType() == MP_INTEGER_LIT)
			{
				Gen_Assembly("MULS");
			}
			else if (termRec->getType() == MP_FLOAT_LIT || termRec->getType() == MP_FIXED_LIT)
			{
				// Now push the -2(SP) to top of stack, cast it, then push back to -2(SP)
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
				// Now push -2(D0) and cast, then move back to -2(SP)
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

		// Check previous and check next to see if we need to cast
		// Rec is previous record, r, is the current record
		if (termTailRec->getType() == MP_INTEGER_LIT)
		{
			if (termRec->getType() == MP_INTEGER_LIT)
			{
				Gen_Assembly("MODS");
				termTailRec->setType(MP_INTEGER_LIT);
			}
			else if (termRec->getType() == MP_FLOAT_LIT || termRec->getType() == MP_FIXED_LIT)
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
	// FactorTail -> MultiplyingOperator Factor FactorTail  , rule# 86
	case MP_AND: 
		parseTree->LogExpansion(86);
		MultiplyingOperator();
		Factor(termRec);
		Gen_Assembly("ANDS");
		FactorTail(termRec);
		break;
	// FactorTail -> {e}, rule# 87
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
	case MP_NEQUAL: 
	case MP_COMMA:
		parseTree->LogExpansion(87);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::MultiplyingOperator()
{
	switch(lookahead)
	{
	// MultiplyingOperator -> "*", rule# 88
	case MP_TIMES: 
		parseTree->LogExpansion(88);
		Match(MP_TIMES);
		break;
	// MultiplyingOperator -> "div", rule# 89
	case MP_DIV: 
		parseTree->LogExpansion(89);
		Match(MP_DIV);
		break;
	// MultiplyingOperator -> "mod", rule# 90
	case MP_MOD: 
		parseTree->LogExpansion(90);
		Match(MP_MOD);
		break;
	// MultiplyingOperator -> "and", rule# 91
	case MP_AND: 
		parseTree->LogExpansion(91);
		Match(MP_AND);
		break;
	// MultiplyingOperator -> "/", rule# 112
	case MP_DIVF: 
		parseTree->LogExpansion(112);
		Match(MP_DIVF);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::Factor(SemanticRecord* termTailRec)
{
	switch(lookahead)
	{
	// Factor -> UnsignedInteger  , rule# 92
	case MP_UNSIGNEDINTEGER: 
		parseTree->LogExpansion(92);
		Match(MP_UNSIGNEDINTEGER);
		break;
	// Factor -> UnsignedInteger  , rule# 95
	case MP_INTEGER_LIT: 
		parseTree->LogExpansion(95);
		termTailRec->setType(MP_INTEGER_LIT);
		Gen_Assembly("PUSH #" + scanner->getLexeme() + "		; Integer litaral");
		if (negativeFlag==true) 
		{
			Gen_Assembly("NEG -1(SP) -1(SP)		; optional sign negative");
			negativeFlag=false;
		}
		Match(MP_INTEGER_LIT);
		break;
	case MP_IDENTIFIER:	{
		// Factor -> VariableIdentifier, rule # 93
		// Lookup factor in symbol table
		SymbolTable::Record* tempRecord = symbolTable->lookupRecord(scanner->getLexeme(),
			SymbolTable::KIND_VARIABLE,0);
		parseTree->LogExpansion(96);

		// Set type of record to the factor type
		termTailRec->setType(tempRecord->token);
		Gen_Assembly("PUSH " + to_string(tempRecord->offset) + "(D0)	; Identifier " + tempRecord->name);

		VariableIdentifier();
		break;
		}
	
	// "not" Factor  , rule# 94
	case MP_NOT: 
		parseTree->LogExpansion(94);
		Match(MP_NOT);

		Factor(termTailRec);
		Gen_Assembly("NOTS");
		break;
	// Factor -> FIXED_LIT  , rule# 95
	case MP_FIXED_LIT: 
		parseTree->LogExpansion(95);

		termTailRec->setType(MP_FIXED_LIT);
		Gen_Assembly("PUSH #" + scanner->getLexeme() + "	; Fixed literal");
		if (negativeFlag==true) 
		{
			Gen_Assembly("NEGF -1(SP) -1(SP)	; optional sign negative");
			negativeFlag=false;
		}
		Match(MP_FIXED_LIT);
		break;	
	case MP_FLOAT_LIT:
		parseTree->LogExpansion(95);

		termTailRec->setType(MP_FLOAT_LIT);
		Gen_Assembly("PUSH #" + scanner->getLexeme() + "	; Float literal");
		if (negativeFlag==true) 
		{
			Gen_Assembly("NEGF -1(SP) -1(SP)	; optional sign negative");
			negativeFlag=false;
		}
		Match(MP_FLOAT_LIT);
		break;
	// Factor -> "(" Expression ")"  , rule# 95
	case MP_LPAREN: 
		parseTree->LogExpansion(95);
		Match(MP_LPAREN);
		Expression(termTailRec);
		Match(MP_RPAREN);
		break;
	// Factor -> String Literal Rule # 114
	case MP_STRING: 
		parseTree->LogExpansion(114);
		Match(MP_STRING);
		break;
	// Factor -> "true" Rule # 115
	case MP_TRUE: 
		parseTree->LogExpansion(115);
		Gen_Assembly("PUSH #1			; boolean true");
		Match(MP_TRUE);
		break;
	// Factor -> "false" Rule # 116
	case MP_FALSE: 
		parseTree->LogExpansion(116);
		Gen_Assembly("PUSH #0			; boolean false");
		Match(MP_FALSE);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::ProgramIdentifier()
{
	switch(lookahead)
	{
	// ProgramIdentifier -> Identifier  , rule# 97
	case MP_IDENTIFIER: 
		parseTree->LogExpansion(97);
		Match(MP_IDENTIFIER);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::VariableIdentifier()
{
	string v = scanner->getLexeme();

	switch(lookahead)
	{
	// VariableIdentifier -> Identifier  , rule# 98
	case MP_IDENTIFIER: 
		parseTree->LogExpansion(98);

		if (caller->getKind()==SemanticRecord::DECLARATION) 
		// We are declaring variables so add them to the symbol table
		{
			symbolTable->insertRecord(scanner->getLexeme(), SymbolTable::KIND_VARIABLE,
				scanner->token());
		}
		else if (caller->getKind()==SemanticRecord::ASSIGNMENT) 
		// We are at the start of an assignment statement get the variables type 
		// and add it to the caller semantic record
		{
			SymbolTable::Record* rec=symbolTable->lookupRecord(v,
				SymbolTable::KIND_VARIABLE, 0);
			caller->setType(rec->token);
		}
		// This is only set up for control statements
		else if (caller->getKind()
			==SemanticRecord::SIMPLE_PARAMETER) // Variable in control statement
		{
			SymbolTable::Record* rec=symbolTable->lookupRecord(v,
				SymbolTable::KIND_VARIABLE, 0);
			caller->setType(rec->token);
		}

		Match(MP_IDENTIFIER);
		break;
	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::ProcedureIdentifier()
{
	switch(lookahead)
	{
	// ProcedureIdentifier -> Identifier , rule# 99
	case MP_IDENTIFIER: 
		parseTree->LogExpansion(99);

		// Create table for new procedure, create new scope table for that procedure		
		symbolTable->createTable();
		Match(MP_IDENTIFIER);
		break;
	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::FunctionIdentifier()
{
	switch(lookahead)
	{
	case MP_IDENTIFIER: // FunctionIdentifier -> Identifier , rule# 100
		parseTree->LogExpansion(100);

		// Add the function to the symbol table
		symbolTable->insertRecord(scanner->getLexeme(), SymbolTable::KIND_FUNCTION,
			scanner->token());
		symbolTable->createTable();

		Match(MP_IDENTIFIER);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::BooleanExpression()
{	
	SemanticRecord* expressionRec = new SemanticRecord();
	switch(lookahead)
	{
	// BooleanExpression -> Expression Expression (-> SimpleExpression OptionalRelationalPart), 
	// rule# 101
	case MP_PLUS:			
	case MP_MINUS:				
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

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::OrdinalExpression(SemanticRecord* expressionRec)
{
	switch(lookahead)
	{
	// OrdinalExpression -> Expression Expression (-> SimpleExpression OptionalRelationalPart), 
	// rule# 102
	case MP_PLUS:				
	case MP_MINUS:				
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

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::IdentifierList()
{
	switch(lookahead)
	{
	// IdentifierList -> Identifier IdentifierTail, rule# 103
	case MP_IDENTIFIER:
		parseTree->LogExpansion(103);

		// Add new record into symbol table
		symbolTable->insertRecord(scanner->getLexeme(), SymbolTable::KIND_VARIABLE,
			scanner->token());
		Match(MP_IDENTIFIER);	
		IdentifierTail();
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::IdentifierTail()
{
	switch(lookahead)
	{
	// IdentifierTail -> "," Identifier IdentifierTail, rule# 104
	case MP_COMMA: 
		parseTree->LogExpansion(104);
		Match(MP_COMMA);

		// Add new record into symbol table
		symbolTable->insertRecord(scanner->getLexeme(), SymbolTable::KIND_VARIABLE,
			scanner->token());
		Match(MP_IDENTIFIER);		
		IdentifierTail();
		break;
	// IdentifierTail -> e  , rule# 105
	case MP_COLON: 
		parseTree->LogExpansion(105);
		break;

	default: // Everything else
		Syntax_Error();
		break;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
Token Parser::Type()
{
	switch(lookahead)
	{
	//Type -> "Integer", rule #107
	case MP_INTEGER_LIT: 
		parseTree->LogExpansion(107);
		Match(MP_INTEGER_LIT);
		return MP_INTEGER_LIT;
	//Type -> "Float", rule #108
	case MP_FLOAT_LIT: 
		parseTree->LogExpansion(108);
		Match(MP_FLOAT_LIT);
		return MP_FLOAT_LIT;
	//Type -> "Float", rule #108
	case MP_FIXED_LIT: 
		parseTree->LogExpansion(108);
		Match(MP_FIXED_LIT);
		return MP_FIXED_LIT;
	// Type -> "String", rule #109
	case MP_STRING:	
		parseTree->LogExpansion(109);
		Match(MP_STRING);
		return MP_STRING;
	//Type -> "Boolean", rule #110
	case MP_BOOLEAN: 
		parseTree->LogExpansion(110);
		Match(MP_BOOLEAN);
		return MP_BOOLEAN;
	// Type -> "Float"  (aka Real), rule# 108
	case MP_REAL: 
		// Added to support real data type (float)
		parseTree->LogExpansion(108);
		Match(MP_REAL);
		return MP_REAL;
	// Added to support in parameters
	case MP_IN:	
		parseTree->LogMessage("No rule defined. In parameter matched.");
		Match(MP_IN);
		return Type();
	// Added to support character data type
	case MP_CHARACTER:			
		parseTree->LogMessage("No rule defined. Character data type matched.");
		Match(MP_CHARACTER);
		return MP_CHARACTER;
	
	default: // Everything else
		Syntax_Error();
		return MP_NULL;
	}
}

// Precondition: (lookahead is a valid token)
// Postcondition: (method applies rules correctly)
void Parser::Match(Token token)
{
	// Puts a token on the tree
	// Gets the next lookahead
	if (token == lookahead)
	{
		parseTree->LogMessage("      Matched: " + EnumToString(token) + " = " 
			+ scanner->getLexeme());
		currentLexeme = scanner->getLexeme();
		currentToken = scanner->token();
		lookahead = scanner->getToken();
	}
	else
		Syntax_Error(token);
}

void Parser::Syntax_Error(Token expected)
{
	// Stops everything and gives a meaningful error message 
	string msg = "";
	msg.append("\nFile: " + fileName + ": \nSyntax error found on line " 
		+ to_string(scanner->getLineNumber()) + ", column " 
		+ to_string(scanner->getColumnNumber()) + ".\n    Expected " + EnumToString(expected) 
		+ " but found " + EnumToString(lookahead) + ".\n    Next token: " 
		+ EnumToString(scanner->getToken())+ "\n    Next Lexeme: " + scanner->getLexeme());
	cout << msg;
	parseTree->LogMessage(msg);
	throw -1;
}

void Parser::Gen_Assembly(string s)
{
	irFile << s << endl;
	printf(s.c_str());
	printf("\n");
}

string Parser::LabelMaker()
{
	string newLabel;
	labelCount++;
	newLabel= "L";
	newLabel.append(to_string(labelCount));
	return newLabel;
}