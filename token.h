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

        string tokenNames[41] = { 
            "ID",
            "Number",
            "Equality",
            "Not Equal",
            "Greater than Equal to",
            "Less than Equal to",
            "Assignment",
            "Colon",
            "Colon Equal",
            "Plus",
            "Minus",
            "Multiply",
            "Division",
            "Modulous",
            "Dot",
            "Comma",
            "Left Parentheses",
            "Right Parentheses",
            "Left Curly Bracket",
            "Right Curly Bracket",
            "Semicolon",
            "Left Bracket",
            "Right Bracket",
            "start",
            "stop",
            "repeat", 
            "until", 
            "whole",
            "quit", 
            "label", 
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
            "End of line",
        };
        string errorNames[7] = {
            "Whitespace is not a token",
            "ID cannot start with a number or a capital letter",
            "unfished assignment (unrecongnized character after = )",
            "unfinished less than or equal to ",
            "unfinished greater than or equal to",
            "hanging !",
            "Illegal Character",
        };


    public:
        Token(int tokenId, string tokenInstance, int lineNumber, int charNumber);
        void resetAttributes(int tokenId, string tokenInstance, int lineNumber, int charNumber); //setter for all details
        
        int getTokenId();
        string getTokenInstance();
        string getTokenDescription();
        
        int getLineNumber();
        int getCharNumber();
        
        bool isFinal();
        bool isError();

        static string getTokenName(int tokenId);
        
};

#endif
    
