#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "testScanner.h"
#include "scanner.h"
#include "tokenID.h"
#include "token.h"

using namespace std;

// Takes in a filename and feeds the lines of that file to the scanner, which returns a token and testScanner prints out the token.
void testScanner(string fileName) {
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
                
                if (token.isError() && token.getTokenId() != WS_E) { //whitespace error is not really an error, it's just an edge case
                    cout << "SCANNER ERROR: " << token.getTokenDescription() << ": line " << token.getLineNumber() << endl;
                    file.close();
                    return;
                    
                } else if (token.isFinal()) {
                    cout << token.getTokenDescription() << "(" << token.getTokenInstance() << ") " << " line " << token.getLineNumber() << endl;
                }
            }
            cout << endl;

            if (file.eof()){
                Token endToken = Token(EOF_TK, "EOF", numberLines, 0);
                cout << endToken.getTokenDescription() << "(" << endToken.getTokenInstance() << ") " << " line " << endToken.getLineNumber() << endl;
                break;
            }
        }
    
    } else {
        cerr << "Unable to open file: " << fileName << endl;
    }   

    file.close();
    
}
