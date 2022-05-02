#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <string>
#include "token.h"

using namespace std;

class Stack {
    private:
        struct StackNode {
            string token;
            int count;
            StackNode * next;
        };
        
        StackNode * top;
        int size;
    
    public:
        Stack();
        ~Stack();
        void push(string token);
        void pop();
        int  find(string str);
        int  getCurrentBlockVarCount();
        void incrementCurrentBlockVarCount();
        bool varDeclaredInLocalScope(string str);
        void pushPlaceholder();
        void popCurrentBlock(); 
        bool isEmpty();
};

#endif

