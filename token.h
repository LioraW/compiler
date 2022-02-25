#ifndef TOKEN_H
#define TOKEN_H

#include <string.h>
#include "tokenID.h"


struct Token {
    tokenID tokenId;
    std::string tokenInstance;
    double lineLetterNumber;
};

#endif
    
