#pragma once

#include <iostream>
#include <iomanip>
#include "Scanner.h"
#include "Tokens.h"
using namespace std;


class Parser
{
public:
	Parser(void);
	~Parser(void);

private:
	Token lookahead;


	void SystemGoal();
	void Program();
	void ProgramHeading();
	void ProgramIdentifier();
	void Block();
	void VariableDeclarationPart();
	void VariableDeclarationTail();
	void VariableDeclaration();
	void Type();
	void ProcedureAndFunctionDeclarationPart();
	void ProcedureDeclaration();
	void ProcedureIdentifier();
	void FunctionDeclaration();
	void FunctionHeading();
	void FunctionIdentifier();
	void ProcedureHeading();
	void OptionalFormalParameterList();
	void FormalParameterSectionTail();
	void FormalParameterSection();
	void ValueParameterSection();
	void VariableParameterSection();
	void StatementPart();
	void CompoundStatement();
	void StatementSequence();
	void StatementTail();
	void Statement();
	void IdentifierList();
	void ReadStatement();
	void WriteStatement();
	void AssignmentStatement();
	void IfStatement();
	void WhileStatement();
	void RepeatStatement();
	void ForStatement();
	void ProcedureStatement();
	
	void Match(Token);
	void Error();
};

