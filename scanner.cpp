#include <iostream>
#include <string>

#include "token.h"
#include "tokenID.h"
#include "Characters.h"

using namespace std;

string trim(const string str);

// Token scanner(string line){

//     for (char c: line){

//     }

// }

// void filter (string line, int lineNumber, bool openComment) {
//     line = trim(line);

//     int commentBeginNumber;
//     int commentEndNumber;

    
//     for (int i = 0; i < line.size() - 1; i++) { // go until the second to last character in the line, because we need two #
//         if (line[i] == '#' && line[i + 1] == '#'){ //check for comments
            
//             if (openComment){
//                 commentBeginNumber = 0;
//                 commentEndNumber = i + 1;
//             } else {
//                 commentBeginNumber = i + 1;
//             }

//             openComment = !openComment; //flip if the comment is closed on the same line
//         }
//     }

//     if (openComment){
//         commentEndNumber = line.size();
//     }


//     //line = line.substr(0, commentBeginNumber) + line.substr(commentEndNumber, line.size());

//     cout << line << endl;

// }

void filter (string line, int lineNumber, bool openComment){
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
    cout << newString << endl;
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