#include "Scanner.h"

mp::mp()
{
	/*
		Constructor. Initialize the lines and columns counters. 
		Setup and populate the TOKENS hash map with reserved words.
	*/
	lines = 0;
	cols = 0;

	// initialize the hash map of all our tokens
	tokens["and"]		= "MP_AND";
	tokens["begin"]		= "MP_BEGIN";
	tokens["div"]		= "MP_DIV";
	tokens["do"]		= "MP_DO";
	tokens["downto"]	= "MP_DOWNTO";
	tokens["else"]		= "MP_ELSE";
	tokens["end"]		= "MP_END";
	tokens["fixed"]		= "MP_FIXED";
	tokens["float"]		= "MP_FLOAT";
	tokens["for"]		= "MP_FOR";
	tokens["function"]	= "MP_FUNCTION";
	tokens["if"]		= "MP_IF";
	tokens["integer"]	= "MP_INTEGER";
	tokens["mod"]		= "MP_MOD";
	tokens["not"]		= "MP_NOT";
	tokens["or"]		= "MP_OR";
	tokens["procedure"]	= "MP_PROCEDURE";
	tokens["program"]	= "MP_PROGRAM";
	tokens["read"]		= "MP_READ";
	tokens["repeat"]	= "MP_REPEAT";
	tokens["then"]		= "MP_THEN";
	tokens["to"]		= "MP_TO";
	tokens["until"]		= "MP_UNTIL";
	tokens["var"]		= "MP_VAR";
	tokens["while"]		= "MP_WHILE";
	tokens["write"]		= "MP_WRITE";
	// to retrieve values from hash map --> string s = tokens["read"];
	// string will be empty if the value is not in there
}

mp::~mp(void)
{
	// Destructor. Release resources (file pointer)
	file.close();
}

bool mp::openFile(const char* fName)
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

bool mp::hasToken()
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

	lexeme = get();
	return false;
}

string mp::getToken()
{
	/*
		Reads the next available character and dispatches to the
		appropriate FSA for them to parse. 
	*/
	char next = peek();

	// reset the TOKEN and LEXEME variables, FSA will set new values
	token = "";
	lexeme = "";

	// which FSA to call 
	if (next == '\'') // handle strings, start with ' (single quote)
		handleString();
	else if (isdigit(next))
		handleNumberic();
	else if (isalpha(next) || next=='_' ) // check for identifier
		handleWord();
	else if (next == '{' || next == '}') // handle comments first becuase {} are considered punctation C std lib
		handleComment();
	else if (ispunct(next))
		handleSymbol();
	else if (next == EOF) {
		token = "MP_EOF";
		lexeme = get();
	}
	
	return token; 
};

string mp::getLexeme()
{
	/*
		Returns the last lexeme that was filled in
	*/
	return lexeme;
};

unsigned int mp::getLineNumber()
{
	/*
		Returns the current line number
	*/
	return lines;
};

unsigned int mp::getColumnNumber()
{
	/*
		Returns the current column counter from the start of the lexeme. 
	*/
	return (cols - lexeme.length());
};

string mp::handleWord()
{
	/*
		Parses input file to try to read an identifier or reserved word.
		Assumes that file pointer is on first available character.
	*/
	bool done = false;
	bool accept = false;
	char next;
	int state = 0;

	while (!done)
	{
		// see whats next, dont consume
		next = peek();

		switch (state)
		{
		case 0:
			// the start of an identifier
			accept = false;
			if (next == '_'){
				state = 2;
				next = get();
				lexeme.push_back(next);				
			} else if (isalpha(next) || isdigit(next)){
				state = 1;
				next = get();
				lexeme.push_back(next);
			} else {
				state = 3;
			}
			break;
		case 1:
			// Accept state. 
			accept = true;
			token = "MP_IDENTIFIER";
			if (isalpha(next) || isdigit(next)){
				state = 1;
				next = get();
				lexeme.push_back(next);
			} else if (next == '_'){
				state = 2;
				next = get();
				lexeme.push_back(next);
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
				lexeme.push_back(next);
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
	if (!accept && lexeme.size() == 1){
		token = "MP_ERROR";
		return token;
	}		
	// must check to see if this lexeme is a reserved word or not
	isReservedWord(lexeme);
	return token;
}

string mp::handleComment()
{
	/*
		Parse input file to read in comments. 
		Comments start with { and end with }
		If EOF is reached before closing comment brace, then MP_RUN_COMMENT
		is returned. 
		Comments must stay on the same line also. 
	*/
	char next = peek();

	while (next != '}')
	{
		if (next == EOF || next == '\n'){
			get(); // consume EOF
			token = "MP_RUN_COMMENT";
			return token;
		} else if (next == '}') {
			// error detected. no starting comment brace. return error
			lexeme.push_back(next);
			token = "MP_ERROR";
			return token;
		} else {
			lexeme.push_back(next);
			get();
		}
		next = peek();
	}
	// consume the closing }
	lexeme.push_back(next);
	next = get();
	token = "MP_COMMENT";
	return token;
}

string mp::handleString()
{
	/*
		Parse input file to try to read strings. 
		Strings start and end with ' (single quote)
		If EOF is reached before closing string quote, then MP_RUN_STRING
		is returned. 
		Strings must stay on the same line. 
	*/
	char next = get();
	next = peek(); // consume first "

	while (next != '\'') // consume
	{
		if (next == EOF || next == '\n'){
			token = "MP_RUN_STRING";
			return token;
		} 
		lexeme.push_back(next);
		next = get();
		next = peek();
	}
	next = get(); // consume closing "
	token = "MP_STRING";
	return token;
}

string mp::handleNumberic()
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
				lexeme.push_back(next);
				state = 1;
				token = "MP_INTEGER_LIT";
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
				lexeme.push_back(next);
				token = "MP_INTEGER_LIT";
			} 
			else if (next == '.') 
			{
				next = get();
				lexeme.push_back(next);
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
				lexeme.push_back(next);
				state = 3;
				token = "MP_FIXED_LIT";
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
				lexeme.push_back(next);
			}
			else if ((next == 'e') | (next == 'E'))
			{
				next = get();
				lexeme.push_back(next);
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
				lexeme.push_back(next);
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
				lexeme.push_back(next);
				token = "MP_FLOAT_LIT";
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
		lexeme.pop_back();
	} 
	return token;
};

string mp::handleSymbol()
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
				lexeme.push_back(next);
				state = 1;
			}
			else if(next == ',')
			{
				next = get();
				lexeme.push_back(next);
				state = 2;
			}
			else if(next == ';')
			{
				next = get();
				lexeme.push_back(next);
				state = 3;
			}
			else if(next == '(')
			{
				next = get();
				lexeme.push_back(next);
				state = 4;
			}
			else if(next == ')')
			{
				next = get();
				lexeme.push_back(next);
				state = 5;
			}
			else if(next == '=')
			{
				next = get();
				lexeme.push_back(next);
				state = 6;
			}
			else if(next == '+')
			{
				next = get();
				lexeme.push_back(next);
				state = 7;
			}
			else if(next == '-')
			{
				next = get();
				lexeme.push_back(next);
				state = 8;
			}
			else if(next == '*')
			{
				next = get();
				lexeme.push_back(next);
				state = 9;
			}
			else if(next == '>')
			{
				next = get();
				lexeme.push_back(next);
				state = 10;
			}
			else if(next == '<')
			{
				next = get();
				lexeme.push_back(next);
				state = 11;
			}
			else if(next == ':')
			{
				next = get();
				lexeme.push_back(next);
				state = 12;
			}
			else
			{
				//error=true;
				lexeme.push_back(get());
				token = "MP_ERROR";
				done=true;
				accept=true;
			}
			break;
		//Trivial cases
		case 1: //symbol is period
			accept = true;
			token = "MP_PERIOD";
			done = true;
			break;

		case 2: //symbol is comma
			accept = true;
			token = "MP_COMMA";
			done = true;
			break;

		case 3: //symbol is semicolon
			accept = true;
			token = "MP_SCOLON";
			done = true;
			break;

		case 4: //symbol is left paren
			accept = true;
			token = "MP_LPAREN";
			done = true;
			break;

		case 5: //symbol is right paren
			accept = true;
			token = "MP_RPAREN";
			done = true;
			break;

		case 6: //symbol is equal
			accept = true;
			token = "MP_PERIOD";
			done = true;
			break;

		case 7: //symbol is plus
			accept = true;
			token = "MP_PLUS";
			done = true;
			break;

		case 8: //symbol is minus
			accept = true;
			token = "MP_MINUS";
			done = true;
			break;

		case 9: //symbol is times
			accept = true;
			token = "MP_TIMES";
			done = true;
			break;

		//Non trivial cases
		case 10: //symbol is greater than
			accept = true;
			if (next == '=') 
			{
				next = get();
				lexeme.push_back(next);
				token = "MP_GEQUAL";
				done = true;
			} 
			else 
			{
				token = "MP_GTHAN";
				done = true;
			}
			break;
		case 11: //symbol is less than
			accept = true;
			if (next == '=') 
			{
				next = get();
				lexeme.push_back(next);
				token = "MP_LEQUAL";
				done = true;
			} 
			else if (next == '>') 
			{
				next = get();
				lexeme.push_back(next);
				token = "MP_NEQUAL";
				done = true;
			} 
			else 
			{
				token = "MP_LTHAN";
				done = true;
			}
			break;
		case 12: //symbol is colon
			accept = true;
			if (next == '=') 
			{
				next = get();
				lexeme.push_back(next);
				token = "MP_ASSIGN";
				done = true;
			} 
			else 
			{
				token="MP_COLON";
				done = true;
			}
			break;
		}
	}
	
	// back up the file pointer to the last acceptable state
	if (!accept)
	{
		seek(-1);
		lexeme.pop_back();
	} 
	return token;
};

bool mp::isReservedWord(string word)
{
	/*
		Checks the string to see if it is a reserved word. 
		First converts the string to lowercase, then checks to see
		if it is in the hash map of reserved words. 
	*/
	string w = word;
	for (int i = 0; i < word.size(); i++)
		w.at(i) = tolower(word.at(i));

	unordered_map<string, string>::const_iterator got = tokens.find (w);
	
	if (got != tokens.end()) {
		token = got->second;
		return true;
	}

	return false;
}

char mp::peek()
{
	/*
		Returns the character that would be consumed next.
	*/
	return file.peek();
}

char mp::get()
{
	/*
		Returns the next available character in the file pointer. 
	*/
	char n = file.get();
	cols++;
	return n;
}

void mp::seek(int n)
{
	/*
		Moves forward or backward in the file pointer. 
		Positive values will move forwards.
		Negative values will move backwards.
	*/
	file.seekg(n, ios::cur);
	cols += n;
}