#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "testScanner.h"
#include "scanner.h"

using namespace std;

void testScanner(string fileName) {
    
    fstream file;
    string line, cleanLine;
    int numberLines = 0;
    bool openComment = false;

        
    file.open(fileName);

    if (file.is_open()) {
        
        while (getline(file, line)) {
            cleanLine = filter(line, numberLines++, openComment);
            cout << "Line " << numberLines << ":" << cleanLine << endl;
        }

    } else {
        cerr << "Unable to open file: " << fileName << endl;
    }   

    file.close();
    
    
}
