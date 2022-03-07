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
    "Proc",
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
            numberLines++;
            charNum = 0;
            while (charNum < line.length()) {

                Token token = scanner(line, charNum, openComment, numberLines);
                
                if (token.isError()) {

                    if (token.getTokenId() != WS_E) { //whitespace error is not really an error, it's just an edge case
                        cout << "SCANNER ERROR: " << errorNames[abs(token.getTokenId()) - 1] << ": line " << token.getLineNumber() << endl;
                        file.close();
                        return;
                    } 
                    
                } else if (token.isFinal()) {
                    cout << tokenNames[token.getTokenId() - ID_TK] << "(" << token.getTokenInstance() << ") " << " line " << token.getLineNumber() << endl;
                } else {
                    cout << "State error: " << token.getTokenId() << "( " << token.getTokenInstance() << " )" << endl;
                }
            }
            cout << endl;

            if (file.eof()){
                break;
            }
        }
    
    } else {
        cerr << "Unable to open file: " << fileName << endl;
    }   

    file.close();
    
}
