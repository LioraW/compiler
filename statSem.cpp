#include <iostream>
#include <fstream>

#include "node.h"
#include "tree.h"
#include "labels.h"
#include "stack.h"
#include "utils.h"
#include "statSem.h"

using namespace std;

void checkStaticSemantics(ofstream& file, Node * p, Stack& varStack){
    if (p) {
        //local scoping  
        if (p->label == PROGRAM_LBL  || p->label == BLOCK_LBL ){
            varStack.pushPlaceholder();
            file << "PUSH" << endl; //correpsonding spot in the virtual stack for the placeholder
        }

        //multiple definiton check
        if (p->label == VARS_LBL) {
            if (varStack.varDeclaredInLocalScope(p->token)){
                printError("Multiple definition! " + p->token + " already declared in this local scope.\n");
                return;
            } else {
                varStack.push(p->token);
                file << "PUSH" << endl; //just allocates memory for the variable, does not store anything 
                varStack.incrementCurrentBlockVarCount();
            }
        }

        //variable defintion check
        if (usesIdentifier(p) && varStack.find(p->token) == -1){
            printError(statSemError(p->token));
            return;
        }

        //Code Generation
        string v = "";
        string argR = ""; 
        string argL = "";
        string label = "";
        string label2 = "";
        int stackPosition = -1; //initialize it for error conditions

        switch(p->label){
            case EXPR_LBL:
                //evaulate and store LHS
                checkStaticSemantics(file, p->left, varStack);
                argL = getName(V);
                file << "STORE " << argL << endl; 
                
                if (p->token == "-"){
                    //evaulate and store RHS
                    checkStaticSemantics(file, p->right, varStack);
                    argR = getName(V);
                    file << "STORE " << argR << endl;
                    file << "LOAD " << argL << endl;
                    file << "SUB " << argR << endl; //would store result in acc
                }
                // else the result of the expression is just the LHS, which is already in ACC
                break;
            case N_LBL:
                checkStaticSemantics(file, p->left, varStack);
                checkStaticSemantics(file, p->right, varStack);
                break;
            case X_LBL:
                checkStaticSemantics(file, p->left, varStack);
                checkStaticSemantics(file, p->right, varStack);

                if (p->token == "/"){
                    file << "DIV " << argR << endl; //would store result in acc
                } else if (p->token == "+"){
                    file << "ADD " << argR << endl; //would store result in acc
                }
                break;
            case A_LBL:
                //evaulate and store LHS
                checkStaticSemantics(file, p->left, varStack);
                argL = getName(V);
                file << "STORE " << argL << endl; 
                if (p->token == "*"){
                    checkStaticSemantics(file, p->right, varStack);
                    file << "MULT " << argL << endl;
                }
                break;
            case M_LBL:
                if (p->token == "%"){
                    checkStaticSemantics(file, p->right, varStack);
                    v = getName(V);
                    file << "STORE " << v << endl; //store the result in a temporary variable
                    file << "SUB " << v << endl;
                    file << "SUB " << v << endl; 
                } else {
                    checkStaticSemantics(file, p->left, varStack);
                }
                break;
            case R_LBL:
                if (p->middle1){
                    checkStaticSemantics(file, p->middle1, varStack);

                } else if (isNumber(p->token)){
                        file << "LOAD " << p->token << endl;
                } else {
                    stackPosition = varStack.find(p->token);
                    if (stackPosition != -1){
                        file << "STACKR " << stackPosition << endl;
                    } 
                }
                break;
            case IN_LBL:
                stackPosition = varStack.find(p->token);
                if (stackPosition != -1) {
                    argL = getName(V);
                    file << "READ " << argL << endl;
                    file << "LOAD " << argL << endl;
                    file << "STACKW " << stackPosition << endl; //store the value
                }
                break;
            case OUT_LBL:
                v = getName(V);
                file << "STORE " << v << endl; //assuming expr stored the result in the accumuator
                file << "WRITE " << v << endl;
                break;
            case IF_LBL:
                //evaluate and store RHS
                checkStaticSemantics(file, p->middle2, varStack);
                argR = getName(V);
                file << "STORE " << argR << endl; 

                //evaluate and store LHS
                checkStaticSemantics(file, p->left, varStack); //lhs, will store result in acc
                argL = getName(V); 
                file << "STORE " << argL << endl; //store result of ACC 

                //evaluate for most arguments (except ...)
                file << "SUB " << argR << endl; // lhs - rhs => ACC
                label = getName(L); 
                label2 = getName(L);

                //Relational Operators
                if (p->middle1->token == "=="){ //acc pos or neg (ie not zero) ? false : true 
                    file << "BRNEG " << label << endl; 
                    file << "BRPOS " << label << endl;
                } else if (p->middle1->token == "<=") { //acc is neg or zero ? true : false
                    file << "BRPOS " << label << endl;
                } else if (p->middle1->token == ">="){ //acc is pos or zero ? true : false
                    file << "BRNEG " << label << endl;
                } else if (p->middle1->token == "!="){ //acc is zero ? true : false
                    file << "BRZERO " << label << endl;
                } else if (p->middle1->token == "..."){ //needs special evaluation. '...' means two sides have different signs
                    file << "LOAD " << argR << endl;
                    file << "MULT " << argL << endl;
                    file << "BRPOS " << label << endl; //if the product is positve they had the same sign. skip the block.
                    file << "BRNEG " << label2 << endl; //if the product is negative, they have different signs. go to block.
                    
                    //PAST THIS POINT the product was zero. Deal with that.
                    // Therefore, they have the same sign if:

                    //1. either of them is positive.
                    file << "LOAD  " << argL << endl;
                    file << "BRPOS " << label << endl;
                    file << "LOAD " << argR << endl;
                    file << "BRPOS " << label << endl;
                    
                    //2. they are both zero.
                    file << "LOAD " << argL << endl;
                    file << "ADD " << argR << endl;//this works because we know their product is zero at this point.
                    file << "BRZERO " << label << endl; 

                }

                //dependant statements
                if (p->right){
                    cout << "There was a right" << endl;
                }

                file << label2 << ": NOOP " << endl; //then 
                checkStaticSemantics(file, p->right, varStack); 
                file << label << ": NOOP" << endl; //end if

                break;
            case LOOP1_LBL:

                break;
            case LOOP2_LBL:

                break;
            case ASSIGN_LBL:
                checkStaticSemantics(file, p->left, varStack); //leaves result in accumulator
                stackPosition = varStack.find(p->token);
                if (stackPosition != -1){
                    file << "STACKW " << stackPosition << endl; //store the value of the variable in the virtual stack in the same positon as the corresponding name in the varStack
                }
                break;
            default:
                //continue traversal
                checkStaticSemantics(file, p->left, varStack); 
                checkStaticSemantics(file, p->middle1, varStack);
                checkStaticSemantics(file, p->middle2, varStack);
                checkStaticSemantics(file, p->right, varStack);
                break;
        }

        

        //local scoping 
        if (p->label == PROGRAM_LBL  || p->label == BLOCK_LBL){
            int varCount = varStack.getCurrentBlockVarCount();
            for (int i = 0; i < varCount; i++){
                file << "POP" << endl;
            }
            varStack.popCurrentBlock();
            file << "POP" << endl; //pop the placeholder
        }
	}
}
void writeFileVarDeclarations(ofstream& file){
    int totalVarCount  = (int) getName(V)[1];
    file << "STOP" << endl;
    for (int i = 0; i < totalVarCount; i++){
        file << "V" << i << " 0" << endl;
    }
}

string statSemError(string variableName){
    return "ERROR: variable \'" + variableName + "\' was used without being declared in this scope.\n";
}

bool isNumber(const string str){
    for (const char c: str){
        if (isdigit(c) == 0)
            return false;
    }
    return true;
}

bool usesIdentifier(Node * p){
    Label label = p->label;
    return label == IN_LBL || label == ASSIGN_LBL || label == LABEL_LBL || label == GOTO_LBL|| (label == R_LBL && !isNumber(p->token));
}

string getName(tempType type){ //numVars is to get the static variable 
    static int labelCount = 0;
    static int varCount = 0;
    string result = "";

    //need to "declare " all the variables after the STOP directive

    if (type == L){ //label
        result = "L" + to_string(labelCount);
        labelCount++;
    } else if (type == V){
        result = "V" + to_string(varCount);
        varCount++;
    } else {
        result  = "ERROR";
    }
    
    return result;
}