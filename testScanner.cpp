#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "testScanner.h"
#include "scanner.h"
#include "tokenID.h"
#include "token.h"

using namespace std;

void printToken(Token token);

// Takes in a filename and feeds the lines of that file to the scanner, which returns a token and testScanner prints out the token.
void testScanner(string fileName) {
    fstream file;
    string line;
    int numberLines = 0;
    int charNum = 0;
    bool openComment = false; 

    cout << "Output Formatting: Token Name | Instance, if Applicable | LineNumber.CharNumber" << endl << endl;

    // Open the file
    file.open(fileName);
    if (file.is_open()) {
        
        // Go through the file line by line
        while (getline(file, line)) { 
            numberLines++;
            charNum = 0;

            // Go through the file character by character
            while (charNum < line.length()) {
                
                //receive the tokens as they become relavent
                Token token = scanner(line, charNum, openComment, numberLines, file);
                
                // print the tokens
                if (token.isFinal()) {
                    printToken(token);
                
                } else if (token.isError() && token.getTokenId() != WS_E) {
                    cout << "SCANNER ERROR: ";
                    printToken(token);
                    file.close();
                    return;
                } 
            }
            cout << endl;
        }
    } else {
        cerr << "Unable to open file: " << fileName << endl;
    }   
    file.close();   
}

void printToken(Token token) {
    cout << token.getTokenDescription();
                 
    if (token.getTokenId() == ID_TK || token.getTokenId() == NUM_TK || token.isError()) { //only print an instance if its an ID, num, or error
        cout << " (" << token.getTokenInstance() << ") ";
    }      
    cout  << " " << token.getLineNumber() << "." << token.getCharNumber() << endl;
}
