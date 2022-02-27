#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std;

class Token {
    private:
        int tokenId;
        string tokenInstance;
        int lineNumber;
        int charNumber;
        //double lineCharNumber;
    public:
        Token(int tokenId, string tokenInstance, int lineNumber, int charNumber);
        void resetAttributes(int tokenId, string tokenInstance, int lineNumber, int charNumber);
        int getTokenId();
        string getTokenInstance();
        int getLineNumber();
        int getCharNumber();

        bool isFinal();
        bool isError();
        
};

#endif
    
