#include <iostream>
#include <string>

#include "token.h"
#include "tokenID.h"
#include "Characters.h"
#include "scanner.h"
#include "FSAtable.h"

using namespace std;

Token scanner(string line, int lineNum, int& charNum, int lineLength){
    int currentState = s1;
    int nextState;
    int startingChar = charNum; //save the position of the first char of the token
    
    string s = "";
    Token token(currentState, s, lineNum, startingChar);
    bool openComment = false;
    
    char nextChar = filter(line, lineNum, charNum, openComment);
    
    while (!isFinalState(currentState) &&  charNum < line.length()) { //while we havent found the end of a token or the end of the line yet
        
        nextState = table[currentState][FSAColumn(nextChar)]; 

        if (isFinalState(nextState)){
    
            if (nextState == ID_TK && isKeyword(s)) {
                nextState = STR_TK; // TODO: find  keyword token 
            
            } else if (isDoubleOperator(nextState)){
                //keep next character and move on to the next one
                s += nextChar; 
                charNum++; 
            }
            
            token.resetAttributes(nextState, s, lineNum, startingChar);
            return token;

        } else { // else its not final state
            currentState = nextState;
            s += nextChar;
            
            charNum++;
            nextChar = filter(line, lineNum, charNum, openComment);
            
        }
    }

    //deal with the last token on the line
    if (charNum >= line.length()){
        nextState = table[currentState][FSAColumn('\n')];
        token.resetAttributes(nextState, s, lineNum, startingChar);
    }

    return token;
}

bool isFinalState(int state){
    return state > 1000 || state < 0;
}
bool isDoubleOperator(int state){
    int doubleOperators[] = {
        EQ_TK,   // == 
        NTEQ_TK, // != 
        GRTE_TK, // >=
        LTE_TK,  // <= 
        COLNEQ_TK, // :=
    };
    for (int op: doubleOperators){
        if (state == op)
            return true;
    }
    return false;
}

bool isKeyword(string word) {

    string keywords[] = {
    "start",
    "stop",
    "repeat", 
    "until", 
    "whole", 
    "label", 
    "quit", 
    "listen", 
    "yell", 
    "main", 
    "portal", 
    "if", 
    "then", 
    "pick", 
    "declare", 
    "assign",
    "proc",
};

    for (string keyword: keywords) {
        if (word == keyword)
            return true;
    }
    return false;
}

char filter (string line, int lineNumber, int& charNum, bool& openComment){

    //skip spaces
    while (isspace(line[charNum])){
        charNum++;
    }

    //see if there are comments
    if (charNum < line.length() - 1 && line[charNum] == '#' && line[charNum + 1] == '#') { //if the current char and the one before is #, its a comment 
            openComment = !openComment;
            charNum += 2;
    }
    //skip all the characters between the open comments
    while (openComment && charNum < line.length()){
        if (charNum < line.length() - 2 && line[charNum] == '#' && line[charNum + 1] == '#'){
            openComment = !openComment;
            charNum += 2;
            break;
        }
        charNum++;
    }
    cout << line[charNum] << endl;
    return line[charNum];
}

int FSAColumn(char ch) {
    switch(ch) {
        case '_':
            return underscore;
        case '=':
            return equals;
        case '<':
            return lessThan;
        case '>':
            return greaterThan;
        case '!':
            return bang;
        case ':':
            return colon;
        case '(':
            return leftParen;
        case ')':
            return rightParen;
        case '[':
            return leftBracket;
        case ']':
            return rightBracket;
        case '{':
            return leftCurlyBracket;
        case '}':
            return rightCurlyBracket;
        case ',':
            return comma;
        case '.':
            return dot;
        case '+':
            return add;
        case '-':
            return subtract;
        case '*':
            return multiply;
        case '/':
            return divide;
        case '%':
            return mod;
        case ';':
            return semiColon;
        case '\n':
            return newLine;
        default:
            if ('a' <= ch && 'z' >= ch){
                return lowercaseLetter;
            } else if ('A' <= ch && 'Z'>= ch) {
                return upperCaseLetter;
            } else if ( isdigit(ch) ) {
                return number;
            } else if ( isspace(ch) ) {
                return whiteSpace;
            } else {
                return illegalCharacter;
            }
    }
   
}