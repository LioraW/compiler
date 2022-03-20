#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "scanner.h"
#include "tokenID.h"
#include "token.h"

using namespace std;

// Takes in a filename and feeds the lines of that file to the scanner, which returns a token and testScanner prints out the token.
vector<Token> scannerUtility(string fileName) {
    vector<Token> tokens;

    fstream file;
    string line;
    int numberLines = 0;
    int charNum = 0;
    bool openComment = false; 


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
                    tokens.push_back(token);
                
                } else if (token.isError() && token.getTokenId() != WS_E) {
                    cout << "SCANNER ERROR: ";
                    tokens.push_back(token);
                    file.close();
                    return tokens;
                } 
            }
            cout << endl;
        }
    } else {
        cerr << "Unable to open file: " << fileName << endl;
    }   
    file.close();
    return tokens;   
}

