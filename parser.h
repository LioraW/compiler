#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include "token.h"
#include "node.h"

vector<Token> scannerUtility(string fileName);
void parser(string fileName);
void printError(int expecting, string actual);

//BNF
Node * program(vector<Token>::iterator& i);
Node * block(vector<Token>::iterator& i);
Node * vars(vector<Token>::iterator& i);
Node * expr(vector<Token>::iterator& i);
Node * N(vector<Token>::iterator& i);
Node * X(vector<Token>::iterator& i);
Node * A(vector<Token>::iterator& i);
Node * M(vector<Token>::iterator& i);
Node * R(vector<Token>::iterator& i);
Node * stats(vector<Token>::iterator& i);
Node * mstat(vector<Token>::iterator& i);
Node * stat(vector<Token>::iterator& i);
Node * in(vector<Token>::iterator& i);
Node * out(vector<Token>::iterator& i);
Node * ifStat(vector<Token>::iterator& i);
Node * loop(vector<Token>::iterator& i);
Node * loop1(vector<Token>::iterator& i);
Node * loop2(vector<Token>::iterator& i);
Node * assign(vector<Token>::iterator& i);
Node * label(vector<Token>::iterator& i);
Node * gotoStat(vector<Token>::iterator& i);
Node * RO(vector<Token>::iterator& i);


#endif