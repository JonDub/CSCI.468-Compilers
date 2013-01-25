#include "mp.h"

mp::mp()
{
	// constructor
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
	while ((next == ' ') | (next == '\n'))
	{
		cols++;
		if (next == '\n')
		{
			// reset the col counter, add to line counter
			lines++;
			cols = 1;
		}
		next = get();
		next = peek();		
	}
	if (next != EOF)
			return true;
	return false;
}

string mp::getToken()
{
	char next = peek();
	string token = "";

	// don't return anything if EOF
	/*if (next == EOF)
	{
		next = get();
		token.push_back(next);
		return token;
	}*/
	
	// consume white space until we reach a character or EOF
	/*while (next != EOF && next == ' ')
	{
		next = get();
		next = peek();
	}*/

	// which FSA to call 	
	if (isdigit(next))
		return handleNumberic();
	else if (isalpha(next))
		int n = get();
		//return handleAlpa();
	else if (ispunct(next))
		int n = get();
		//return handleSymbol();

	// convert to string before sending back??
	return token;
};

char mp::getLexeme()
{
	return 'a';
};

int mp::getLineNumber()
{
	return lines;
};

int mp::getColumnNumber()
{
	// gets the column 
	return cols;
};

string mp::handleAlpa()
{
	return "";
};

string mp::handleNumberic()
{
	// Assuming on the beginning of the next possible token
	string token = "";
	int numMoves = 0;
	int state = 0;
	bool done = false;
	bool accept = false;

	// cycle through states until done
	while (!done)
	{
		char next = peek();

		switch(state)
		{
		case 0:
			accept = false;
			if (isdigit((int)next))
			{
				next = get();
				token.push_back(next);
				numMoves++;
				state = 1;
			} 
			else 
			{
				done = true;
			}
			break;
		case 1:
			accept = true;
			if (isdigit((int)next))
			{
				next = get();
				token.push_back(next);
				numMoves++;
			} 
			else if (next == '.') 
			{
				next = get();
				token.push_back(next);
				numMoves++;
				state = 2;
			} 
			else 
			{
				done = true;
			}
			break;
		case 2:
			accept = false;
			if (isdigit((int)next))
			{
				next = get();
				token.push_back(next);
				numMoves++;
				state = 3;
			}  
			else 
			{
				done = true;
			}
			break;
		case 3:
			accept = true;
			if (isdigit((int)next))
			{
				next = get();
				token.push_back(next);
				numMoves++;
			}
			else if ((next == 'e') | (next == 'E'))
			{
				next = get();
				token.push_back(next);
				numMoves++;
				state = 4;
			}
			else
			{
				done = true;
			}
			break;
		case 4:
			accept = false;
			if ((next == '+') | (next == '-'))
			{
				next = get();
				token.push_back(next);
				numMoves++;
				state = 5;
			}
			else
			{
				done = true;
			}
			break;
		case 5:
			accept = true;
			if (isdigit((int)next))
			{
				next = get();
				token.push_back(next);
				numMoves++;
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
		file.seekg(-1, ios::cur);
		token.pop_back();
	} 
	return token;
};

string mp::handleSymbol()
{
	return "";
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