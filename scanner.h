#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include "token.h"

using namespace std;

string trim(const string str);

string filter (string line, int lineNumber, bool& openComment);
Token scanner(string line, int lineNum, int& charNum, int lineLength);
bool isKeyword(string word); 
int FSAColumn(char ch);

#endif