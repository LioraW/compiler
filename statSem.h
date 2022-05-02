#ifndef STATSEM_H
#define STATSEM_H

#include "stack.h"
#include "node.h"

enum tempType {
    L,
    V
};

void checkStaticSemantics(ofstream &file, Node * p, Stack& varstack);
string statSemError(string variableName);
bool isNumber(const string str);
bool usesIdentifier(Node * p);
string getName(tempType type);
void writeFileVarDeclarations(ofstream& file);


#endif