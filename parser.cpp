#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "scanner.h"
#include "tokenID.h"
#include "token.h"
#include "parser.h"

using namespace std;

void parser(string fileName){
    vector<Token> tokens = scannerUtility(fileName);
    
    //bandaid for right now
    Token endToken = Token(EOF_TK, "EOF", 0, 0);
    tokens.push_back(endToken);

    vector<Token>::iterator i = tokens.begin();

    program(tokens, i);

    // for (vector<Token>::iterator i = tokens.begin(); i != tokens.end(); ++i) {

    //     if (i->getTokenId() == EOF_TK) {
    //         break;
    //     }
    // }
    
}

void program(vector<Token> tokens, vector<Token>::iterator& i){
    vars(tokens, i);
    if (i->getTokenId() == MN_TK){
        block();
    } else {
        cout << "parser error" << endl;
    }
}
void block(){
    cout << "It's a block!" << endl;
}

void vars (vector<Token> tokens, vector<Token>::iterator& i){
    //  empty | declare Identifier :=  whole  ;  <vars>
    if (i->getTokenId() == DEC_TK){
        cout << i->getTokenDescription() << endl;
        i++;
        if (i->getTokenId() == ID_TK){
            cout << i->getTokenDescription() << ": " << i->getTokenInstance() <<  endl;
            i++;
            if (i->getTokenId() == COLNEQ_TK){
                cout << i->getTokenDescription() << endl;
                i++;
                if(i->getTokenId() == WHL_TK){
                    cout << i->getTokenDescription() << endl;
                    i++;
                    if(i->getTokenId() == SCOLN_TK){
                        cout << i->getTokenDescription() << endl;
                        i++;
                        vars(tokens, i);
                    }
                } else { //error conditons
                    return;
                }

            } else {
                return;
            }
        } else {
            return;
        }
    }
    else {
        return;
    }
}

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

                    if (token.getTokenId() == EOF_TK) {
                        file.close();
                        return tokens;
                    }
                
                } else if (token.isError() && token.getTokenId() != WS_E) {
                    cout << "SCANNER ERROR: ";
                    tokens.push_back(token);
                    file.close();
                    return tokens;
                } 
            }
        }
    } else {
        cerr << "Unable to open file: " << fileName << endl;
    }   
    file.close();
    return tokens;   
}

