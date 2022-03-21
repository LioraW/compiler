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

    program(i);

    // for (vector<Token>::iterator i = tokens.begin(); i != tokens.end(); ++i) {

    //     if (i->getTokenId() == EOF_TK) {
    //         break;
    //     }
    // }
    
}
void printError(int expecting, string actual){
    cout << "Expecting " << expecting << "; Instead found " << actual << endl;
}

void program(vector<Token>::iterator& i){
    vars(i);
    if (i->getTokenId() == MN_TK){
        i++;
        cout << "Main Keyword" << endl;
        block(i);
    } else {
        printError(MN_TK, i->getTokenInstance());
    }
}
void block(vector<Token>::iterator& i){
    if (i->getTokenId() == LCBRC_TK){
        i++;
        cout << "Left curly brace, beginning of block" << endl;
        vars(i);
        stats(i);

        if(i->getTokenId() == RCBRC_TK){
            i++;
            cout << "Right curly brace, end of block" << endl;
            if (i->getTokenId() == EOF_TK){ //hacky 
                return;
            }
        } else {
            printError(RCBRC_TK, i->getTokenInstance());
        }
    }
    return; //just return so we know 
    
}

void vars (vector<Token>::iterator& i){
    string idInstance = ""; // to hold possible ID instance in this var set

    //  empty | declare Identifier :=  whole  ;  <vars>
    if (i->getTokenId() == DEC_TK){
        i++;
        if (i->getTokenId() == ID_TK){
            idInstance = i->getTokenInstance();
            i++;
            if (i->getTokenId() == COLNEQ_TK){
                i++;
                if(i->getTokenId() == WHL_TK){
                    i++;
                    if(i->getTokenId() == SCOLN_TK){
                        i++;
                        cout << "Vars: declare ID (" << idInstance << ") := whole ;" << endl;
                        vars(i);
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
void stats(vector<Token>::iterator& i){
    stat(i);
    //mstat(i);
}
void mstat(vector<Token>::iterator& i){
    stat(i);

}

void stat(vector<Token>::iterator& i){
    in(i);
    ifStat(i);

    if (i->getTokenId() == SCOLN_TK){
        i++;
    } else {
        printError(SCOLN_TK, i->getTokenInstance());
    }
}

void in(vector<Token>::iterator& i){
    string idInstance = "";

    if (i->getTokenId() == LST_TK){
        i++;
        if (i->getTokenId() == ID_TK){
            i++;
            idInstance = i->getTokenInstance();
            cout << "Listen Statement: listen " << idInstance << endl;
        } else {
            printError(ID_TK, i->getTokenInstance());
        }
    }
}

void ifStat(vector<Token>::iterator& i){
    if (i->getTokenId() == IF_TK){
        i++;
        if (i->getTokenId() == SCOLN_TK){
            cout << "Stat: if " << endl;
        } else {
            printError(SCOLN_TK, i->getTokenInstance());
        }
    } //else it's just not an if, don't increment the iterator yet
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

