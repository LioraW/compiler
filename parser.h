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
void expr(vector<Token>::iterator& i);
void N(vector<Token>::iterator& i);
void X(vector<Token>::iterator& i);
void A(vector<Token>::iterator& i);
void M(vector<Token>::iterator& i);
void R(vector<Token>::iterator& i);
void stats(vector<Token>::iterator& i);
void mstat(vector<Token>::iterator& i);
void stat(vector<Token>::iterator& i);
void in(vector<Token>::iterator& i);
void out(vector<Token>::iterator& i);
void ifStat(vector<Token>::iterator& i);
void loop(vector<Token>::iterator& i);
void loop1(vector<Token>::iterator& i);
void loop2(vector<Token>::iterator& i);
void assign(vector<Token>::iterator& i);
void label(vector<Token>::iterator& i);
void gotoStat(vector<Token>::iterator& i);
void RO(vector<Token>::iterator& i);


#endif