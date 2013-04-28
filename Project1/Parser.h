#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "Scanner.h"
#include "Tokens.h"
#include "ParseTree.h"
#include "SemanticAnalyzer.h"
#include "SemanticRecord.h"
#include "SymbolTable.h"
using namespace std;


class Parser
{
public:
	Parser(void);
	Parser(string);
	~Parser(void);

	bool parse();
	void setInputFile(string);	

private:
	string irFilename;
	std::ofstream irFile;

	Token lookahead;	
	string fileName;
	string currentLexeme;
	Token currentToken;
	Scanner* scanner;
	ParseTree* parseTree;
	SemanticAnalyzer* analyzer;
	SymbolTable* symbolTable;
	SemanticRecord* caller;
	int labelCount;	
	//string operation; // probably a better way
	//string operationSide; // ditto
	//string currentRightHandSide;
	
	bool negativeFlag;

	string LabelMaker();
	bool SystemGoal();
	void Program();
	void ProgramHeading();
	void ProgramIdentifier();
	void Block();
	void VariableDeclarationPart();
	void VariableDeclarationTail();
	void VariableDeclaration();
	Token Type();
	void ProcedureAndFunctionDeclarationPart();
	void ProcedureDeclaration();
	void ProcedureIdentifier();
	void FunctionDeclaration();
	void FunctionHeading();
	void FunctionIdentifier();
	void ProcedureHeading();
	void OptionalFormalParameterList();
	void OptionalActualParameterList();
	void OptionalRelationalPart(SemanticRecord* &);
	void RelationalOperator();
	void FormalParameterSectionTail();
	void FormalParameterSection();
	void ActualParameter();
	void ActualParameterTail();
	void ValueParameterSection();
	void VariableParameterSection();
	void StatementPart();
	void ControlVariable();
	Token StepValue();
	void FinalValue(SemanticRecord* &);
	void CompoundStatement();
	void StatementSequence();
	void StatementTail();
	void Statement();
	void EmptyStatement();
	void IdentifierList();	
	void IdentifierTail();
	void ReadStatement();
	void WriteStatement(SemanticRecord* &);
	void WriteParameter(SemanticRecord* &);
	void WriteParameterTail(SemanticRecord* &);
	void BooleanExpression();
	void OptionalElsePart();
	void AssignmentStatement(SemanticRecord* &);
	void IfStatement(SemanticRecord* &);
	void WhileStatement();
	void RepeatStatement();
	void ForStatement(SemanticRecord* &);
	void ProcedureStatement();
	void InitialValue(SemanticRecord* &);
	void OrdinalExpression(SemanticRecord* &);
	void Expression(SemanticRecord* &);
	void SimpleExpression(SemanticRecord* &);
	void Term(SemanticRecord* &);
	void Factor(SemanticRecord* &);
	void FactorTail(SemanticRecord* &);
	void MultiplyingOperator();
	void TermTail(SemanticRecord* &);
	void OptionalSign();
	void AddingOperator();
	void VariableIdentifier();
	void ReadParameter();
	void ReadParameterTail();
	
	void Match(Token);
	void Syntax_Error(Token = MP_NULL);
	void Gen_Assembly(string s);
};

