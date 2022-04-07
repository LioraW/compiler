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

using namespace std;

void parser(string fileName){
    
    //get tokens
    vector<Token> tokens = scannerUtility(fileName);

    Node * root = nullptr;

    if (!tokens.back().isError()){

        //bandaid for right now
        Token endToken = Token(EOF_TK, "EOF", 0, 0);
        tokens.push_back(endToken);

        //start parsing
        vector<Token>::iterator i = tokens.begin();
        
        //create parse tree
        root = program(i); 

        printPreorder(root, 0);

    } else {
        cout << "SCANNER ERROR: " << tokens.back().getTokenDescription() << endl;
    }
}

void printError(string expecting, string actual){
    cout << "Expecting " << expecting << "; Instead found " << actual << endl;
}

Node * program(vector<Token>::iterator& i){
    Node * p = getNode("program");
    
    p->left = vars(i);

    if (i->getTokenId() == MN_TK){
        i++;
        p->right = block(i);
    } else {
        printError("main keyword", i->getTokenInstance());
        return nullptr;
    }
    return p;

}
Node * block(vector<Token>::iterator& i){
    if (i->getTokenId() == LCBRC_TK){
        Node * p = getNode("block");
        i++;
        p->middle1 = vars(i);
        p->middle2 = stats(i);

        if(i->getTokenId() == RCBRC_TK) {
            i++;
            return p;
            if (i->getTokenId() == EOF_TK){ //TODO
                return p;
            }
        } else {
            
            printError("Right bracket after block", i->getTokenInstance());
            return nullptr;
        }
    }
    return nullptr; //just return so we know 
    
}

Node * vars (vector<Token>::iterator& i){

    //  empty | declare Identifier :=  whole  ;  <vars>
    if (i->getTokenId() == DEC_TK){
        Node * p = getNode("vars");
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
                        printError("semicolon", i->getTokenInstance());
                        return nullptr;
                    }
                } else { //error conditions
                    printError("Whole keyword", i->getTokenInstance());
                    return nullptr;
                }

            } else {
                printError("Colon Equals", i->getTokenInstance());
                return nullptr;
            }
        } else {
            printError("Identifier", i->getTokenInstance());
            return nullptr;
        }
    } else {
        return nullptr; //for empty
    }
}

Node * expr(vector<Token>::iterator& i){
    // <expr> -> <N> - <expr>  | <N>
    Node * p = getNode("expression");
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
    Node * p = getNode("N");
    p->left = A(i);
    p->right = X(i);

    return p;
}

Node * X(vector<Token>::iterator& i) {
    //<X> ->  / <A><X> | +<A><X> |  empty
    Node * p = getNode("P");

    if (i->getTokenId() == DIV_TK){
        i++;
        p->token = "/";
        p->left = A(i);
        p->right = X(i); //right recursive
    } else if (i->getTokenId() == PLUS_TK) {
        i++;
        p->token = "+";
        p->left = A(i);
        p->right = X(i);
    } else {
        return nullptr; //empty
    }
    return p;
}

Node * A(vector<Token>::iterator& i) {
    //A> -> <M> * <A> | <M>
    Node * p = getNode("A");
    p->left = M(i);
    if (i->getTokenId() == MULT_TK){
        i++;
        p->token = "*";
        p->right = A(i);

    } else {
        return nullptr; //does not need another piece
    }
    return p;
}

Node * M(vector<Token>::iterator& i) {
    //<M> -> % <M> |  <R>
    Node * p = getNode("M");

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
    Node * p = getNode("R");

   if (i->getTokenId() == LPRN_TK) {
       i++;
       p->middle1 = expr(i);
       if (i-> getTokenId() == RPRN_TK){
           i++;
       } else {
           printError("Right parentheses", i->getTokenInstance());
           return nullptr;
       }
   } else if (i->getTokenId() == ID_TK) {
       i++;
       p->token = i->getTokenInstance();
   } else if (i->getTokenId() == NUM_TK) {
       i++;
       p->token = i->getTokenInstance();
   } else {
       printError("Expression in parantheses, identifier, or number", i->getTokenInstance());
       return nullptr;
   }
   return p;
}

Node * stats(vector<Token>::iterator& i){
    //<stats> -> <stat>  <mStat>
    Node * p = getNode("stats");
    p->left = stat(i);
    p->right = mstat(i);

    return p;
}

Node * mstat(vector<Token>::iterator& i){
    //<mStat>-> <stat>  <mStat> | empty
    Node * p = getNode("mstat");

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
    Node * p = getNode("stat");

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
        Node * p = getNode("in");
        i++;
        if (i->getTokenId() == ID_TK){
            p->token = i->getTokenInstance();
            i++;
     
            //check for semicolon
            if (i->getTokenId() == SCOLN_TK){
                i++;
                return p;
            } else {
                printError("semicolon", i->getTokenInstance());
                return nullptr;
            }
        } else {
            printError("identifier", i->getTokenInstance());
            return nullptr;
        }
    }
    return nullptr;
}

Node * out(vector<Token>::iterator& i){
    if (i->getTokenId() == YELL_TK){
        Node * p = getNode("out");
        i++;
        p->right = expr(i);

        //check for semicolon
        if (i->getTokenId() == SCOLN_TK){
            i++;
            return p;
        } else {
            printError("semicolon", i->getTokenInstance());
            return nullptr;
        }
    }
    return nullptr;
}

Node * ifStat(vector<Token>::iterator& i){
    
    if (i->getTokenId() == IF_TK){ //if token
        Node * p = getNode("if");
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
                        printError("semicolon", i->getTokenInstance());
                        return nullptr;
                    }
                } else {
                    printError("then keyword", i->getTokenInstance());
                    return nullptr;
                }
            } else {
                printError("Right bracket", i->getTokenInstance());
                return nullptr;
            }
        } else {
            printError("Left bracket", i->getTokenInstance());
            return nullptr;
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
    Node * p = getNode("loop1");
    //<loop1> -> repeat  [ <expr> <RO> <expr> ]  <stat>
    p->left = expr(i);
    p->middle1 = RO(i);
    p->middle2 = expr(i);
    if (i->getTokenId() == RBRC_TK){
        i++;
        p->right = stat(i);
    } else {
        printError("Right Bracket", i->getTokenInstance());
        return nullptr;
    }  
    //check for semicolon
    if (i->getTokenId() == SCOLN_TK){
        i++;
        return p;
    } else {
        printError("semicolon", i->getTokenInstance());
        return nullptr;
    }  

}

Node * loop2(vector<Token>::iterator& i){
    //<loop2> -> repeat <stat> until [ <expr> <RO> <expr> ] 
    Node * p = getNode("loop2");
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
                printError("Right bracket", i->getTokenInstance());
                return nullptr;
            }
        } else {
            printError("Left Bracket", i->getTokenInstance());
            return nullptr;
        }
    } else {
        printError("Until keyword", i->getTokenInstance());
        return nullptr;
    }

    //check for semicolon
    if (i->getTokenId() == SCOLN_TK){
        i++;
        return p;
    } else {
        printError("semicolon", i->getTokenInstance());
        return nullptr;
    }  
}

Node * assign(vector<Token>::iterator& i){
    //<assign> -> assign Identifier  = <expr>  

    if (i->getTokenId() == ASGN_KW_TK){
        Node * p = getNode("assign");
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
                    printError("semicolon", i->getTokenInstance());
                    return nullptr;
                } 
            } else {
                printError("Assignemnt operator", i->getTokenInstance());
                return nullptr;
            }        
        } else {
            printError("indentifier", i->getTokenInstance());
            return nullptr;
        }
    } else {
        return nullptr;
    }
}

Node * label(vector<Token>::iterator& i){
    // <label> -> label Identifier

    if (i->getTokenId() == LBL_TK) {
        Node * p = getNode("label");
        i++;
        if (i->getTokenId() == ID_TK){
            p->token = i->getTokenInstance();
            i++;
            //check for semicolon
            if (i->getTokenId() == SCOLN_TK){
                i++;
                return p;
            } else {
                printError("semnicolon", i->getTokenInstance());
                return nullptr;
            }       
        } else {
            printError("identifier", i->getTokenInstance());
            return nullptr;
        }
    } else {
        return nullptr;
    }
}

Node * gotoStat(vector<Token>::iterator& i){
    if (i->getTokenId() == PTL_TK) {
        Node * p = getNode("goto");
        i++;
        if (i->getTokenId() == ID_TK){
            p->token = i->getTokenInstance();
            i++;
            //check for semicolon
            if (i->getTokenId() == SCOLN_TK){
                i++;
                return p;
            } else {
                printError("semicolon", i->getTokenInstance());
                return nullptr;
            }       
        } else {
            printError("identifier", i->getTokenInstance());
            return nullptr;
        }
    } else {
        return nullptr;
    }
}

Node * RO(vector<Token>::iterator& i){
    Node * p = getNode("RO");
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
                    printError("dot", i->getTokenInstance());
                    return nullptr;
                }
            } else {
                printError("dot", i->getTokenInstance());
                return nullptr;
            }
            break;
        default:
            printError("Relational Operator (<=, >=, ==, ..., or !=)", i->getTokenInstance());
            return nullptr;
            break;
    }
    return p;
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

