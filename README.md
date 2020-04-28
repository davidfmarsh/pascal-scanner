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


## How to use
1. Ensure that all test files and "tokenTable.txt" are in the same folder as the source file. 
2. Compile and run the program in your favorite C IDE or from the command line. A command 
    prompt will appear for steps 2 and 3.
2. When prompted, enter the name of the test file. (ex. "tests/allTokens.txt")
3. When prompted, enter the name of an output file to store the results. (ex "allTokensOutput.txt")
    The output file will be stored in the same folder as the executable file. 
4. The program will execute and store results in output file. If successful, a prompt will display 
    the name of the output file.

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


## Sample Tests
Several premade tests are included to test certain cases as follows.

1. allTokens.txt - This file tests all 23 tokens in the "tokenTable.txt" file. The output should be 
    one token per line, with IDs 1-23 printed in order. ALLTOKENS and VARIABLE will be in the 
    symbol table.

2. caseTest.txt - This file demonstrates the case insensitivity of Pascal. In this example, TEST, test, 
    and TeST are defined as variables. In the output file, this should generate errors on the second 
    two definitions, since these tokens are the same when capitalized. There should only be CASE and 
    one instance of TEST in the symbol table.

4. colonTest.txt - This file ensures that the ":" and ":=" tokens work as expected, testing the lookahead 
    required when a colon is found. The output should list ":" and ":=" with IDs 13 and 15 respectively. 
    COLONTEST will appear in the symbol table.

5. exceedMaxTokenLength.txt - This file tests two identifier names to show how they are handled. Both 
    NUMBER1234567 and LONGERTHANALLOWED exeed the 12 character token limit. NUMBER1234567 has a number 
    following the first 12 characters, so the remaining "7" is tokenized as an integer. 
    LONGERTHANALLOWED has a letter following the first 12 characters, so "LOWED" is tokenized as its 
    own identifier and is added to the symbol table. Thus the symbol table contains EXCEED, 
    NUMBER123456, LONGERTHANAL, and LOWED. Note that an error is printed to the output file in 
    both cases.

6. invalidCharacter.txt - This file tests the handling of invalid characters in the Pascal langauge. In this
    case, INVALID$ CHARACTER is tested. In the output file, INVALID is registered in the symbol table, 
    an error is thrown for "$" being an invalid character, and then CHARACTER is registered to the 
    symbol table. 

7. multiComment.txt - This file tests multi-line comments in Pascal. Lines 2, 3, and 4 are not tokenized,
    therefore only TEST is added to the symbol table. 

8. parenTest.txt - This file tests the lookahead needed to disinguish the "(" token from the "(*" token. 
    The only tokens registered on the line are "(" and ")". Note that this is also an example of single 
    line commenting, as COMMENT is not tokenized or registered to the symbol table. 

9. sampleProgram.txt - This is a full sample program meant to test a practical application of the scanner. 
    TEST, SUM, SUMSQ, I, VALUE, MEAN, and VARIANCE are all added to the symbol table. Since this is a valid 
    program, no errors should be thrown. 

10. simpleTest.txt - This file is a bare-bones test used to ensure that the scanner is working on a basic level.
    SIMPLE should be the only symbol in the symbol table. 

11. singleComment.txt - This program tests single line commenting. None of the words between the comment symbols
    are tokenized or added to the symbol table. Thus, SINGLE is the only symbol in the symbol table. 

12. symbolTest.txt - This program tests that all non-alphanumeric characters are separated from alphanumeric tokens 
    even without a space between. The purpose of this program is twofold. First, it demonstrates that tokens 
    are terminated when an non-alphanumeric character is reached. This prevents errors when minimal whitespace 
    is used. Second, it prints errors to the output file for each time that TEST is declared. This demonstrates 
    that duplicate definitions of one identifier return an error and are only added to the symbol table at 
    the first instance.

13. tooManySymbols.txt - This program demonstrates filling a symbol table and then attempting to add another symbol. 
    For every symbol that is added once the symbol table is full, an error is printed in the output file. That 
    symbol is not added to the symbol table. 

14. undefined.txt - This program is similar to sampleProgram.txt, but demostrates an undefined identifier being 
    called after the BEGIN token is found. An error is printed in the output file for this identifier.
    Notice that since the SUM2 token is not declared, it is not added to the symbol table. 

15. unendingComment.txt - This file shows how a comment that is never ended is handled. TEST is added to the symbol
    table, but from the second line on, no text is tokenized or added to the symbol table.

16. whitespace.txt - This file demonstrates that multiple lines of whitespace are allowed in programs. Line numbers 
    are printed, but there is no text, so no tokens are printed. The scanner operates as expected despite the 
    additional whitespace. 
