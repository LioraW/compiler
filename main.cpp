#include <iostream>
#include <string>
#include "tokenID.h"

using namespace std;

bool fileNameContainsDot(string FileName);
string validateFileName(string fileName);

int main(int argc, char **argv) {
	
    fstream file;
	string word;
    string inputFileName = "testFile.txt"
    int lineNumber = 0;
	
	file.open(inputFileName);

	if (file.is_open()) {
		
		while (file >> word) {

			filter(word, lineNumber)
		}

	} else {
		cerr << "Unable to open file: " << inputFileName << endl;
	}   

	file.close();

    
    return 0;
}

bool fileNameContainsDot(string fileName) {
    for (int i = 0; i < fileName.length(); i++) {
        if (fileName[i] == '.') {
            return true;
        }
    }
    return false;
}

string validateFileName(string fileName) {
    
    string fileExtension = fileName.substr(fileName.length() - 7, fileName.length());
    
    //the file should have the extension .sp2022 or no extension at all
    if (!fileNameContainsDot(fileName)){
        return fileName;
    } else if (fileExtension == ".sp2022") {
        return fileName.substr(0, fileName.length()  - 7);
    } else {
        cerr << "Bad file extension: " << fileName << endl;
        return "";
    }
}