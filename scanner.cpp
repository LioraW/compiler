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

            if (nextState == ID_TK || nextState == NUM_TK) {
                if (isspace(nextChar)){
                    charNum++;
                }
                if (isKeyword(s)){
                    nextState = STR_TK; //TODO find keyword token
                }
            } else {
                s += nextChar; //keep next character and move on to the next one
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
    if (charNum >= line.length()) {
        nextState = table[currentState][FSAColumn('\n')];
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

//if the current char and the one after it is #, its a comment 
bool commentCharacter(string line, int charNum){
    return charNum < line.length() - 1 && line[charNum] == '#' && line[charNum + 1] == '#';
}

char filter (string line, int lineNumber, int& charNum, bool& openComment){

    //check for comment characters
    if (commentCharacter(line, charNum)) { 
        openComment = !openComment;
        charNum += 2;
    }

    //skip all the characters between the open comments
    while (openComment && charNum < line.length()){
        //if it comes across another comment character
        if (commentCharacter(line, charNum)){
            openComment = !openComment;
            charNum += 2;
            break;
        }
        charNum++;
    }
    
    //skip spaces
    if (charNum > 1 && commentCharacter(line, charNum - 2) && isspace(line[charNum]) || // was just a comment and now a space
        charNum > 0 && isspace(line[charNum - 1]) ||                                    // the previous char was a space
        charNum == 0) {                                                                 // first character
        while (isspace(line[charNum])){
            charNum++;
        }
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