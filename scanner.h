#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include "token.h"

using namespace std;

string trim(const string str);

char filter ( string line, int lineNumber, int& charNum, bool& openComment);
Token scanner(string line, int lineNum, int& charNum, int lineLength);
bool isKeyword(string word); 
bool isFinalState(int state);
int FSAColumn(char ch);

#endif