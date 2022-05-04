#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "scanner.h"
#include "tokenID.h"
#include "token.h"
#include "parser.h"
#include "tree.h"
#include "node.h"
#include "stack.h"
#include "labels.h"
#include "utils.h"

using namespace std;

Node * parser(string fileName){
    Node * root = nullptr;  //parse tree
    
    //get tokens
    vector<Token> tokens = scannerUtility(fileName);

    if (!tokens.back().isError() && !tokens.empty()){

        //start parsing
        vector<Token>::iterator i = tokens.begin();

        //create parse tree
        root = program(i); 

        //destroySubTree(root); //cleanup

    } else if (!tokens.empty()){ //if it is empty there was a file error, the error was already send to stdout
        printError(scannerError(tokens.back()));
    }
    
    return root;
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

string scannerError(Token token){
    return "SCANNER ERROR: " + token.getTokenDescription() +  " on line " + to_string(token.getLineNumber()) +
           ", char " + to_string(token.getCharNumber()) + '\n';
}

string parserError(string expecting, const vector<Token>::iterator token) {
    return "PARSER ERROR: line " + to_string(token->getLineNumber()) + ", char " + to_string(token->getCharNumber()) + ": "  + "Expecting " + expecting + "; Instead found " + token->getTokenInstance() + "\n";
}
//Start implementation of BNF:

Node * program(vector<Token>::iterator& i){
    Node * p = getNode(PROGRAM_LBL);
    
    p->left = vars(i);

    if (i->getTokenId() == MN_TK){
        i++;
        p->right = block(i);
    } else {
        printError(parserError("main keyword", i));
        return nullptr;
    }
    return p;

}
Node * block(vector<Token>::iterator& i){
    if (i->getTokenId() == LCBRC_TK){
        
        Node * p = getNode(BLOCK_LBL);
        i++;
        p->middle1 = vars(i);
        p->middle2 = stats(i);

        if(i->getTokenId() == RCBRC_TK) {
            i++;
            return p;
        } else {
            printError(parserError("Right bracket", i));
        }
    }
    return nullptr;   
}

Node * vars (vector<Token>::iterator& i){
    //  empty | declare Identifier :=  whole  ;  <vars>
    if (i->getTokenId() == DEC_TK){
        Node * p = getNode(VARS_LBL);
        i++;
        if (i->getTokenId() == ID_TK){
            p->token = i->getTokenInstance();
            i++;
            if (i->getTokenId() == COLNEQ_TK){
                i++;
                if(i->getTokenId() == WHL_TK){
                    i++;
                    if(i->getTokenId() == SCOLN_TK){
                        i++;
                        p->right = vars(i);
                        return p;
                    } else {
                        printError(parserError("semicolon", i));
                    }
                } else {
                    printError(parserError("Whole keyword", i));
                }
            } else {
                printError(parserError("Colon Equals", i));
            }
        } else {
            printError(parserError("Identifier", i));
        }
    }
    return nullptr; //if it reached this point there was an error or it was empty
}

Node * expr(vector<Token>::iterator& i){
    // <expr> -> <N> - <expr>  | <N>
    Node * p = getNode(EXPR_LBL);
    p->left = N(i);
    if (i->getTokenId() == MINUS_TK){
        i++;
        p->token = "-";
        p->right = expr(i);
    }
    return p;

}
Node * N(vector<Token>::iterator& i) {
    // split up to aNode * left recursion
    Node * p = getNode(N_LBL);
    p->left = A(i);
    p->right = X(i);

    return p;
}

Node * X(vector<Token>::iterator& i) {
    //<X> ->  / <A><X> | +<A><X> |  empty
    if (i->getTokenId() == DIV_TK || i->getTokenId() == PLUS_TK ){
        Node * p = getNode(X_LBL);
        p->token = i->getTokenInstance();
        i++;
        p->left = A(i);
        p->right = X(i); //right recursive
        return p;
    }
    return nullptr; //empty
}

Node * A(vector<Token>::iterator& i) {
    //A> -> <M> * <A> | <M>
    Node * p = getNode(A_LBL);
    p->left = M(i);
    if (i->getTokenId() == MULT_TK){
        i++;
        p->token = "*";
        p->right = A(i);
    }
    return p;
}

Node * M(vector<Token>::iterator& i) {
    //<M> -> % <M> |  <R>
    Node * p = getNode(M_LBL);

    if (i->getTokenId() == MOD_TK){
        i++;
        p->token = "%";
        p->right = M(i);

    } else {
        p->left = R(i);
    }
    return p;
}

Node * R(vector<Token>::iterator& i) {
   //<R> -> ( <expr> ) | Identifier | Integer
    Node * p = getNode(R_LBL);

   if (i->getTokenId() == LPRN_TK) {

       i++;
       p->middle1 = expr(i);
       if (i-> getTokenId() == RPRN_TK){
           i++;
       } else {
           printError(parserError("Right parentheses", i));
           return nullptr;
       }
    } else if (i->getTokenId() == NUM_TK) {
       p->token = i->getTokenInstance();
       i++;
    } else if (i->getTokenId() == ID_TK) {
        p->token = i->getTokenInstance();
        i++;
   } else {
       printError(parserError("Expression in parentheses, identifier, or number", i));
       return nullptr;
   }
   return p;
}

Node * stats(vector<Token>::iterator& i){
    //<stats> -> <stat>  <mStat>
    Node * p = getNode(STATS_LBL);
    p->left = stat(i);
    p->right = mstat(i);

    return p;
}

Node * mstat(vector<Token>::iterator& i){
    //<mStat>-> <stat>  <mStat> | empty
    Node * p = getNode(MSTATS_LBL);

    //to allow empty
    vector<Token>::iterator previous = i;
    
    p->left = stat(i);

    //if there was a statement, check for one more. otherwise it's empty and return.
    if (previous != i){
        p-> right = mstat(i);
        return p;
    } else {
        return nullptr;
    }
}

Node * stat(vector<Token>::iterator& i){
    Node * p = getNode(STAT_LBL);

    p->left = in(i);
    if (p->left == nullptr)
        p->left = out(i);
    if (p->left == nullptr)
        p->left = block(i);
    if (p->left == nullptr)
        p->left = ifStat(i);
    if (p->left == nullptr)
        p->left = loop(i);
    if (p->left == nullptr)
        p->left = assign(i);
    if (p->left == nullptr)
        p->left = label(i);
    if (p->left == nullptr)
        p->left = gotoStat(i);
    if (p->left != nullptr)
        return p;
    else
        return nullptr;
}

Node * in(vector<Token>::iterator& i){
    if (i->getTokenId() == LST_TK){
        Node * p = getNode(IN_LBL);
        i++;
        if (i->getTokenId() == ID_TK){
            p->token = i->getTokenInstance();
            i++;
            //check for semicolon
            if (i->getTokenId() == SCOLN_TK){
                i++;
                return p;
            } else {
                printError(parserError("semicolon", i));
            }
        } else {
            printError(parserError("identifier", i));
        }
    }
    return nullptr;
}

Node * out(vector<Token>::iterator& i){
    if (i->getTokenId() == YELL_TK){
        Node * p = getNode(OUT_LBL);
        i++;
        p->right = expr(i);

        //check for semicolon
        if (i->getTokenId() == SCOLN_TK){
            i++;
            return p;
        } else {
            printError(parserError("semicolon", i));
        }
    }
    return nullptr;
}

Node * ifStat(vector<Token>::iterator& i){
    
    if (i->getTokenId() == IF_TK){ //if token
        Node * p = getNode(IF_LBL);
        i++;
        if (i->getTokenId() == LBRC_TK){ //left bracket
            i++;
            p->left = expr(i);
            p->middle1 = RO(i);
            p->middle2 = expr(i);

            if (i->getTokenId() == RBRC_TK){ //right bracket
                i++;
                if (i->getTokenId() == THEN_TK){ //then 
                    i++;
                    p->right = stat(i);
                    if (i->getTokenId() == SCOLN_TK){ //semicolon
                        i++;
                        
                        return p;
                    } else {
                        printError(parserError("semicolon", i));
                    }
                } else {
                    printError(parserError("Then keyword", i));
                }
            } else {
                printError(parserError("Right bracket", i));
            }
        } else {
            printError(parserError("Left bracket", i));
        }
    } //else it's just not an if, don't increment the iterator yet
    return nullptr;
}

Node * loop(vector<Token>::iterator& i){
    //BNF is not ll1 => extra lookahead needed
    if (i->getTokenId() == RPT_TK){
        i++; //consume repeat token
        if (i->getTokenId() == LBRC_TK){ //if there is a right bracket, it's loop1
            i++; //consume the left bracket
            return loop1(i);
        } else {
            return loop2(i); //we did not consume the next token
        }  
    } else {
        return nullptr;
    }
}

Node * loop1(vector<Token>::iterator& i){
    Node * p = getNode(LOOP1_LBL);
    //<loop1> -> repeat  [ <expr> <RO> <expr> ]  <stat>
    p->left = expr(i);
    p->middle1 = RO(i);
    p->middle2 = expr(i);
    if (i->getTokenId() == RBRC_TK){
        i++;
        p->right = stat(i);
    } else {
        printError(parserError("Right Bracket", i));
        return nullptr;
    }  
    //check for semicolon
    if (i->getTokenId() == SCOLN_TK){
        i++;
        return p;
    } else {
        printError(parserError("semicolon", i));
        return nullptr;
    }  
}

Node * loop2(vector<Token>::iterator& i){
    //<loop2> -> repeat <stat> until [ <expr> <RO> <expr> ] 
    Node * p = getNode(LOOP2_LBL);
    p->left = stat(i);
    if (i->getTokenId() == UNT_TK){
        i++;
        if (i->getTokenId() == LBRC_TK){
            i++;
            p->middle1 = expr(i);
            p->middle2 = RO(i);
            p->right = expr(i);
            if (i->getTokenId() == RBRC_TK) {
                i++;
            } else {
                printError(parserError("Right bracket", i));
                return nullptr;
            }
        } else {
            printError(parserError("Left Bracket", i));
            return nullptr;
        }
    } else {
        printError(parserError("Until keyword", i));
        return nullptr;
    }

    //check for semicolon
    if (i->getTokenId() == SCOLN_TK){
        i++;
        return p;
    } else {
        printError(parserError("semicolon", i));
        return nullptr;
    }  
}

Node * assign(vector<Token>::iterator& i){
    //<assign> -> assign Identifier  = <expr>  

    if (i->getTokenId() == ASGN_KW_TK){
        Node * p = getNode(ASSIGN_LBL);
        i++;
        if (i->getTokenId() == ID_TK) {
            p->token = i->getTokenInstance();
            i++;        
            if (i->getTokenId() == ASGN_TK) {
                i++;
                p->left = expr(i);
                //check for semicolon
                if (i->getTokenId() == SCOLN_TK){
                    i++;
                    return p;
                } else {
                    printError(parserError("semicolon", i));
                } 
            } else {
                printError(parserError("Assignemnt operator", i));
            }        
        } else {
            printError(parserError("indentifier", i));
        }
    }
    return nullptr;
}

Node * label(vector<Token>::iterator& i){
    // <label> -> label Identifier

    if (i->getTokenId() == LBL_TK) {
        Node * p = getNode(LABEL_LBL);
        i++;
        if (i->getTokenId() == ID_TK){
            p->token = i->getTokenInstance();
            i++;
            
            //check for semicolon
            if (i->getTokenId() == SCOLN_TK){
                i++;
                return p;
            } else {
                printError(parserError("semnicolon", i));
            }       
        } else {
            printError(parserError("identifier", i));
        }
    }
    return nullptr;
}

Node * gotoStat(vector<Token>::iterator& i){
    if (i->getTokenId() == PTL_TK) {
        Node * p = getNode(GOTO_LBL);
        i++;
        if (i->getTokenId() == ID_TK){
            p->token = i->getTokenInstance();
            i++;
            //check for semicolon
            if (i->getTokenId() == SCOLN_TK){
                i++;
                return p;
            } else {
                printError(parserError("semicolon", i));
            }       
        } else {
            printError(parserError("identifier", i));
        }
    }
    return nullptr;
}

Node * RO(vector<Token>::iterator& i){
    Node * p = getNode(RO_LBL);
    switch(i->getTokenId())
    {
        case LTE_TK:
        case GRTE_TK:
        case EQ_TK:
        case NTEQ_TK:
            p->token = i->getTokenInstance();
            i++;
            break;
        case DOT_TK:
            i++;
            if (i->getTokenId() == DOT_TK) {
                i++;
                if (i->getTokenId() == DOT_TK){
                    p->token = "...";
                    i++;
                } else {
                    printError(parserError("dot", i));
                    return nullptr;
                }
            } else {
                printError(parserError("dot", i));
                return nullptr;
            }
            break;
        default:
            printError(parserError("Relational Operator (<=, >=, ==, ..., or !=)", i));
            return nullptr;
            break;
    }
    return p;
}