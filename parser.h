#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include "token.h"
#include "node.h"
#include "stack.h"

vector<Token> scannerUtility(string fileName);
void parser(string fileName);
void printError(int expecting, const vector<Token>::iterator i);

//BNF
Node * program  (vector<Token>::iterator& i, Stack& varStack);
Node * block    (vector<Token>::iterator& i, Stack& varStack);
Node * vars     (vector<Token>::iterator& i, Stack& varStack);
Node * expr     (vector<Token>::iterator& i, Stack& varStack);
Node * N        (vector<Token>::iterator& i, Stack& varStack);
Node * X        (vector<Token>::iterator& i, Stack& varStack);
Node * A        (vector<Token>::iterator& i, Stack& varStack);
Node * M        (vector<Token>::iterator& i, Stack& varStack);
Node * R        (vector<Token>::iterator& i, Stack& varStack);
Node * stats    (vector<Token>::iterator& i, Stack& varStack);
Node * mstat    (vector<Token>::iterator& i, Stack& varStack);
Node * stat     (vector<Token>::iterator& i, Stack& varStack);
Node * in       (vector<Token>::iterator& i, Stack& varStack);
Node * out      (vector<Token>::iterator& i, Stack& varStack);
Node * ifStat   (vector<Token>::iterator& i, Stack& varStack);
Node * loop     (vector<Token>::iterator& i, Stack& varStack);
Node * loop1    (vector<Token>::iterator& i, Stack& varStack);
Node * loop2    (vector<Token>::iterator& i, Stack& varStack);
Node * assign   (vector<Token>::iterator& i, Stack& varStack);
Node * label    (vector<Token>::iterator& i, Stack& varStack);
Node * gotoStat (vector<Token>::iterator& i, Stack& varStack);
Node * RO       (vector<Token>::iterator& i, Stack& varStack);


#endif