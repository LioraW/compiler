#include <iostream>
#include <string>

#include "token.h"
#include "tokenID.h"
#include "Characters.h"
#include "scanner.h"

using namespace std;

// Token scanner(string line){

//     for (char c: line){

//     }

// }


string filter (string line, int lineNumber, bool& openComment){
    line = trim(line);
    string newString = "";
    for (int i = 0; i < line.size(); ++i ){
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

//trim whitespace from both ends of a string
string trim(const string str) {
    // from https://stackoverflow.com/questions/25829143/trim-whitespace-from-a-string
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
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