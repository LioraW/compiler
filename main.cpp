#include <iostream>
#include <string>
#include <fstream>

#include "testScanner.h"

bool fileNameContainsDot(string FileName);
string validateFileName(string fileName);

int main(int argc, char **argv) {
	string fileName;

    // If there is no argument
    if (argc <= 1) {
        
        // open a temp file 
        fileName = "output";
        ofstream inputfile(fileName + ".sp2022", ios::trunc);
        string text = "";

        //read keyboard input into temp file
        while (cin >> text && text != "EOF") {
            inputfile << text << " ";
        }

        inputfile.close();

    //else use the argument
    } else {
        fileName = validateFileName(argv[1]);
    }
    
if (fileName != "") {
       testScanner(fileName + ".sp2022");
}

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