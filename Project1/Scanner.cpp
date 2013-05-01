#include "Scanner.h"


Scanner::Scanner()
{
	/*
		Constructor. Initialize the lines and columns counters. 
		Setup and populate the TOKENS hash map with reserved words.
	*/
	lines = 0;
	cols = 0;

	// initialize the hash map of all our tokens
	// to retrieve values from hash map --> Token t = tokens["read"];
	tokens["and"]		= MP_AND;
	tokens["begin"]		= MP_BEGIN;
	tokens["boolean"]	= MP_BOOLEAN;
	tokens["character"]	= MP_CHARACTER;
	tokens["div"]		= MP_DIV;
	tokens["do"]		= MP_DO;
	tokens["downto"]	= MP_DOWNTO;
	tokens["else"]		= MP_ELSE;
	tokens["end"]		= MP_END;
	tokens["fixed"]		= MP_FIXED_LIT;
	tokens["float"]		= MP_FLOAT_LIT;
	tokens["for"]		= MP_FOR;
	tokens["function"]	= MP_FUNCTION;
	tokens["if"]		= MP_IF;
	tokens["in"]		= MP_IN;
	tokens["integer"]	= MP_INTEGER_LIT;
	tokens["mod"]		= MP_MOD;
	tokens["not"]		= MP_NOT;
	tokens["or"]		= MP_OR;
	tokens["procedure"]	= MP_PROCEDURE;
	tokens["program"]	= MP_PROGRAM;
	tokens["read"]		= MP_READ;
	tokens["real"]		= MP_REAL;
	tokens["return"]	= MP_RETURN;
	tokens["repeat"]	= MP_REPEAT;
	tokens["string"]	= MP_STRING;
	tokens["then"]		= MP_THEN;
	tokens["to"]		= MP_TO;
	tokens["until"]		= MP_UNTIL;
	tokens["var"]		= MP_VAR;
	tokens["while"]		= MP_WHILE;
	tokens["write"]		= MP_WRITE;
	tokens["writeln"]	= MP_WRITELN;
	tokens["true"]		= MP_TRUE;
	tokens["false"]		= MP_FALSE;
}

Scanner::~Scanner(void)
{
	// Destructor. Release resources (file pointer)
	file.close();
}

bool Scanner::openFile(std::string fName)
{
	/*
		Open file and initialize the file pointer
	*/
	file.open(fName, fstream::in);

	if (file.is_open())
	{
		lines = 1;
		cols = 1;
		return true;
	}
	return false;
}

bool Scanner::hasToken()
{
	/*
		Scanns the input to see if there is a character left. 
		Skips over white space, new lines, and tabs. 
		Leaves file pointer at beginning of next available character. 
	*/

	char next = peek();
	while ((next == ' ') || (next == '\n') || (next == '\t'))
	{
		if (next == '\n')
		{
			// reset the col counter, add to line counter
			lines++;
			cols = 0;
		}
		next = get();
		next = peek();		
	}
	if (next != EOF)
		return true;

	// set lexeme to EOF ascii value
	_lexeme = get();
	return false;
}

Token Scanner::getToken()
{
	/*
		Reads the next available character and dispatches to the
		appropriate FSA for them to parse. 
	*/		


	// do not return MP_COMMENT to the parser
	do 
	{	
		// detect EOF and skip whitespace
		hasToken();	
		char next = peek();	

		// reset the TOKEN and LEXEME variables, FSA will set new values
		_token = MP_NULL;
		_lexeme = "";

		// which FSA to call 
		if (next == '\'') // handle strings, start with ' (single quote)
			handleString();
		else if (isdigit(next))
			handleNumberic();
		else if (isalpha(next) || next=='_' ) // check for identifier
			handleWord();
		else if (next == '{' || next == '}') // handle comments first becuase {} are considered punctation
			handleComment();
		else if (ispunct(next))
			handleSymbol();
		else if (next == EOF) {
			_token = MP_EOF;
			_lexeme = get();
		}
	} while (_token == MP_COMMENT);

	return _token; 
};

string Scanner::getLexeme()
{
	/*
		Returns the last lexeme that was filled in
	*/
	return _lexeme;
};

Token Scanner::token()
{
		return _token;
}

unsigned int Scanner::getLineNumber()
{
	/*
		Returns the current line number
	*/
	return lines;
};

unsigned int Scanner::getColumnNumber()
{
	/*
		Returns the current column counter from the start of the lexeme. 
	*/
	return (cols - _lexeme.length());
};

Token Scanner::handleWord()
{
	/*
		Parses input file to try to read an identifier or reserved word.
		Assumes that file pointer is on first available character.
	*/
	bool done = false;
	bool accept = false;
	int state = 0;

	while (!done)
	{
		// see whats next, dont consume
		char next = peek();

		switch (state)
		{
		case 0:
			// the start of an identifier
			accept = false;
			if (next == '_'){
				state = 2;
				next = get();
				_lexeme.push_back(next);				
			} else if (isalpha(next) || isdigit(next)){
				state = 1;
				next = get();
				_lexeme.push_back(next);
			} else {
				state = 3;
			}
			break;
		case 1:
			// Accept state. 
			accept = true;
			_token = MP_IDENTIFIER;
			if (isalpha(next) || isdigit(next)){
				state = 1;
				next = get();
				_lexeme.push_back(next);
			} else if (next == '_'){
				state = 2;
				next = get();
				_lexeme.push_back(next);
			} else {
				state = 3;
			}
			break;
		case 2:
			// do not accept
			accept = false;
			if (isalpha(next) || isdigit(next) ){
				state = 1;
				next = get();
				_lexeme.push_back(next);
			} else {
				state = 3;
			}
			break;
		case 3:
			done = true;
			break;
		}
	}

	// check to see if lexeme == '_'
	if (!accept && _lexeme.size() == 1){
		_token = MP_ERROR;
		return _token;
	}		
	// must check to see if this lexeme is a reserved word or not
	isReservedWord(_lexeme);
	return _token;
}

Token Scanner::handleComment()
{
	/*
		Parse input file to read in comments. 
		Comments start with { and end with }
		If EOF is reached before closing comment brace, then MP_RUN_COMMENT
		is returned. 
		Comments must stay on the same line also. 
	*/
	bool done = false;
	int state = 0;

	while (!done)
	{
		char next = peek();

		switch (state)
		{
		case 0:
			if (next == '{')
			{
				next = get();
				_lexeme.push_back(next);
				_token = MP_COMMENT;
				state = 1;
			} 
			else
			{
				next = get();
				_lexeme.push_back(next);
				_token = MP_ERROR;
				state = 2;
			}
			break;
		case 1:
			if (next == '}') // end comment
			{
				next = get();
				_lexeme.push_back(next);
				_token = MP_COMMENT;
				state = 2;
			}
			else if (next == '\n') // run on comment
			{
				next = get();
				_token = MP_RUN_COMMENT;
				lines++;
				cols = 0;
				//state = 2;
			} 
			else
			{
				next = get();
				_lexeme.push_back(next);
			}
			break;
		case 2:
			done = true;
			break;
		}
	}

	return _token;
}

Token Scanner::handleString()
{
	/*
		Parse input file to try to read strings. 
		Strings start and end with ' (single quote)
		If EOF is reached before closing string quote, then MP_RUN_STRING
		is returned. 
		Strings must stay on the same line. 
	*/
	bool done = false;
	int state = 0;

	while (!done)
	{
		char next = peek();

		switch (state)
		{
		case 0:
			if (next == '\'') // comment start
			{
				next = get();
				//_lexeme.push_back(next);
				_token = MP_STRING;
				state = 1;
			} 
			else 
			{
				next = get();
				_lexeme.push_back(next);
				_token = MP_ERROR;
				state = 2;			
			}
			break;
		case 1:
			if (next == '\'') // closing ' for string
			{
				next = get();
				//_lexeme.push_back(next);
				_token = MP_STRING;
				state = 2;
			} 
			else if (next == '\n')
			{
				//get(); // consume EOF
				_token = MP_RUN_STRING;
				state = 2;
			}
			else 
			{
				next = get();
				_lexeme.push_back(next);
			}
			break;
		case 2:
			done = true;
			break;
		}
	}

	return _token;
}

Token Scanner::handleNumberic()
{
	/*
		Parse input file and try to read in a number identifier. 
		Numbers can be either integer, fixed, or floating point numbers.
	*/
	int state = 0;
	bool done = false;
	bool accept = false;

	// cycle through states until done
	while (!done)
	{
		char next = peek();

		switch(state)
		{
		case 0:	// DO NOT ACCEPT
			accept = false;
			if (isdigit((int)next))
			{
				next = get();
				_lexeme.push_back(next);
				state = 1;
				_token = MP_INTEGER_LIT;
			} 
			else 
			{
				done = true;
			}
			break;
		case 1: // Accept MP_INTEGER_LIT  {digit}+1	ex: 134323
			accept = true;

			if (isdigit((int)next))
			{
				next = get();
				_lexeme.push_back(next);
				_token = MP_INTEGER_LIT;
			} 
			else if (next == '.') 
			{
				next = get();
				_lexeme.push_back(next);
				state = 2;
			} 
			else 
			{
				done = true;
			}
			break;
		case 2:	// DO NOT ACCEPT
			accept = false;
			if (isdigit((int)next))
			{
				next = get();
				_lexeme.push_back(next);
				state = 3;
				_token = MP_FIXED_LIT;
			}  
			else 
			{
				done = true;
			}
			break;
		case 3: // MP_FIXED_LIT  {digit}.{digit}+1		ex: 234.234234
			accept = true;
			if (isdigit((int)next))
			{
				next = get();
				_lexeme.push_back(next);
			}
			else if ((next == 'e') | (next == 'E'))
			{
				next = get();
				_lexeme.push_back(next);
				state = 4;
			}
			else
			{
				done = true;
			}
			break;
		case 4: // DO NOT ACCEPT
			accept = false;
			if ((next == '+') | (next == '-'))
			{
				next = get();
				_lexeme.push_back(next);
				state = 5;
			}
			else
			{
				done = true;
			}
			break;
		case 5: // MP_FLOAT_LIT		{digit}.{digit}+1 {+|-}1{e|E}1{digit}+1  ex: 01.2234E+1234
			accept = true;
			if (isdigit((int)next))
			{
				next = get();
				_lexeme.push_back(next);
				_token = MP_FLOAT_LIT;
			}
			else
			{
				done = true;
			}
			break;
		}
	}
	
	// back up the file pointer to the last acceptable state
	if (!accept)
	{
		seek(-1);
		_lexeme.pop_back();
	} 
	return _token;
};

Token Scanner::handleSymbol()
{
	/*
		Parse input file and try to read they symbols.
		Assumes file pointer is on the first available character.
	*/
	int state = 0;
	bool done = false;
	bool accept = false;

	// cycle through states until done
	while (!done)
	{
		char next = peek();

		switch(state)
		{
		case 0: //setup
			accept = false;
			if(next == '.')
			{
				next = get();
				_lexeme.push_back(next);
				state = 1;
			}
			else if(next == ',')
			{
				next = get();
				_lexeme.push_back(next);
				state = 2;
			}
			else if(next == ';')
			{
				next = get();
				_lexeme.push_back(next);
				state = 3;
			}
			else if(next == '(')
			{
				next = get();
				_lexeme.push_back(next);
				state = 4;
			}
			else if(next == ')')
			{
				next = get();
				_lexeme.push_back(next);
				state = 5;
			}
			else if(next == '=')
			{
				next = get();
				_lexeme.push_back(next);
				state = 6;
			}
			else if(next == '+')
			{
				next = get();
				_lexeme.push_back(next);
				state = 7;
			}
			else if(next == '-')
			{
				next = get();
				_lexeme.push_back(next);
				state = 8;
			}
			else if(next == '*')
			{
				next = get();
				_lexeme.push_back(next);
				state = 9;
			}
			else if(next == '>')
			{
				next = get();
				_lexeme.push_back(next);
				state = 10;
			}
			else if(next == '<')
			{
				next = get();
				_lexeme.push_back(next);
				state = 11;
			}
			else if(next == ':')
			{
				next = get();
				_lexeme.push_back(next);
				state = 12;
			}
			else if(next == '/')
			{
				next = get();
				_lexeme.push_back(next);
				state = 13;
			}
			else
			{
				//error=true;
				_lexeme.push_back(get());
				_token = MP_ERROR;
				done=true;
				accept=true;
			}
			break;
		//Trivial cases
		case 1: //symbol is period
			accept = true;
			_token = MP_PERIOD;
			done = true;
			break;

		case 2: //symbol is comma
			accept = true;
			_token = MP_COMMA;
			done = true;
			break;

		case 3: //symbol is semicolon
			accept = true;
			_token = MP_SCOLON;
			done = true;
			break;

		case 4: //symbol is left paren
			accept = true;
			_token = MP_LPAREN;
			done = true;
			break;

		case 5: //symbol is right paren
			accept = true;
			_token = MP_RPAREN;
			done = true;
			break;

		case 6: //symbol is equal
			accept = true;
			_token = MP_EQUAL;
			done = true;
			break;

		case 7: //symbol is plus
			accept = true;
			_token = MP_PLUS;
			done = true;
			break;

		case 8: //symbol is minus
			accept = true;
			_token = MP_MINUS;
			done = true;
			break;

		case 9: //symbol is times
			accept = true;
			_token = MP_TIMES;
			done = true;
			break;
		case 13: //symbol is / (float divide)
			accept = true;
			_token = MP_DIVF;
			done = true;
			break;

		//Non trivial cases
		case 10: //symbol is greater than
			accept = true;
			if (next == '=') 
			{
				next = get();
				_lexeme.push_back(next);
				_token = MP_GEQUAL;
				done = true;
			} 
			else 
			{
				_token = MP_GTHAN;
				done = true;
			}
			break;
		case 11: //symbol is less than
			accept = true;
			if (next == '=') 
			{
				next = get();
				_lexeme.push_back(next);
				_token = MP_LEQUAL;
				done = true;
			} 
			else if (next == '>') 
			{
				next = get();
				_lexeme.push_back(next);
				_token = MP_NEQUAL;
				done = true;
			} 
			else 
			{
				_token = MP_LTHAN;
				done = true;
			}
			break;
		case 12: //symbol is colon
			accept = true;
			if (next == '=') 
			{
				next = get();
				_lexeme.push_back(next);
				_token = MP_ASSIGN;
				done = true;
			} 
			else 
			{
				_token = MP_COLON;
				done = true;
			}
			break;
		}
	}
	
	// back up the file pointer to the last acceptable state
	if (!accept)
	{
		seek(-1);
		_lexeme.pop_back();
	} 
	return _token;
};

bool Scanner::isReservedWord(string word)
{
	/*
		Checks the string to see if it is a reserved word. 
		First converts the string to lowercase, then checks to see
		if it is in the hash map of reserved words. 
	*/
	string w = word;
	for (int i = 0; i < word.size(); i++)
		w.at(i) = tolower(word.at(i));

	unordered_map<string, Token>::const_iterator got = tokens.find (w);
	
	if (got != tokens.end()) {
		_token = got->second;
		return true;
	}

	return false;
}

char Scanner::peek()
{
	/*
		Returns the character that would be consumed next.
	*/
	return file.peek();
}

char Scanner::get()
{
	/*
		Returns the next available character in the file pointer. 
	*/
	char n = file.get();
	cols++;
	return n;
}

void Scanner::seek(int n)
{
	/*
		Moves forward or backward in the file pointer. 
		Positive values will move forwards.
		Negative values will move backwards.
	*/
	file.seekg(n, ios::cur);
	cols += n;
}