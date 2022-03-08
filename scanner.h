#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <fstream>
#include "token.h"

using namespace std;

char filter   (string line, int& charNum, bool& openComment);
Token scanner ( string line, int& charNum, bool& openComment, int lineNum, fstream& file);
int keywordIndex(string word); 
bool isFinalState(int state);
int FSAColumn(char ch);

#endif