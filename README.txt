P1 Scanner CS4280
Liora Wachsstock

I am using option 3, FSA table + driver.
The FSA table is in the file FSAtable.h and the driver is in the file scanner.h, using a combination of the scanner, filter, and FSAcolumn functions.

COMPILATION AND EXECUTION:
For easy compilation use "Make".
Execution: 
./scanner (it will take input from stdin until you type "EOF") OR
./scanner testFile.sp2022 (.sp2022 is a required extension type) OR
./scanner < testFile.sp2022


ORGANIZATION:
main.cpp: validates filename and starts program

testScanner.cpp: tests the scanner by feeding it the input file and printing the resulting tokens
scanner.cpp: contains FSA driver code and main scanner

token.cpp/token.h: contains code for holding token types, including descriptions
tokenID.h: contains enums for states, final token values, and error codes

FSAtable.h: contains a 2D array of states
Characters.h: contains enum for column numbers for the FSA table
