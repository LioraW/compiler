P3 Static Semantics CS4280
Liora Wachsstock

Storage = Local 

Global variables are processed as local varibles in the top scope. (This is one of the options in the requirments.)

COMPILATION AND EXECUTION:
For easy compilation use "Make".

Execution: 
./statSem (it will take input from stdin until you type "EOF") OR
./statSem testFile.sp2022 (.sp2022 is a required extension type) OR
./statSem < testFile.sp2022


ORGANIZATION:
main.cpp: validates filename and starts program

parser.cpp/parser.h: parser code (implementation of BNF)

tree.cpp/tree.h: code for parse tree

stack.cpp/stack.h: code for static semantics (keeping track of declared variables)

testScanner.cpp: tests the scanner by feeding it the input file and printing the resulting tokens

scanner.cpp: contains FSA driver code and main scanner

token.cpp/token.h: contains code for holding token types, including descriptions

tokenID.h: contains enums for states, final token values, and error codes

FSAtable.h: contains a 2D array of states

Characters.h: contains enum for column numbers for the FSA table
