#include <iostream>
#include <string>

#include "labels.h"

using namespace std;


void printError(string errorMessage) {
    static bool printedError = false; // error and exit with only one error message
    if (!printedError){
        cout << errorMessage;
    }
    printedError = true;
}

string printLabel(Label label){
	switch (label)
	{
	case PROGRAM_LBL:
		return "program";
	case BLOCK_LBL:
		return "block";
	case VARS_LBL:
		return "vars";
	case EXPR_LBL:
		return "expr";
	case N_LBL:
		return "N";
	case X_LBL:
		return "X";
	case A_LBL:
		return "A";
	case M_LBL:
		return "M";
	case R_LBL:
		return "R";
	case STATS_LBL:
		return "stats";
	case MSTATS_LBL:
		return "mstats";
	case STAT_LBL:
		return "stat";
	case IN_LBL:
		return "in";
	case OUT_LBL:
		return "out";	
	case IF_LBL:
		return "if";
	case LOOP1_LBL:
		return "loop1";
	case LOOP2_LBL:
		return "loop2";
	case ASSIGN_LBL:
		return "assign";
	case RO_LBL:
		return "RO";
	case LABEL_LBL:
		return "label";
	case GOTO_LBL:
		return "goto";	
	default:
		return "nothing";
	}
}
