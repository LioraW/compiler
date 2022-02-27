#include <string>
#include <math.h>
#include "token.h"
using namespace std;

Token::Token(int tokenId, string tokenInstance, int lineNumber, int charNumber) {
    this->tokenId = tokenId;
    this->tokenInstance = tokenInstance;
    this->lineNumber = lineNumber;
    this->charNumber = charNumber;
}

void Token::resetAttributes(int tokenId, string tokenInstance, int lineNumber, int charNumber) {
    this->tokenId = tokenId;
    this->tokenInstance = tokenInstance;
    this->lineNumber = lineNumber;
    this->charNumber = charNumber;
}

int Token::getTokenId () {
    return tokenId;
}
string Token::getTokenInstance(){
    return tokenInstance;
}
int Token::getLineNumber(){
    return lineNumber;
}
int Token::getCharNumber(){
    return charNumber; 
}

bool Token::isFinal() {
    return tokenId > 1000;
}

bool Token::isError() {
    return tokenId < 0;
}