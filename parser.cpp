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

using namespace std;

void parser(string fileName){
    
    //get tokens
    vector<Token> tokens = scannerUtility(fileName);

    Node * root = nullptr;

    if (!tokens.back().isError() && !tokens.empty()){

        //start parsing
        vector<Token>::iterator i = tokens.begin();

        Stack varStack;
        
        //create parse tree
        root = program(i, varStack); 

        printPreorder(root, 0);

        destroySubTree(root); //cleanup

    } else if (!tokens.empty()){ //if it is empty there was a file error, the error was already send to stdout
        cout << "SCANNER ERROR: " << tokens.back().getTokenDescription();
        cout << " on line " << tokens.back().getLineNumber() << ", char " << tokens.back().getCharNumber()  << endl;
    }
}

void printError(string expecting, const vector<Token>::iterator token) {
    cout << "PARSER ERROR: line " << token->getLineNumber() << ", char " << token->getCharNumber() << ": ";
    cout << "Expecting " << expecting << "; Instead found " << token->getTokenInstance() << endl;
 
}

Node * program(vector<Token>::iterator& i, Stack& varStack){
    Node * p = getNode("program");
    
    varStack.pushPlaceholder();
    p->left = vars(i, varStack);

    if (i->getTokenId() == MN_TK){
        i++;
        p->right = block(i, varStack);
    } else {
        printError("main keyword", i);
        return nullptr;
    }
    varStack.popCurrentBlock();
    return p;

}
Node * block(vector<Token>::iterator& i, Stack& varStack){
    if (i->getTokenId() == LCBRC_TK){
        
        varStack.pushPlaceholder();

        Node * p = getNode("block");
        i++;
        p->middle1 = vars(i, varStack);
        p->middle2 = stats(i, varStack);

        if(i->getTokenId() == RCBRC_TK) {
            i++;
            varStack.popCurrentBlock();
            return p;
        } else {
            printError("Right bracket", i);
        }
    }
    return nullptr;
    
}

Node * vars (vector<Token>::iterator& i, Stack& varStack){
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
                        p->right = vars(i, varStack);

                        //Add the new variable to the stack
                         if (varStack.varAlreadyInScope(p->token)){
                            cout << "Multiple definition! " << p->token << " already declared in local scope. " << endl;
                            return nullptr; //error
                        }
                        varStack.push(p->token); 
                        varStack.incrementCurrentBlockVarCount();

                        return p;
                    } else {
                        printError("semicolon", i);
                    }
                } else {
                    printError("Whole keyword", i);
                }
            } else {
                printError("Colon Equals", i);
            }
        } else {
            printError("Identifier", i);
        }
    }
    return nullptr; //if it reached this point there was an error or it was empty
}

Node * expr(vector<Token>::iterator& i, Stack& varStack){
    // <expr> -> <N> - <expr>  | <N>
    Node * p = getNode("expr");
    p->left = N(i, varStack);
    if (i->getTokenId() == MINUS_TK){
        i++;
        p->token = "-";
        p->right = expr(i, varStack);
    }
    return p;

}
Node * N(vector<Token>::iterator& i, Stack& varStack) {
    // split up to aNode * left recursion
    Node * p = getNode("N");
    p->left = A(i, varStack);
    p->right = X(i, varStack);

    return p;
}

Node * X(vector<Token>::iterator& i, Stack& varStack) {
    //<X> ->  / <A><X> | +<A><X> |  empty
    if (i->getTokenId() == DIV_TK || i->getTokenId() == PLUS_TK ){
        Node * p = getNode("X");
        p->token = i->getTokenInstance();
        i++;
        p->left = A(i, varStack);
        p->right = X(i, varStack); //right recursive
        return p;
    } else {
        return nullptr; //empty
    }
}

Node * A(vector<Token>::iterator& i, Stack& varStack) {
    //A> -> <M> * <A> | <M>
    Node * p = getNode("A");
    p->left = M(i, varStack);
    if (i->getTokenId() == MULT_TK){
        i++;
        p->token = "*";
        p->right = A(i, varStack);
    }
    return p;
}

Node * M(vector<Token>::iterator& i, Stack& varStack) {
    //<M> -> % <M> |  <R>
    Node * p = getNode("M");

    if (i->getTokenId() == MOD_TK){
        i++;
        p->token = "%";
        p->right = M(i, varStack);

    } else {
        p->left = R(i, varStack);
    }
    return p;
}

Node * R(vector<Token>::iterator& i, Stack& varStack) {
   //<R> -> ( <expr> ) | Identifier | Integer
    Node * p = getNode("R");

   if (i->getTokenId() == LPRN_TK) {

       i++;
       p->middle1 = expr(i, varStack);
       if (i-> getTokenId() == RPRN_TK){
           i++;
       } else {
           printError("Right parentheses", i);
           return nullptr;
       }
   } else if (i->getTokenId() == ID_TK) {
       
       p->token = i->getTokenInstance();
       i++;
   } else if (i->getTokenId() == NUM_TK) {
       p->token = i->getTokenInstance();
       i++;
   } else {
       printError("Expression in parantheses, identifier, or number", i);
       return nullptr;
   }
   return p;
}

Node * stats(vector<Token>::iterator& i, Stack& varStack){
    //<stats> -> <stat>  <mStat>
    Node * p = getNode("stats");
    p->left = stat(i, varStack);
    p->right = mstat(i, varStack);

    return p;
}

Node * mstat(vector<Token>::iterator& i, Stack& varStack){
    //<mStat>-> <stat>  <mStat> | empty
    Node * p = getNode("mstat");

    //to allow empty
    vector<Token>::iterator previous = i;
    
    p->left = stat(i, varStack);

    //if there was a statement, check for one more. otherwise it's empty and return.
    if (previous != i){
        p-> right = mstat(i, varStack);
        return p;
    } else {
        return nullptr;
    }
}

Node * stat(vector<Token>::iterator& i, Stack& varStack){
    Node * p = getNode("stat");

    p->left = in(i, varStack);
    if (p->left == nullptr)
        p->left = out(i, varStack);
    if (p->left == nullptr)
        p->left = block(i, varStack);
    if (p->left == nullptr)
        p->left = ifStat(i, varStack);
    if (p->left == nullptr)
        p->left = loop(i, varStack);
    if (p->left == nullptr)
        p->left = assign(i, varStack);
    if (p->left == nullptr)
        p->left = label(i, varStack);
    if (p->left == nullptr)
        p->left = gotoStat(i, varStack);
    if (p->left != nullptr)
        return p;
    else
        return nullptr;
}

Node * in(vector<Token>::iterator& i, Stack& varStack){
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
                printError("semicolon", i);
                return nullptr;
            }
        } else {
            printError("identifier", i);
            return nullptr;
        }
    }
    return nullptr;
}

Node * out(vector<Token>::iterator& i, Stack& varStack){
    if (i->getTokenId() == YELL_TK){
        Node * p = getNode("out");
        i++;
        p->right = expr(i, varStack);

        //check for semicolon
        if (i->getTokenId() == SCOLN_TK){
            i++;
            return p;
        } else {
            printError("semicolon", i);
            return nullptr;
        }
    }
    return nullptr;
}

Node * ifStat(vector<Token>::iterator& i, Stack& varStack){
    
    if (i->getTokenId() == IF_TK){ //if token
        Node * p = getNode("if");
        i++;
        if (i->getTokenId() == LBRC_TK){ //left bracket
            i++;
            p->left = expr(i, varStack);
            p->middle1 = RO(i, varStack);
            p->middle2 = expr(i, varStack);

            if (i->getTokenId() == RBRC_TK){ //right bracket
                i++;
                if (i->getTokenId() == THEN_TK){ //then 
                    i++;
                    p->right = stat(i, varStack);
                    if (i->getTokenId() == SCOLN_TK){ //semicolon
                        i++;
                        return p;
                    } else {
                        printError("semicolon", i);
                        return nullptr;
                    }
                } else {
                    printError("then keyword", i);
                    return nullptr;
                }
            } else {
                printError("Right bracket", i);
                return nullptr;
            }
        } else {
            printError("Left bracket", i);
            return nullptr;
        }
    } //else it's just not an if, don't increment the iterator yet
    return nullptr;
}

Node * loop(vector<Token>::iterator& i, Stack& varStack){
    //BNF is not ll1 => extra lookahead needed
    if (i->getTokenId() == RPT_TK){
        i++; //consume repeat token
        if (i->getTokenId() == LBRC_TK){ //if there is a right bracket, it's loop1
            i++; //consume the left bracket
            return loop1(i, varStack);
        } else {
            return loop2(i, varStack); //we did not consume the next token
        }  
    } else {
        return nullptr;
    }
}

Node * loop1(vector<Token>::iterator& i, Stack& varStack){
    Node * p = getNode("loop1");
    //<loop1> -> repeat  [ <expr> <RO> <expr> ]  <stat>
    p->left = expr(i, varStack);
    p->middle1 = RO(i, varStack);
    p->middle2 = expr(i, varStack);
    if (i->getTokenId() == RBRC_TK){
        i++;
        p->right = stat(i, varStack);
    } else {
        printError("Right Bracket", i);
        return nullptr;
    }  
    //check for semicolon
    if (i->getTokenId() == SCOLN_TK){
        i++;
        return p;
    } else {
        printError("semicolon", i);
        return nullptr;
    }  

}

Node * loop2(vector<Token>::iterator& i, Stack& varStack){
    //<loop2> -> repeat <stat> until [ <expr> <RO> <expr> ] 
    Node * p = getNode("loop2");
    p->left = stat(i, varStack);
    if (i->getTokenId() == UNT_TK){
        i++;
        if (i->getTokenId() == LBRC_TK){
            i++;
            p->middle1 = expr(i, varStack);
            p->middle2 = RO(i, varStack);
            p->right = expr(i, varStack);
            if (i->getTokenId() == RBRC_TK) {
                i++;
            } else {
                printError("Right bracket", i);
                return nullptr;
            }
        } else {
            printError("Left Bracket", i);
            return nullptr;
        }
    } else {
        printError("Until keyword", i);
        return nullptr;
    }

    //check for semicolon
    if (i->getTokenId() == SCOLN_TK){
        i++;
        return p;
    } else {
        printError("semicolon", i);
        return nullptr;
    }  
}

Node * assign(vector<Token>::iterator& i, Stack& varStack){
    //<assign> -> assign Identifier  = <expr>  

    if (i->getTokenId() == ASGN_KW_TK){
        Node * p = getNode("assign");
        i++;
        if (i->getTokenId() == ID_TK) {
            p->token = i->getTokenInstance();
            i++;
            if (i->getTokenId() == ASGN_TK) {
                i++;
                p->left = expr(i, varStack);

                //check for semicolon
                if (i->getTokenId() == SCOLN_TK){
                    i++;
                    return p;
                } else {
                    printError("semicolon", i);
                    return nullptr;
                } 
            } else {
                printError("Assignemnt operator", i);
                return nullptr;
            }        
        } else {
            printError("indentifier", i);
            return nullptr;
        }
    } else {
        return nullptr;
    }
}

Node * label(vector<Token>::iterator& i, Stack& varStack){
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
                printError("semnicolon", i);
                return nullptr;
            }       
        } else {
            printError("identifier", i);
            return nullptr;
        }
    } else {
        return nullptr;
    }
}

Node * gotoStat(vector<Token>::iterator& i, Stack& varStack){
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
                printError("semicolon", i);
                return nullptr;
            }       
        } else {
            printError("identifier", i);
            return nullptr;
        }
    } else {
        return nullptr;
    }
}

Node * RO(vector<Token>::iterator& i, Stack& varStack){
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
                    printError("dot", i);
                    return nullptr;
                }
            } else {
                printError("dot", i);
                return nullptr;
            }
            break;
        default:
            printError("Relational Operator (<=, >=, ==, ..., or !=)", i);
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

