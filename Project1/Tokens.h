#pragma once

#include <string>

// Does not work for what we need
//#define stringify( name ) # name

enum Token
{
	MP_NULL = 0,
	MP_AND,
	MP_ASSIGN,
	MP_BEGIN,
	MP_BOOLEAN,
	MP_CHARACTER,
	MP_COLON,
	MP_COMMA,
	MP_COMMENT,
	MP_DIV,
	MP_DIVF,
	MP_DO,
	MP_DOWNTO,
	MP_ELSE,
	MP_END,
	MP_EOF,
	MP_EQUAL,
	MP_ERROR,
	MP_FIXED_LIT,
	MP_FLOAT_LIT,
	MP_FOR,
	MP_FUNCTION,
	MP_GEQUAL,
	MP_GTHAN,
	MP_IDENTIFIER,
	MP_IF,
	MP_IN,
	MP_INTEGER_LIT,
	MP_LEQUAL,
	MP_LTHAN,
	MP_LPAREN,
	MP_MINUS,
	MP_MOD,
	MP_NEQUAL,
	MP_NOT,
	MP_OR,
	MP_PERIOD,
	MP_PLUS,
	MP_PROCEDURE,
	MP_PROGRAM,
	MP_READ,
	MP_REAL,
	MP_REPEAT,
	MP_RETURN,
	MP_RPAREN,
	MP_RUN_COMMENT,
	MP_RUN_STRING,
	MP_SCOLON,
	MP_STRING,
	MP_THEN,
	MP_TIMES,
	MP_TO,
	MP_UNTIL,
	MP_UNSIGNEDINTEGER,
	MP_VAR,
	MP_WHILE,
	MP_WRITE,
	MP_WRITELN
};

// Sorry, I couldn't find any easier way to do this. 
// Takes a token enum and returns the string equivalent for output
static std::string EnumToString(Token t)
{
	switch (t)
	{
	case MP_NULL: return "MP_NULL";
	case MP_AND: return "MP_AND";
	case MP_ASSIGN: return "MP_ASSIGN";
	case MP_BEGIN: return "MP_BEGIN";
	case MP_BOOLEAN: return "MP_BOOLEAN";
	case MP_COLON: return "MP_COLON";
	case MP_CHARACTER: return "MP_CHARACTER";
	case MP_COMMA: return "MP_COMMA";
	case MP_COMMENT: return "MP_COMMENT";
	case MP_DIV: return "MP_DIV";
	case MP_DIVF: return "MP_DIVF";
	case MP_DO: return "MP_DO";
	case MP_DOWNTO: return "MP_DOWNTO";
	case MP_ELSE: return "MP_ELSE";
	case MP_END: return "MP_END";
	case MP_EOF: return "MP_EOF";
	case MP_EQUAL: return "MP_EQUAL";
	case MP_ERROR: return "MP_ERROR";
	case MP_FIXED_LIT: return "MP_FIXED_LIT";
	case MP_FLOAT_LIT: return "MP_FLOAT_LIT";
	case MP_FOR: return "MP_FOR";
	case MP_FUNCTION: return "MP_FUNCTION";
	case MP_GEQUAL: return "MP_GEQUAL";
	case MP_GTHAN: return "MP_GTHAN";
	case MP_IDENTIFIER: return "MP_IDENTIFIER";
	case MP_IF: return "MP_IF";
	case MP_IN: return "MP_IN";
	case MP_INTEGER_LIT: return "MP_INTEGER_LIT";
	case MP_LEQUAL: return "MP_LEQUAL";
	case MP_LTHAN: return "MP_LTHAN";
	case MP_LPAREN: return "MP_LPAREN";
	case MP_MINUS: return "MP_MINUS";
	case MP_MOD: return "MP_MOD";
	case MP_NEQUAL: return "MP_NEQUAL";
	case MP_NOT: return "MP_NOT";
	case MP_OR: return "MP_OR";
	case MP_PERIOD: return "MP_PERIOD";
	case MP_PLUS: return "MP_PLUS";
	case MP_PROCEDURE: return "MP_PROCEDURE";
	case MP_PROGRAM: return "MP_PROGRAM";
	case MP_READ: return "MP_READ";
	case MP_REAL: return "MP_REAL";
	case MP_REPEAT: return "MP_REPEAT";
	case MP_RETURN: return "MP_RETURN";
	case MP_RPAREN: return "MP_RPAREN";
	case MP_RUN_COMMENT: return "MP_RUN_COMMENT";
	case MP_RUN_STRING: return "MP_RUN_STRING";
	case MP_SCOLON: return "MP_SCOLON";
	case MP_STRING: return "MP_STRING";
	case MP_THEN: return "MP_THEN";
	case MP_TIMES: return "MP_TIMES";
	case MP_TO: return "MP_TO";
	case MP_UNTIL: return "MP_UNTIL";
	case MP_VAR: return "MP_VAR";
	case MP_WHILE: return "MP_WHILE";
	case MP_WRITE: return "MP_WRITE";
	case MP_WRITELN: return "MP_WRITELN";
	default: return "UNKNOWN";
	}
};

