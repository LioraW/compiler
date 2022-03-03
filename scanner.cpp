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
    
    char nextChar = line[startingChar];
    
    while (!isFinalState(currentState) && charNum < line.length()) {
        //s += nextChar;
        
        nextState = table[currentState][FSAColumn(nextChar)];

        cout << nextState << endl;

        if (isFinalState(nextState)){ //if error state or FINAL state
    
            if (nextState == ID_TK && isKeyword(s)) {
                nextState = STR_TK; // TODO: find correct keyword token
            } 

            token.resetAttributes(nextState, s, lineNum, startingChar);
            return token;

        } else { // else its not final state
            
            currentState = nextState;
            s+= nextChar;
            charNum++;

            // nextChar = charNum < line.length() ? line[charNum] : '\n';
            if (charNum < line.length() - 1){
                nextChar = line[charNum];
            }
            // s += nextChar; //having this here keeps the last token

        }
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


string filter (string line, int lineNumber, bool& openComment){
    string newString = "";
    int i = 0;
    // while(isspace(line[i])){
    //     i++;
    // }
    for ( ; i < line.length(); ++i ){
        //check for a comment starting or ending
        if (line[i] == '#' && line[i - 1] == '#') { //if the current char and the one before is #, its a comment 
            openComment = !openComment;
        }
        //if its not an open comment, append the words back to the line
        if (!openComment && line[i] != '#') {
            newString += line[i];
        }
    }
    return newString;
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