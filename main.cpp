#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "parser.h"
#include "node.h"
#include "statSem.h"
#include "tree.h"

bool fileNameContainsDot(string FileName);
string validateFileName(string fileName);
void makeInputFile(string fileName);

int main(int argc, char **argv) {
	string fileName;

    // If there is no argument
    if (argc <= 1) {
        //make a temp file for input
        fileName = "output";
        makeInputFile(fileName);

    //else use the argument
    } else {
        fileName = validateFileName(argv[1]);
    }
    
    //however we got the input, continue to compile
    if (fileName != "") {
        Node * tree = parser(fileName + ".sp2022"); //parse tree
        Stack varStack;                             //tracks variable declarations
        
        //create an ouput file
        ofstream file;
        
        //open and truncate any previous data. then close it  
        file.open(fileName + ".asm");
        file.close();

        //now open it for appending
        file.open(fileName + ".asm", ios::app);

        if (file.is_open()){
            //check static semantics and generate code
            codeGeneration(file, tree, varStack);
            writeFileVarDeclarations(file);
            file.close();

        } else {
            cout << "Error opening outputfile" << endl;
        }

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

//make a temp file for input
void makeInputFile(string fileName){
    ofstream inputfile(fileName + ".sp2022", ios::trunc);
    string text = "";

    //read keyboard input into temp file
    while (cin >> text && text != "EOF") {
        inputfile << text << " ";
    }

    inputfile.close();
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