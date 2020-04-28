

## Sample Tests
Several premade tests are included to test certain cases as follows.

* allTokens.txt - This file tests all 23 tokens in the "tokenTable.txt" file. The output should be 
    one token per line, with IDs 1-23 printed in order. ALLTOKENS and VARIABLE will be in the 
    symbol table.

* caseTest.txt - This file demonstrates the case insensitivity of Pascal. In this example, TEST, test, 
    and TeST are defined as variables. In the output file, this should generate errors on the second 
    two definitions, since these tokens are the same when capitalized. There should only be CASE and 
    one instance of TEST in the symbol table.

* colonTest.txt - This file ensures that the ":" and ":=" tokens work as expected, testing the lookahead 
    required when a colon is found. The output should list ":" and ":=" with IDs 13 and 15 respectively. 
    COLONTEST will appear in the symbol table.

* exceedMaxTokenLength.txt - This file tests two identifier names to show how they are handled. Both 
    NUMBER1234567 and LONGERTHANALLOWED exeed the 12 character token limit. NUMBER1234567 has a number 
    following the first 12 characters, so the remaining "7" is tokenized as an integer. 
    LONGERTHANALLOWED has a letter following the first 12 characters, so "LOWED" is tokenized as its 
    own identifier and is added to the symbol table. Thus the symbol table contains EXCEED, 
    NUMBER123456, LONGERTHANAL, and LOWED. Note that an error is printed to the output file in 
    both cases.

* invalidCharacter.txt - This file tests the handling of invalid characters in the Pascal langauge. In this
    case, INVALID$ CHARACTER is tested. In the output file, INVALID is registered in the symbol table, 
    an error is thrown for "$" being an invalid character, and then CHARACTER is registered to the 
    symbol table. 

* multiComment.txt - This file tests multi-line comments in Pascal. Lines 2, 3, and 4 are not tokenized,
    therefore only TEST is added to the symbol table. 

* parenTest.txt - This file tests the lookahead needed to disinguish the "(" token from the "(*" token. 
    The only tokens registered on the line are "(" and ")". Note that this is also an example of single 
    line commenting, as COMMENT is not tokenized or registered to the symbol table. 

* sampleProgram.txt - This is a full sample program meant to test a practical application of the scanner. 
    TEST, SUM, SUMSQ, I, VALUE, MEAN, and VARIANCE are all added to the symbol table. Since this is a valid 
    program, no errors should be thrown. 
    
* simpleTest.txt - This file is a bare-bones test used to ensure that the scanner is working on a basic level.
    SIMPLE should be the only symbol in the symbol table. 

* singleComment.txt - This program tests single line commenting. None of the words between the comment symbols
    are tokenized or added to the symbol table. Thus, SINGLE is the only symbol in the symbol table. 

* symbolTest.txt - This program tests that all non-alphanumeric characters are separated from alphanumeric tokens 
    even without a space between. The purpose of this program is twofold. First, it demonstrates that tokens 
    are terminated when an non-alphanumeric character is reached. This prevents errors when minimal whitespace 
    is used. Second, it prints errors to the output file for each time that TEST is declared. This demonstrates 
    that duplicate definitions of one identifier return an error and are only added to the symbol table at 
    the first instance.

* tooManySymbols.txt - This program demonstrates filling a symbol table and then attempting to add another symbol. 
    For every symbol that is added once the symbol table is full, an error is printed in the output file. That 
    symbol is not added to the symbol table. 

* undefined.txt - This program is similar to sampleProgram.txt, but demostrates an undefined identifier being 
    called after the BEGIN token is found. An error is printed in the output file for this identifier.
    Notice that since the SUM2 token is not declared, it is not added to the symbol table. 

* unendingComment.txt - This file shows how a comment that is never ended is handled. TEST is added to the symbol
    table, but from the second line on, no text is tokenized or added to the symbol table.

* whitespace.txt - This file demonstrates that multiple lines of whitespace are allowed in programs. Line numbers 
    are printed, but there is no text, so no tokens are printed. The scanner operates as expected despite the 
    additional whitespace. 
