P3 Static Semantics CS4280
Liora Wachsstock

Storage = Local 

COMPILATION AND EXECUTION:
For easy compilation use "Make".

Execution: 
./statSem (it will take input from stdin until you type "EOF") OR
./statSem testFile.sp2022 (.sp2022 is a required extension type) OR
./statSem < testFile.sp2022


ORGANIZATION:
main.cpp: validates filename and starts program

parse.cpp/parser.h: parser code

tree.cpp/tree.h: tree code

testScanner.cpp: tests the scanner by feeding it the input file and printing the resulting tokens
scanner.cpp: contains FSA driver code and main scanner

token.cpp/token.h: contains code for holding token types, including descriptions
tokenID.h: contains enums for states, final token values, and error codes

FSAtable.h: contains a 2D array of states
Characters.h: contains enum for column numbers for the FSA table
