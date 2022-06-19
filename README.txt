CS 4280 Compiler
Liora Wachsstock

Compiles our high level language (described in the BNF document) into UMSL's assembly language.

COMPILATION AND EXECUTION:
For easy compilation use "Make".

Execution:
./compfs (will take input from stdin until you type "EOF") OR
./compfs testFile.sp2022 (.sp2022 is a required extension type) OR
./compfs < testFile.sp2022


ORGANIZATION:
main.cpp: validates filename and starts program

parser.cpp/parser.h: parser code (implementation of BNF)

codeGen.cpp/codeGen.h: static semantics and assembly language code generation

tree.cpp/tree.h: code for parse tree

stack.cpp/stack.h: code for static semantics (keeping track of declared variables)

testScanner.cpp: tests the scanner by feeding it the input file and printing the resulting tokens

scanner.cpp: contains FSA driver code and main scanner

token.cpp/token.h: contains code for holding token types, including descriptions

tokenID.h: contains enums for states, final token values, and error codes

FSAtable.h: contains a 2D array of states

Characters.h: contains enum for column numbers for the FSA table
