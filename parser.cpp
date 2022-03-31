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
    
    //get tokens
    vector<Token> tokens = scannerUtility(fileName);
    
    if (!tokens.back().isError()){

        //bandaid for right now
        Token endToken = Token(EOF_TK, "EOF", 0, 0);
        tokens.push_back(endToken);

        //start parsing
        vector<Token>::iterator i = tokens.begin();
        program(i); 

    } else {
        cout << "SCANNER ERROR: " << tokens.back().getErrorName() << endl;
    }
    
}

void printError(int expecting, string actual){
    //TODO it needs to expect any number of things, so it should use a spread operator or an array?
    // solution: change int expecting to a string and voila
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

void expr(vector<Token>::iterator& i){
    // <expr> -> <N> - <expr>  | <N>
    cout << "Expression:";
    N(i);
    if (i->getTokenId() == MINUS_TK){
        i++;
        cout << "subtraction expression" << endl;
        expr(i);

    } else {
        return; //does not need another expression
    }

}
void N(vector<Token>::iterator& i) {
    // split up to avoid left recursion
    cout << " N ";
    A(i);
    X(i);
}

void X(vector<Token>::iterator& i) {
    cout << " X ";
    //<X> ->  / <A><X> | +<A><X> |  empty

    if (i->getTokenId() == DIV_TK){
        i++;
        cout << "dividng Expression" << endl;
        A(i);
        X(i); //right recursive
    } else if (i->getTokenId() == PLUS_TK) {
        i++;
        cout << "Adding expression" << endl;
        A(i);
        X(i);
    } else {
        return; //empty
    }
}
void A(vector<Token>::iterator& i) {
    cout << " A ";
    //A> -> <M> * <A> | <M>
    M(i);
    if (i->getTokenId() == MULT_TK){
        i++;
        cout << "Multiplication expression" << endl;
        A(i);

    } else {
        return; //does not need another peice
    }
}
void M(vector<Token>::iterator& i) {
    //<M> -> % <M> |  <R>
     cout << " M ";
    if (i->getTokenId() == MOD_TK){
        i++;
        cout << "Modulus expression" << endl;
        M(i);

    } else {
        R(i); //does not need another peice
    }
}

void R(vector<Token>::iterator& i) {
   //<R> -> ( <expr> ) | Identifier | Integer
    cout << " R ";
   if (i->getTokenId() == LPRN_TK) {
       i++;
       expr(i);
       if (i-> getTokenId() == RPRN_TK){
           i++;
           cout << "R expression" << endl;
       } else {
           printError(RPRN_TK, i->getTokenInstance());
       }
   } else if (i->getTokenId() == ID_TK) {
       i++;
       cout << "R is ID" << endl; //termial
   } else if (i->getTokenId() == NUM_TK) {
       i++;
       cout << "R is number" << endl;
   } else {
       cout << "R is wacky: instead we found"  << i->getTokenId() << endl;
       printError(LPRN_TK, i->getTokenInstance()); //TODO it needs to know to expect one of three things
   }
}

void stats(vector<Token>::iterator& i){
    stat(i);
    //mstat(i);
}
void mstat(vector<Token>::iterator& i){
    //how to allow empty?
    stat(i);
    mstat(i);
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
            idInstance = i->getTokenInstance();
            i++;
            cout << "Listen Statement: listen " << idInstance << endl;
        } else {
            printError(ID_TK, i->getTokenInstance());
        }
    }
}

void out(vector<Token>::iterator& i){
    if (i->getTokenId() == YELL_TK){
        i++;
        cout << "YEll" << endl;
        //need to check for an expression
    }
}

void ifStat(vector<Token>::iterator& i){
    if (i->getTokenId() == IF_TK){
        i++;
        cout << "if ..." << endl;
        if (i->getTokenId() == LBRC_TK){
            i++;
            expr(i);
            RO(i);
            expr(i);
            if (i->getTokenId() == RBRC_TK){
                i++;
                if (i->getTokenId() == THEN_TK){
                    i++;
                    stat(i);
                } else {
                    printError(THEN_TK, i->getTokenInstance());
                }
            } else {
                printError(RBRC_TK, i->getTokenInstance());
            }
        } else {
            printError(LBRC_TK, i->getTokenInstance());
        }

        // if (i->getTokenId() == SCOLN_TK){
        //     cout << "Stat: if " << endl;
        // } else {
        //     printError(SCOLN_TK, i->getTokenInstance());
        // }
    } //else it's just not an if, don't increment the iterator yet
}

void RO(vector<Token>::iterator& i){
    if (i->getTokenId() == LTE_TK) {
        i++;
        cout << "Less than or Equal to" << endl;
    } else if (i->getTokenId() == GRTE_TK) {
        i++;
        cout << "Greater than or equal to" << endl;
    } else if (i->getTokenId() == EQ_TK) {
        i++;
        cout << "Equality" << endl;

    } else if (i->getTokenId() == DOT_TK){
        i++;
        if (i->getTokenId() == DOT_TK) {
            i++;
            if (i->getTokenId() == DOT_TK){
                i++;
                cout << "Dot dot dot token" << endl;
            } else {
                printError(DOT_TK, i->getTokenInstance());
            }
        } else {
            printError(DOT_TK, i->getTokenInstance());
        }
    } else if (i->getTokenId() == NTEQ_TK){
        i++;
        cout << "not equal" << endl;
    } else {
        printError(LTE_ERR, i->getTokenInstance()); // should just be relational operator
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

