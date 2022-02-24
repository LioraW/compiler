#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "testScanner.h"

using namespace std;



void testScanner(string fileName) {
    
fstream file;
string line;
int numberLines = 0;
	
file.open(fileName);

	if (file.is_open()) {
		
		while (getline(file, line)) {
			processLine(line, numberLines++);
		}

	} else {
		cerr << "Unable to open file: " << fileName << endl;
	}   

	file.close();
    
    
}

void processLine (string line, int lineNumber) {
    cout << "Line Number: " << lineNumber << endl;
    for (int i = 0; i < line.size(); i++) {
                cout << line[i] << endl;
    }
}
