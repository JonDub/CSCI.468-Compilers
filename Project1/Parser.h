#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include "Scanner.h"
#include "Tokens.h"
#include "ParseTree.h"

class Parser
{
public:
	Parser(void);
	Parser(std::string);
	void SetInputFile(std::string);
	~Parser(void);

	bool Parse();

private:
	Token lookahead;
	Scanner* scanner;
	ParseTree* parseTree;
	string fileName;

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
	void IdentifierTail();
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
	void Term();
	void Factor();
	void FactorTail();
	void MultiplyingOperator();
	void TermTail();
	void OptionalSign();
	void AddingOperator();
	void VariableIdentifier();
	void ReadParameter();
	void ReadParameterTail();
	
	void Match(Token);
	void Syntax_Error(Token = MP_NULL);
};

