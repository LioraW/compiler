#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include "token.h"

vector<Token> scannerUtility(string fileName);
void parser(string fileName);
void program(vector<Token> tokens, vector<Token>::iterator& i);
void block();
void vars(vector<Token> tokens, vector<Token>::iterator& i);

#endif