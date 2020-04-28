/*-----------------------------------------------------------------------------
* Author: David Marsh
* 
* This program accepts as input a text file containing code in a simplified 
* version of the Pascal language. The program tokenizes the code, assigns 
* token IDs based on a supplied token table, and outputs to a new text file 
* each line of code followed by the tokens on that line. 
*
* This program also manages a symbol table for all identifier tokens 
* in the input file. If the identifier appears before the BEGIN token and is 
* not already in the symbol table, then it is added to the symbol table if 
* there is room. If the identifier appears after the BEGIN token, then the 
* table is searched for the identifier. If the identifier is not found, an 
* error is printed to the output file.
*
* NOTE: "tokenTable.txt" must contain the token table and must be in the same 
* folder as this program to function properly. 
-----------------------------------------------------------------------------*/
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 81
#define TOKEN_LENGTH 12
#define TOKEN_TABLE_LENGTH 23
#define SYMBOL_TABLE_LENGTH 32
#define MAX_FILENAME_LENGTH 30

typedef struct
{
    char string[LINE_LENGTH];
    int lineNumber;
    int index;
} line;

typedef struct 
{
    char string[TOKEN_LENGTH];
    int index;
    int ID;
} token;

typedef struct
{
	token token[TOKEN_TABLE_LENGTH];
} tokenTable;

typedef struct
{
	token token[SYMBOL_TABLE_LENGTH];
} symbolTable;


void addSymbol(token *currentToken, symbolTable *symTable, int *symbolCount, FILE *outputFile);
void assignID(tokenTable *table, token *token);
token buildToken(FILE *inputFile, line *currentLine, FILE *outputFile);
void checkSymbol(token *currentToken, symbolTable *symTable, bool *beginReached, int *symbolCount, FILE *outputFile);
tokenTable constructTokenTable(char* tokenTableFilename);
symbolTable constructSymbolTable();
void formatLine(line *currentLine);
int getHash(token *currentToken);
void getNewLine(FILE *inputFile, line *currentLine);
token getToken(FILE *inputFile, line *currentLine, tokenTable *table, FILE *outputFile);
void ignoreComment(FILE *inputFile, line *currentLine, FILE *outputFile);
void printLine(FILE *outputFile, line *currentLine);
void printSymbolTable(symbolTable *symTable, FILE *outputFile);
void printToken(FILE *outputFile, token *currentToken);
bool searchForSymbol(token *currentToken, symbolTable *symTable);

/*-----------------------------------------------------------------------------
* The main program calls a function to construct the token table, opens input 
* and output files, then gets tokens, creates a symbol table, and prints the 
* tokens. At the end of the file, the symbol table is printed. Then, the files  
* are closed.
-----------------------------------------------------------------------------*/

int main()
{   
    /* 
    * Use "tokenTable.txt" to build the table that will be referenced
    * to assign token IDs. Data in the file should be in the format:
    *       "TokenString TokenID"
    * where each token is on a new line. TokenStrings should be no longer
    * than the set constant TOKEN_LENGTH. 
    */
    char tokenTableFilename[] = "tokenTable.txt";
    tokenTable table = constructTokenTable(tokenTableFilename);
    
    symbolTable symTable = constructSymbolTable();

    /* 
    * Prompts user for input file name,
    * opens file, and returns an error 
    * if file couldn't be opened.
    */
    printf("Enter name of input file to scan. (maximum of %d characters): ", MAX_FILENAME_LENGTH);
    char inputFilename[MAX_FILENAME_LENGTH];
    scanf("%s", inputFilename);

    FILE *inputFile;
    if ((inputFile = fopen(inputFilename, "r")) == NULL) {
        printf("\nError: %s could not be opened.\n", inputFilename);
        exit(1);
    }

    /* 
    * Prompts user for output file name,
    * opens file, and returns an error 
    * if file couldn't be opened.
    */
    printf("\nEnter name of output file to store results, (maximum of %d characters): ", MAX_FILENAME_LENGTH);
    char outputFilename[MAX_FILENAME_LENGTH];
    scanf("%s", outputFilename);

    FILE *outputFile;
    if ((outputFile = fopen(outputFilename, "w")) == NULL) {
        printf("\nError: %s could not be opened.\n", outputFilename);
        exit(1);
    }

    
    
    line currentLine;
    bool beginReached = false;
    int symbolCount = 0;
    
    while (!feof(inputFile))
    {
    	/*
    	* getToken starts the process of generating tokens for output.
    	* Check symbol determines whether the token needs to be added
    	* to the symbol table or searched for in the table. 
    	*/
        token currentToken = getToken(inputFile, &currentLine, &table, outputFile);
        printToken(outputFile, &currentToken);
        checkSymbol(&currentToken, &symTable, &beginReached, &symbolCount, outputFile);
	}
	
	printSymbolTable(&symTable, outputFile);
    fclose(inputFile); /* close files */
    fclose(outputFile);
    printf("Results stored in %s\n", outputFilename);

    return 0;
}

/*-----------------------------------------------------------------------------
* addSymbol function:
* Input: a pointer to a token object, a pointer to a symbol table object, a 
*		pointer to an integer representing the number of tokens in the symbol 
*		table, and a pointer to an output file
* Output: void
* Purpose: This function adds an identifier to a symbol table and increments
*		the count of items in the symbol table by one. First, the symbol table
*		is checked to determine if the symbol is already in the table. If the 
*		symbol is a duplicate, it is not added to the symbol table and an 
*		error is printed in the output file. The accepted symbols are stored 
*		in the symbol table using a hashing function. The ASCII values of all 
*		characters in the symbol are summed. The sum is then modulo'd by the 
*		number of spaces in the symbol table to determine its location. If 
*		there is a collision, linear probing will place the symbol in the 
*		next available spot.
-----------------------------------------------------------------------------*/

void addSymbol(token *currentToken, symbolTable *symTable, int *symbolCount, FILE *outputFile)
{
	bool duplicate = false;
	int i = 0;
	
	/* Check for duplicate entries */
	while ((i < SYMBOL_TABLE_LENGTH) && (duplicate == false))
	{
		if (strcmp(symTable->token[i].string, currentToken->string) == 0)
		{
			duplicate = true;
			fprintf(outputFile, "\t\t*ERROR: TOKEN \"%s\" IS DECLARED TWICE\n", currentToken->string);
		}
		
		i = i + 1;
	}
	
	/* Calculate hash and place identifier in symbol table */
	if (duplicate != true)
	{
		int tableSlot = getHash(currentToken);
		
		/* Find slot in table */
		if (symTable->token[tableSlot].ID == 0)
		{
			symTable->token[tableSlot] = *currentToken;
		}
		else
		{
			while(symTable->token[tableSlot].ID != 0)
			{
				if (tableSlot == SYMBOL_TABLE_LENGTH)
				{
					tableSlot = 0;
				}
				else if (tableSlot < SYMBOL_TABLE_LENGTH)
				{
					tableSlot = tableSlot + 1;
				}
			}	
			symTable->token[tableSlot] = *currentToken;
		}
		*symbolCount = *symbolCount + 1;
	}
}

/*-----------------------------------------------------------------------------
* assignID function:
* Input: a pointer to a tokenTable object, a pointer to a token object.
* Output: void
* Purpose: Compares myToken's string with all of the token strings in the 
*   tokenTable. If the strings match, then the ID of the token in the table 
*   is assigned to myToken. This function sets the ID of variables to the ID of 
*   the token with string "id" in the table, and the ID of integers to the ID 
*   of the token with string "int" in the table. If no ID is assigned, a 0 
*   ID will be given.
-----------------------------------------------------------------------------*/

void assignID(tokenTable *table, token *myToken)
{
    myToken->ID = 0;

    /* search for match in table to assign ID */
    int i = 0;
    for (i = 0; i < TOKEN_TABLE_LENGTH; i++)
    {
        if (strcmp(table->token[i].string, myToken->string) == 0)
        {
            myToken->ID = table->token[i].ID;
        }
        
    }

    /* if no match is found, set as id or int based on first character */
    if (myToken->ID == 0 && isalpha(myToken->string[0]))
    {
    	
        for (i = 0; i < TOKEN_TABLE_LENGTH; i++)
        {
            if (strcmp(table->token[i].string, "id") == 0)
            {
            	myToken->ID = table->token[i].ID;
            }
        
        }
    }

    else if (myToken->ID == 0 && isalnum(myToken->string[0]))
    {
        for (i = 0; i < TOKEN_TABLE_LENGTH; i++)
        {
            if (strcmp(table->token[i].string, "int") == 0)
            {
                myToken->ID = table->token[i].ID;
            }
        
        }
    }
}

/*-----------------------------------------------------------------------------
* buildToken function:
* Input: a pointer to an input file, a pointer to an output file, a pointer to 
*   a line object.
* Output: a token object
* Purpose: Uses rules of the Simplified Pascal language to create tokens. When 
*   a case is found that requires a token string to end based on the lexical 
*   rules of the language, the "stop" flag is set to true and the token is 
*   returned.
-----------------------------------------------------------------------------*/

token buildToken(FILE *inputFile, line *currentLine, FILE *outputFile)
{
    /* Create and initialize a new token */ 
    token newToken;
    newToken.index = 0;
    int i = 0;
    for (i = 0; i < TOKEN_LENGTH; i++)
    {
        newToken.string[i] = '\0';
    }
    
    bool stop = false;

    /* Skip leading whitespace */
    while (currentLine->string[currentLine->index] == ' ')
        {
            currentLine->index = currentLine->index + 1;  
        }

    while (stop != true)
    {
        /* Add first character that isn't ' ' or '\n' to token and increment.*/
        newToken.string[newToken.index] = currentLine->string[currentLine->index];

        /* validate keywords and IDs */
        if (isalpha(currentLine->string[currentLine->index]))
        {
            if (!isalnum(currentLine->string[currentLine->index + 1]) && 
                         currentLine->string[currentLine->index + 1] != '.')
            {
                stop = true;
            }
        }

        /* validate integers */
        if (isdigit(currentLine->string[currentLine->index]))
        {
            if (!isdigit(currentLine->string[currentLine->index + 1]))
            {
                stop = true;
            }
        }

        /* Switch handles all operators that are not alphanumeric */
        if (!isalnum((currentLine->string[currentLine->index])))
        {
            switch (currentLine->string[currentLine->index])
            {
                /* Allows '.' to be appended but then ends the token */
                case '.': 
                    newToken.string[newToken.index] = 
                        currentLine->string[currentLine->index];
                    newToken.index++;
                    currentLine->index++;
                    stop = true;
                    break;
                /* Handles ':' and ":=" cases */
                case ':':
                    if (currentLine->string[currentLine->index + 1] == '=')
                    {
                        newToken.string[newToken.index] = 
                            currentLine->string[currentLine->index];
                        newToken.index++;
                        currentLine->index++;
                        newToken.string[newToken.index] = 
                            currentLine->string[currentLine->index];
                        
                    }
                    stop = true;
                    break;
                /* Handles '(' and "(*" cases */
                case '(':
                    if (currentLine->string[currentLine->index + 1] == '*')
                    {
                        /* Enter comment mode */
                        newToken.string[newToken.index] = ' ';
                        currentLine->index++;
                        ignoreComment(inputFile, currentLine, outputFile);
                    }
                    stop = true;
                    break;
                case ';':
                case ',':
                case '+':
                case '-':
                case '*':
                case '=': 
                case ')':
                case ' ':
                case '\n':
                case '\0':
                case '\r':
                    stop = true;
                    break;
                default:
                    fprintf(outputFile, "\t\t*ERROR: CHARACTER '%c' NOT"
                            " ALLOWED IN LANGUAGE*\n", 
                            currentLine->string[currentLine->index]);
                    break;
            }
        }
           
        if (currentLine->string[currentLine->index] != ' ' && 
            currentLine->string[currentLine->index] != '\n')
        {
            newToken.index++;
            currentLine->index++;
        }

        if (newToken.index >= TOKEN_LENGTH && 
            isalnum(currentLine->string[currentLine->index]))
        {
            fprintf(outputFile, "\t\t*ERROR: TOO MANY CHARACTERS IN"
                                " FOLLOWING TOKEN. MAX IS 12 CHARACTERS*\n");
            stop = true;
        }
    }  

    return newToken;    
}

/*-----------------------------------------------------------------------------
* checkSymbol function:
* Input: a pointer to a token object, a pointer to a symbol table object, a 
*			pointer to a bool, a pointer to an integer, and a pointer to an 
*			output file
* Output: void
* Purpose: This program determines whether a symbol needs to be added to the 
*		symbol table or searched for in the symbol table. If the symbol is an 
*		identifier and comes before the BEGIN token and the symbol table is 
*		not full, then it is added to the symbol table. If the identifier 
*		comes after the BEGIN token, then the identifier is searched for in 
*		the symbol table. If the symbol is the BEGIN token, then the 
*		beginReached boolean is set to true.
-----------------------------------------------------------------------------*/

void checkSymbol(token *currentToken, symbolTable *symTable, bool *beginReached, int *symbolCount, FILE *outputFile)
{
	/*
	* If BEGIN token is found, mark beginReached as true
	*/
	if (strcmp(currentToken->string, "BEGIN") == 0)
    {
    	*beginReached = true;
	}
	
	/*
	* If identifier is found before BEGIN is found, 
	* add identifier to symbol table
	*/
	else if ((currentToken->ID == 22) && (*beginReached == false))
	{
		if (*symbolCount < (SYMBOL_TABLE_LENGTH))
		{
			addSymbol(currentToken, symTable, symbolCount, outputFile);
		}
		else
		{
			fprintf(outputFile, 
				"\t\t*ERROR: TOO MANY IDENTIFIERS IN PROGRAM, MAX IS %d\n", 
				SYMBOL_TABLE_LENGTH);
		}
			
	}
	
	/*
	* If identifier is found after BEGIN is found, 
	* check for identifier in symbol table
	*/
	else if ((currentToken->ID == 22) && (*beginReached == true))
	{
		bool inTable = searchForSymbol(currentToken, symTable);
		
		if (inTable == false)
		{
			fprintf(outputFile, "\t\t*ERROR: TOKEN %s WAS NOT DECLARED\n", 
				currentToken->string);
		}
	}
}

/*-----------------------------------------------------------------------------
* constructSymbolTable function:
* Input: None
* Output: a symbolTable initialized with blank token 
*			strings and token IDs of 0.
* Purpose: This function creates a symbol table and sets all token 
*			strings to blank and token IDs to zero.
-----------------------------------------------------------------------------*/

symbolTable constructSymbolTable()
{
	symbolTable symTable;
	
	int i = 0;
	for (i = 0; i < SYMBOL_TABLE_LENGTH; i++)
	{
		strset(symTable.token[i].string, ' ');
		symTable.token[i].ID = 0;
	}
	
	return symTable;
}

/*-----------------------------------------------------------------------------
* constructTokenTable function:
* Input: a pointer to a character containing the file name
* Output: a tokenTable object
* Purpose: Builds a tokenTable object using input from a text file. The text 
* file must be of the format "TokenString TokenID" with one token per line and 
* and a maximum TokenString length of TOKEN_LENGTH.
-----------------------------------------------------------------------------*/

tokenTable constructTokenTable(char* tokenTableFilename)
{
    /* Open file containing token table and check for errors */
    FILE *tokenFile;    
    tokenTable table;
    if ((tokenFile = fopen(tokenTableFilename, "r")) == NULL) {
        printf("Error: Token file could not be opened.");
        exit(1);
    }
    
    /* Construct token table */ 
    int i = 0;
    for (i = 0; i < TOKEN_TABLE_LENGTH; i++)
    {
        fscanf(tokenFile, "%s %d", table.token[i].string, &table.token[i].ID);
    }

    /* Close file and return newly constructed token table */
    fclose(tokenFile);
    return table;
}

/*-----------------------------------------------------------------------------
* formatLine function:
* Input: a pointer to a line object.
* Output: void
* Purpose: Converts all characters in the line's string to uppercase, removes 
*   leading whitespace from the line, and appends '\n' if the string does 
*   not have one.
-----------------------------------------------------------------------------*/

void formatLine(line *currentLine)
{
    /* Ensures that all letters are treated as uppercase */
    int k = 0;
    while(currentLine->string[k])
    {
        currentLine->string[k] = toupper(currentLine->string[k]);
        k++;
    }

    /* Removes leading whitespace from string */
    int i, j;
    for(i = 0; currentLine->string[i] == ' ' 
            || currentLine->string[i] == '\t'; i++);

    for(j = 0; currentLine->string[i]; i++)
    {
        currentLine->string[j++] = currentLine->string[i];
    }
    currentLine->string[j] = '\0';  

    /*
    * Appends a new line to any line that does 
    * not have one, such as the last line of a file
    */
    if (!strchr(currentLine->string, '\n'))
    {
        strcat(currentLine->string, "\n");
    }
}

/*-----------------------------------------------------------------------------
* getHash function:
* Input: a pointer to a token obbject
* Output: an integer representing the hashed value of the token
* Purpose: This function accepts a token as input. The ASCII values of each 
*		character in the token string are summed, then the modulo function is 
*		applied with the symbol table length to create a hash value for the 
*		token. The hash value is returned.
-----------------------------------------------------------------------------*/

int getHash(token *currentToken)
{
	int hash = 0;
	int i = 0;
	
	for (i = 0; i < TOKEN_LENGTH; i++)
	{
		hash += currentToken->string[i];
	}
	
	hash = hash % SYMBOL_TABLE_LENGTH;
	
	return hash;
}

/*-----------------------------------------------------------------------------
* getNewLine function:
* Input: a pointer to an input file, a pointer to a line object.
* Output: void
* Purpose: Increments the lineNumber of the line object, assigns a new line 
*   from the file to the line object's string member, and resets the line's 
*   index to zero. 
-----------------------------------------------------------------------------*/

void getNewLine(FILE *inputFile, line *currentLine)
{
    currentLine->lineNumber = currentLine->lineNumber + 1;
    fgets(currentLine->string, LINE_LENGTH, inputFile);
    currentLine->index = 0;
}

/*-----------------------------------------------------------------------------
* getToken function:
* Input: a pointer to an input file, a pointer to the current line, a pointer 
*   to an output file, and a pointer to a table object.
* Output: void
* Purpose: Gets a new line from the input file as needed, then calls functions 
*   to build tokens from the line. Once a token is built, it is assigned a 
*   token ID by comparison to the token table and is printed to the output file.
-----------------------------------------------------------------------------*/

token getToken (FILE *inputFile, line *currentLine, 
				tokenTable *table, FILE *outputFile) 
{
    /* Get new line from input file if needed */
    if ((currentLine->lineNumber == 0) || 
        (currentLine->string[currentLine->index] == '\n'))
    {
        getNewLine(inputFile, currentLine);  
        formatLine(currentLine);
        printLine(outputFile, currentLine);
	}

	token currentToken = buildToken(inputFile, currentLine, outputFile);
	assignID(table, &currentToken);
	
	return currentToken;
}

/*-----------------------------------------------------------------------------
* ignoreComment function:
* Input: a pointer to an input file, a pointer to an output file, a pointer to 
*   a line object.
* Output: void
* Purpose: Ignores all characters within a comment. If a comment passes over 
*   multiple lines, a new line is read from the file.
-----------------------------------------------------------------------------*/

void ignoreComment(FILE *inputFile, line *currentLine, FILE *outputFile)
{
    bool stop = false;

    while (stop == false && !feof(inputFile))
    {
        /* Stopping condition, "*)" */
        if (currentLine->string[currentLine->index] == '*' && 
            currentLine->string[currentLine->index + 1] == ')')
        {
            stop = true;
        }
        /* Gets a new line if needed */
        else if (currentLine->string[currentLine->index] == '\n' || 
                (currentLine->index) == LINE_LENGTH)
        {
            getNewLine(inputFile, currentLine); 
            formatLine(currentLine);
            printLine(outputFile, currentLine);

        }
        
        currentLine->index++;
    }
}

/*-----------------------------------------------------------------------------
* printLine function:
* Input: a pointer to an output file, a pointer to a line object.
* Output: void
* Purpose: Writes the line's lineNumber and string to the output file, 
*   including whitespace between lines.
-----------------------------------------------------------------------------*/

void printLine(FILE *outputFile, line *currentLine)
{
    if (currentLine->lineNumber > 1)
    {
        /* Add space between lines */
        fprintf(outputFile, "\n");
    }
    fprintf(outputFile, "%d\t%s", currentLine->lineNumber, 
                                  currentLine->string);
}

/*-----------------------------------------------------------------------------
* printSymbolTable function:
* Input: a pointer to a symbol table object and a pointer to an output file
* Output: void
* Purpose: This function prints to a given file an entire symbol table, one 
*		item per line. Each line starts with the index of the identifier in 
*		the table and ends with the string of the identifier.
-----------------------------------------------------------------------------*/

void printSymbolTable(symbolTable *symTable, FILE *outputFile)
{	
	fprintf(outputFile, "\n\nSYMBOL TABLE:\n");
	
	int i = 0;
	for (i = 0; i < SYMBOL_TABLE_LENGTH; i++)
	{
		fprintf(outputFile, "%d\t%s\n", i + 1, symTable->token[i].string);
	}
}

/*-----------------------------------------------------------------------------
* printToken function:
* Input: a pointer to an input file, a pointer to a token object.
* Output: void
* Purpose: Writes the token's ID and string to the output file. This function 
*   only writes tokens with valid IDs, and inserts '^' before the string for 
*   variable tokens and '#' for integers.
-----------------------------------------------------------------------------*/

void printToken(FILE *outputFile, token *currentToken)
{
    if (currentToken->ID == 0)
    {
        /*
        * Intentionally blank to prevent printing 
        * tokens without valid IDs, such as forbidden characters.
        */
    }
    else if (currentToken->ID == 22)
    {
        /* Adds a '^' to id tokens */
        fprintf(outputFile, "\t\t%d\t\t^%s\n", 
                currentToken->ID, currentToken->string);
    }
    else if (currentToken->ID == 23)
    {
        /* Adds a '#' to integer tokens */
        fprintf(outputFile, "\t\t%d\t\t#%s\n", 
                currentToken->ID, currentToken->string);
    }
    else
    {
        fprintf(outputFile, "\t\t%d\t\t%s\n", 
                currentToken->ID, currentToken->string);
    }  
}

/*-----------------------------------------------------------------------------
* searchForSymbol function:
* Input: a pointer to a token, a pointer to a symbolTable
* Output: a boolean representing whether or not the token is an element of the 
*		symbol table
* Purpose: This function returns a boolean with the true value if the token is 
*		in the symbol table, and the false value if the token is not. This 
*		function uses a hash to search the table efficiently.
-----------------------------------------------------------------------------*/

bool searchForSymbol(token *currentToken, symbolTable *symTable)
{
	int i = 0;
	int count = 0;
	bool isInTable = false;
	bool emptySpot = false;
	
	/* Calculate hash */
	int tableSlot = getHash(currentToken);
	
	while ((count < SYMBOL_TABLE_LENGTH) && (emptySpot == false))
	{
		if (tableSlot == SYMBOL_TABLE_LENGTH)
		{
			tableSlot = 0;
		}
		
		if (symTable->token[tableSlot].ID == 0)
		{
			/* Empty spot reached, loop can be exited */
			emptySpot = true;
		}
		
		if (strcmp(symTable->token[tableSlot].string, 
			currentToken->string) == 0)
		{
			isInTable = true;
		}
		
		tableSlot = tableSlot + 1;
		count = count + 1;
	}
	
	return isInTable;
}
