#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include "token.h"

vector<Token> scannerUtility(string fileName);
void parser(string fileName);
void printError(int expecting, string actual);

//BNF
void program(vector<Token>::iterator& i);
void block(vector<Token>::iterator& i);
void vars(vector<Token>::iterator& i);
void stats(vector<Token>::iterator& i);
void mstat(vector<Token>::iterator& i);
void stat(vector<Token>::iterator& i);
void in(vector<Token>::iterator& i);
void ifStat(vector<Token>::iterator& i);


#endif