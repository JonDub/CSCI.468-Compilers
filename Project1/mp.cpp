#include "mp.h"

mp::mp()
{
	// constructor
	lines = 0;
	cols = 0;

	// initialize the hash map of all our tokens
	tokens["and"]		= "MP_AND";
	tokens["begin"]		= "MP_BEGIN";
	tokens["div"]		= "MP_DIV";
	tokens["do"]			= "MP_DO";
	tokens["downto"]		= "MP_DOWNTO";
	tokens["else"]		= "MP_ELSE";
	tokens["end"]		= "MP_END";
	tokens["fixed"]		= "MP_FIXED";
	tokens["float"]		= "MP_FLOAT";
	tokens["for"]		= "MP_FOR";
	tokens["function"]	= "MP_FUNCTION";
	tokens["if"]			= "MP_IF";
	tokens["integer"]	= "MP_INTEGER";
	tokens["mod"]		= "MP_MOD";
	tokens["not"]		= "MP_NOT";
	tokens["or"]			= "MP_OR";
	tokens["procedure"]	= "MP_PROCEDURE";
	tokens["program"]	= "MP_PROGRAM";
	tokens["read"]		= "MP_READ";
	tokens["repeat"]		= "MP_REPEAT";
	tokens["then"]		= "MP_THEN";
	tokens["to"]			= "MP_TO";
	tokens["until"]		= "MP_UNTIL";
	tokens["var"]		= "MP_VAR";
	tokens["while"]		= "MP_WHILE";
	tokens["write"]		= "MP_WRITE";
	// to retrieve values from hash map --> string s = tokens["read"];
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
	error = false;
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
	else if (next == '{') // handle comments first becuase {} are considered punctation C std lib
		handleComment();
	else if (ispunct(next))
		handleSymbol();
	
	if (error==true){
		lexeme=get();
		token="MP_ERROR"; 
	}
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
	// handle input for a comment. try to detect a run-on comment when
	// EOF is reached before the closing brace is reached
	// Comments must stay on the same line
	char next = peek();

	while (next != '}')
	{
		if (next == EOF){
			get();
			token = "MP_RUN_COMMENT";
			return token;
		} else if (next == '\n'){ // consume newlines but keep going
			get();	
			next = peek();
		} else {
			lexeme.push_back(next);
			next = get();
			next = peek();
		}
	}
	// consume the closing }
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

	while (next != 39)
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
			else
			{
				int silly=9;
				error=true;
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
	// first conver to all lowercase, then check the word against a 
	// hash set of reserved words, set the token accordingly if its a reserved word
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