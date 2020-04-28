# Pascal Scanner 


## What is this?
This program accepts as input a text file of code in a simplified version of the Pascal language. 
The program tokenizes all symbols in the text file line-by-line and assigns them an ID corresponding to 
tokens in the supplied "tokenTable.txt" file. Once a line is read, it is printed to the output file. If 
a token does not match a specific token in the token table, it is assigned as an integer or an 
identifier based on the rules for those token types. Once a token is assigned an ID, its string and ID 
are printed on a new line. After the end of the file is reached, a symbol table, which stores all 
identifiers generated in the input file, is printed. Identifiers are placed in the symbol table using a 
hashing function for fast searches.


## How to Use
1. Compile and run the program in your favorite C IDE or from the command line. A command 
    prompt will appear for steps 2 and 3.
2. When prompted, enter the path of the test file. (ex. "tests/allTokens.txt")
3. When prompted, enter the name of an output file to store the results. (ex "allTokensOutput.txt")
    The output file will be stored in the same folder as the executable file. 
4. The program will execute and store results in output file. If successful, a prompt will display 
    the name of the output file.
    
## Testing
The following constraints must be adhered to when testing.
### Filenames
* File names can be no more than 30 characters total.
### Lines
* Lines in a test program can be no longer than 80 characters (81 including the newline character).
### Identifiers
* Identifiers are alphanumeric strings of 12 or less characters that begin with a letter. A token can only be an identifier if its string does not match one in the "tokenTable.txt" file.
### Symbol Table
* There can be no more than 32 identifiers defined in a single input program. Any additional 
        identifiers will not be added to the symbol table and will print an error in the output file.
* If an identifier is defined twice, it will only be added to the symbol table the first time 
        and an error will be printed for subsequent definitions.
* Any identifier appearing before the BEGIN token will be added to the symbol table if it is not 
        full and the identifier is not in the symbol table already.
* Any identifier appearing after the BEGIN token will be searched for in the symbol table. If it 
        is not found, an error will be printed to the output file.
### Token Table
* The included "tokenTable.txt" file must be present in the same folder as the executable file
        for this program to work properly. 
