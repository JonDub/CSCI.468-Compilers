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
	Parser(const char*);
	~Parser(void);

	bool Parse();

private:
	Token lookahead;
	Scanner* scanner;


	bool SystemGoal();
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
	void OptionalActualParameterList();
	void OptionalRelationalPart();
	void RelationalOperator();
	void FormalParameterSectionTail();
	void FormalParameterSection();
	void ActualParameter();
	void ActualParameterTail();
	void ValueParameterSection();
	void VariableParameterSection();
	void StatementPart();
	void ControlVariable();
	void StepValue();
	void FinalValue();
	void CompoundStatement();
	void StatementSequence();
	void StatementTail();
	void Statement();
	void EmptyStatement();
	void IdentifierList();
	void ReadStatement();
	void WriteStatement();
	void WriteParameter();
	void WriteParameterTail();
	void BooleanExpression();
	void OptionalElsePart();
	void AssignmentStatement();
	void IfStatement();
	void WhileStatement();
	void RepeatStatement();
	void ForStatement();
	void ProcedureStatement();
	void InitialValue();
	void OrdinalExpression();
	void Expression();
	void SimpleExpression();
	void OptionalRelationPart();
	void ReadParameter();
	void ReadParameterTail();
	
	void Match(Token);
	void Syntax_Error();
};

