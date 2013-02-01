#include "mp.h"

mp::mp()
{
	// constructor
	lines = 0;
	cols = 0;

	// initialize the hash map of reserved words
	reservedWords["and"]		= "MP_AND";
	reservedWords["begin"]		= "MP_BEGIN";
	reservedWords["div"]		= "MP_DIV";
	reservedWords["do"]			= "MP_DO";
	reservedWords["downto"]		= "MP_DOWNTO";
	reservedWords["else"]		= "MP_ELSE";
	reservedWords["end"]		= "MP_END";
	reservedWords["fixed"]		= "MP_FIXED";
	reservedWords["float"]		= "MP_FLOAT";
	reservedWords["for"]		= "MP_FOR";
	reservedWords["function"]	= "MP_FUNCTION";
	reservedWords["if"]			= "MP_IF";
	reservedWords["integer"]	= "MP_INTEGER";
	reservedWords["mod"]		= "MP_MOD";
	reservedWords["not"]		= "MP_NOT";
	reservedWords["or"]			= "MP_OR";
	reservedWords["procedure"]	= "MP_PROCEDURE";
	reservedWords["program"]	= "MP_PROGRAM";
	reservedWords["read"]		= "MP_READ";
	reservedWords["repeat"]		= "MP_REPEAT";
	reservedWords["then"]		= "MP_THEN";
	reservedWords["to"]			= "MP_TO";
	reservedWords["until"]		= "MP_UNTIL";
	reservedWords["var"]		= "MP_VAR";
	reservedWords["while"]		= "MP_WHILE";
	reservedWords["write"]		= "MP_WRITE";

	// to retrieve values from hash map --> string s = reservedWords["read"];
	// string will be empty if the value is not in there
}

mp::~mp(void)
{
	// destructor. release resources
	file.close();
}

bool mp::openFile(const char* fName)
{
	// Opens the file pointer for FSA's to use
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
	// consume white space & newlines until we see a character or reach EOF
	char next = peek();
	while ((next == ' ') | (next == '\n') | (next == '\t'))
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

	token = "MP_EOF";
	return false;
}

string mp::getToken()
{
	char next = peek();

	// reset the TOKEN and LEXEME variables, FSA will set new values
	token = "";
	lexeme = "";

	// which FSA to call 	
	if (isdigit(next))
		handleNumberic();
	else if (isalpha(next) || next=='_' ) // check for identifier
		handleAlpha();
	else if (next == '{') // handle comments first becuase {} are considered punctation C std lib
		handleComment();
	else if (next == '"') // handle strings, start with "
		handleString();
	else if (ispunct(next))
		handleSymbol();
	//else // final case. just read the word in and check if its a keyword or identifier
		//handleWord();
	
	return token;
};

string mp::getLexeme()
{
	return lexeme;
};

unsigned int mp::getLineNumber()
{
	return lines;
};

unsigned int mp::getColumnNumber()
{
	// gets the column 
	return (cols - lexeme.length());
};

char mp::toLowerCase(char c)
{
	if (isalpha(c))
		if ( c < 91) // convert uppercase to lowercase
			c += 32;
	return c;
}

string mp::handleWord()
{
	// handle input for reserved words as well as identiers. 
	// Read in a word, when done check if its a reserved word or an identifier
	// Then set the token and lexeme accordingly
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
			} else if (isalpha(next)){
				state = 1;
				next = get();
				lexeme.push_back(next);
			} else {
				done = true;
			}
			break;
		case 1:
			// Accept state. 
			accept = true;
			token = "MP_IDENTIFIER";
			if (isalpha(next) | isdigit(next)){
				state = 1;
				next = get();
				lexeme.push_back(next);
			} else if (next == '_'){
				state = 2;
				next = get();
				lexeme.push_back(next);
			} else {
				done = true;
			}
			break;
		case 2:
			// do not accept
			accept = false;
			if (isalpha(next) | isdigit(next)){
				state = 1;
				next = get();
				lexeme.push_back(next);
			} else {
				done = true;
			}
			break;
		}
	}

	if (!accept){
		seek(-1);
		lexeme.pop_back();
	}

	return token;
}

string mp::handleComment()
{
	// handle input for a comment. try to detect a run-on comment when
	// EOF is reached before the closing brace is reached
	char next = get();

	while (next != '}')
	{
		if (next == EOF){
			token = "MP_RUN_COMMENT";
			return token;
		} 
		lexeme.push_back(next);
		next = get();
		next = peek();
	}
	lexeme.push_back(next);
	next = get();
	token = "MP_COMMENT";
	return token;
}

string mp::handleString()
{
	// handle input for a strings. try to detect a run-on string when
	// EOF is reached before the closing brace is reached
	char next = get();
	next = peek(); // consume first "

	while (next != '"')
	{
		if (next == EOF){
			token = "MP_RUN_STRING";
			return token;
		} 
		lexeme.push_back(next);
		next = get();
		next = peek();
	}
	next = get(); // consume last "
	token = "MP_STRING";
	return token;
}
string mp::handleAlpha()
{// Assuming on the beginning of the next possible token
	
	lexeme = "";
	int state = 0;
	bool done = false;
	bool accept = false;
	int underscoreCount=0;
	
	//Needs modification to prevent two underscores 
	while (!done)
	{
		char next = peek();

		switch(state)
		{
		case 0: //starts with standard alphabet char or _ ?
			if(isalpha(next) || next=='_')
			{
				next = get();
				next=tolower(next); //Let's keep every thing lowercase to make life easier
				lexeme.push_back(next);
				if (next=='_'){underscoreCount++;accept=false;state=1;}
				else {accept=true;token="MP_IDENTIFIER";state=2;} //for single letter variable name case
				state=1; //advance to test second char case
				
				
			}
			else
			{
				done=true;
			}
			break; //end case 0
			//////////////////////////////////
		case 1:
			if( isalpha(next) || isdigit((int)next) && next !=' ' && next!='_') //Last character seen was _
			{
				next = get();
				next=tolower(next);//Let's keep every thing lowercase to make life easier
				lexeme.push_back(next);
				token="MP_IDENTIFIER";
				state=2;	
			}
				
			
			else
			{
				done=true;
			}
			break; //end case 1

			
		case 2: //test chars where _ not most recent seen and branch for reservered words
			if( isalpha(next) || isdigit((int)next)||next=='_'&& underscoreCount<2 && next !=' ') //Is the second char alpha, digit, or underscore and have we seen less than two underscores so far
			{
				next = get();
				if (next=='_') {underscoreCount++;state=1;}else{state=2;}
				next=tolower(next);//Let's keep every thing lowercase to make life easier
				lexeme.push_back(next);
				accept=true;
				// Given the existing code implementation of state pattern seemed overkill however this code imposes an unnessecary number of string compares, time to code however is reduced.
				// The alternative however will invariably include a large number of states or a large number of nested if statements, which will negatively impact readibility but positively impact total runtime
				if (lexeme.compare("and")==0 )
				{
					token="MP_AND";
				}
				else if (lexeme.compare("begin")==0 )
				{
					token="MP_BEGIN";
				}
				else if (lexeme.compare("div")==0 )
				{
					token="MP_DIV";
				}
				else if (lexeme.compare("do")==0 )
				{
					token="MP_DO";
				}
				else if (lexeme.compare("downto")==0 )
				{
					token="MP_DOWNTO";
				}
				else if (lexeme.compare("else")==0 )
				{
					token="MP_ELSE";
				}
				else if (lexeme.compare("end")==0 )
				{
					token="MP_END";
				}
				else if (lexeme.compare("for")==0 )
				{
					token="MP_FOR";
				}
				else if (lexeme.compare("function")==0 )
				{
					token="MP_FUNCTION";
				}
				else if (lexeme.compare("if")==0 )
				{
					token="MP_IF";
				}
				else if (lexeme.compare("mod")==0 )
				{
					token="MP_MOD";
				}
				else if (lexeme.compare("not")==0 )
				{
					token="MP_NOT";
				}
				else if (lexeme.compare("or")==0 )
				{
					token="MP_OR";
				}
				else if (lexeme.compare("procedure")==0 )
				{
					token="MP_PROCEDURE";
				}
				else if (lexeme.compare("program")==0 )
				{
					token="MP_PROGRAM";
				}
				else if (lexeme.compare("read")==0 )
				{
					token="MP_READ";
				}
				else if (lexeme.compare("repeat")==0 )
				{
					token="MP_REPEAT";
				}
				else if (lexeme.compare("then")==0 )
				{
					token="MP_THEN";
				}
				else if (lexeme.compare("to")==0 )
				{
					token="MP_TO";
				}
				else if (lexeme.compare("until")==0 )
				{
					token="MP_UNTIL";
				}
				else if (lexeme.compare("var")==0 )
				{
					token="MP_VAR";
				}
				else if (lexeme.compare("while")==0 )
				{
					token="MP_WHILE";
				}
				else if (lexeme.compare("write")==0 )
				{
					token="MP_WRITE";
				}
				else
				{
					token="MP_IDENTIFIER";
				}
				
			}
			else
			{
				done=true;
			}
			break; //end case 2
		}//end switch
	}//end while
	 
	

	if (!accept)
	{
		seek(-1);
		lexeme.pop_back();
	} 
	return token;
};

string mp::handleNumberic()
{
	// Assuming on the beginning of the next possible token
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
	// Assuming on the beginning of the next possible token
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

char mp::peek()
{
	// Peek ahead at the next character in the file
	return file.peek();
}

char mp::get()
{
	// Get the next character in the file stream. Increment the columns.
	char n = file.get();
	cols++;
	return n;
}

void mp::seek(int n)
{
	// Move forward/backward in the file by n spaces 
	file.seekg(n, ios::cur);
	cols += n;
}