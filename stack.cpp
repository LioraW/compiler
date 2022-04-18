#include<iostream>
#include <string>

#include "token.h"
#include "stack.h"

using namespace std;

Stack::Stack() {
    top = nullptr;
    size = 0;
}

Stack::~Stack() {
    StackNode *nodePtr = nullptr;
    StackNode *nextNode = nullptr;

    nodePtr = top;

    //transverse the list and delete each node
    while (nodePtr != nullptr) {
        nextNode = nodePtr->next;
        delete nodePtr;
        nodePtr = nextNode;
    }
}

void Stack::push(string token) {
    StackNode *newNode = nullptr;

    if (size < 100){
        //Allocate a new node and store the token there
        newNode = new StackNode;
        newNode->token = token;
        newNode->count = 0;

        if (isEmpty()) {
            top  = newNode;
            newNode->next = nullptr;
            size = 0;
        } else {
            newNode->next = top;
            top = newNode;
            size++;
        }
    } else {
        cout << "Stackoverflow Error!" << endl;
    }
}

void Stack::pop() {
    StackNode *temp = nullptr;
    
    if (!isEmpty()) {
        temp = top->next;
        delete top;
        top = temp;
    }
}

int Stack::find(string str) {
    StackNode *nodePtr = nullptr;
    nodePtr = top;
    int count = 0;

    //transverse the list, looking for str
    while (nodePtr != nullptr ) {
        if (nodePtr->token == str) { 
            return count;
        }
        count++;
        nodePtr = nodePtr->next;
    }
    return -1; // if we got to this point it did not find the string
}

int Stack::getCurrentBlockVarCount(){
    StackNode *nodePtr = nullptr;
    nodePtr = top;

    //transverse the list, looking for the current Block's placeholder
    while (nodePtr != nullptr) {
        if (nodePtr->token == "Block") { //variables cannot start with a capital letter so this is acceptable as a place holder
            return nodePtr->count;
        }
        nodePtr = nodePtr->next;
    }
    return -1; // if we got to this point there was no first block (which is an error)
}

void Stack::incrementCurrentBlockVarCount(){
    StackNode *nodePtr = nullptr;
    nodePtr = top;

    //transverse the list, looking for the current Block's placeholder
    while (nodePtr != nullptr) {
        if (nodePtr->token == "Block") { //variables cannot start with a capital letter so this is acceptable as a place holder
            nodePtr->count++;
            return;
        }
        nodePtr = nodePtr->next;
    }
}

bool Stack::varDeclaredInLocalScope(string str){
    //if find() is either -1 (name not at all there) or its position is in the range of allowed varaibles, its
    return getCurrentBlockVarCount() > find(str) && find(str) > -1; 
}

void Stack::pushPlaceholder(){
    push("Block");
}

void Stack::popCurrentBlock(){
   for (int i = 0; i < getCurrentBlockVarCount(); i++){
       pop();
   }
   pop(); //pop the block placeholder off too
}

//returns true if stack is empty and false otherwise
bool Stack::isEmpty() {
    return !top;
}




