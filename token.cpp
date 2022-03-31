#include <string>
#include <math.h>
#include <stdlib.h>
#include "token.h"
#include "tokenID.h"
using namespace std;

//constructor
Token::Token(int tokenId, string tokenInstance, int lineNumber, int charNumber) {
    this->tokenId = tokenId;
    this->tokenInstance = tokenInstance;
    this->lineNumber = lineNumber;
    this->charNumber = charNumber;
}
// setter
void Token::resetAttributes(int tokenId, string tokenInstance, int lineNumber, int charNumber) {
    this->tokenId = tokenId;
    this->tokenInstance = tokenInstance;
    this->lineNumber = lineNumber;
    this->charNumber = charNumber;
}
// Getters
int Token::getTokenId () {
    return tokenId;
}
string Token::getTokenInstance(){
    return tokenInstance;
}
string Token::getTokenDescription() {
    if (this->isFinal()){
        return tokenNames[this->getTokenId() - ID_TK]; 
    } else {
        return errorNames[abs(this->getTokenId()) - 1];
    }
    
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
