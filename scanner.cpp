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
    int startingChar = charNum; //save the first char of the token
    string s = "";
    Token token(currentState, s, lineNum, startingChar);
    bool openComment = false;
    
    char nextChar = filter(line, lineNum, charNum, openComment);
    
    while (!isFinalState(currentState) && charNum < line.length() /* && charNum < line.length() */) {
        
        nextState = table[currentState][FSAColumn(nextChar)];

        if (isFinalState(nextState)){ //if error state or FINAL state
    
            if (nextState == ID_TK && isKeyword(s)) {
                nextState = STR_TK; // TODO: find correct keyword token
            } 
            
            token.resetAttributes(nextState, s, lineNum, startingChar);
            return token;

        } else { // else its not final state
            currentState = nextState;
            s += nextChar;
            // cout << s << endl;
            charNum++;
            nextChar = filter(line, lineNum, charNum, openComment);
            
        }
    }
    if (charNum >= line.length()){
        cout << "end of line";
        token.resetAttributes(nextState, s, lineNum, startingChar);
    }
    return token;
}

bool isFinalState(int state){
    return state > 1000 || state < 0;
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
        if (charNum < line.length() - 1 && line[charNum] == '#' && line[charNum + 1] == '#'){
            openComment = !openComment;
            charNum += 2;
            break;
        }
        charNum++;
    }
   
    
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