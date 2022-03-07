#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "testScanner.h"
#include "scanner.h"
#include "tokenID.h"

using namespace std;

void testScanner(string fileName) {

    
string tokenNames[] = { 
    "ID",
    "Number",
    "Equality",
    "Not Equal",
    "Greater than Equal to",
    "Less than Equal to",
    "Assignment",
    "Colon",
    "Colon Equal",
    "Plus",
    "Minus",
    "Multiply",
    "Division",
    "Modulous",
    "Dot",
    "Comma",
    "Left Parentheses",
    "Right Parentheses",
    "Left Curly Bracket",
    "Right Curly Bracket",
    "Semicolon",
    "Left Bracket",
    "Right Bracket",
    "start",
    "stop",
    "repeat", 
    "until", 
    "whole", 
    "label", 
    "quit", 
    "listen", 
    "yell", 
    "main", 
    "portal", 
    "if", 
    "then", 
    "pick", 
    "declare", 
    "assign",
    "proc",
    "End of line",

};

string errorNames[] = {
    "Whitespace is not a token",
    "ID cannot start with a number or a capital letter",
    "unfished assignment (unrecongnized character after = )",
    "unfinished less than or equal to ",
    "unfinished greater than or equal to",
    "hanging !",
};

    fstream file;
    string line;
    int numberLines = 0;
    int charNum = 0;
    bool openComment = false;

    file.open(fileName);

    if (file.is_open()) {
        
        while (getline(file, line)) {
            charNum = 0;

            cout << "Line " << ++numberLines << ":" << line << endl;

            while (charNum < line.length()) {
                //get next token
                Token token = scanner(line, numberLines, charNum, line.length());
                
                if (token.isError()) {
                    cout << errorNames[abs(token.getTokenId())] << ": " << token.getTokenInstance() << " |";
                    file.close();
                    return;
                
                } else if (token.isFinal()) {
                    cout << tokenNames[token.getTokenId() - ID_TK] << "(" << token.getTokenInstance() << ") | ";
                } else {
                    cout << "State error: " << token.getTokenId() << "( " << token.getTokenInstance() << " )" << endl;
                }
            }
            cout << endl << endl;

            if (file.eof()){
                break;
            }
        }
    
    } else {
        cerr << "Unable to open file: " << fileName << endl;
    }   

    file.close();
    
}
