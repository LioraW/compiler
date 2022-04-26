#include <iostream>
#include <string>
#include <ctype.h>
#include <fstream>
 
#include "node.h"
#include "tree.h"
#include "labels.h"

using namespace std;

//allocates and returns a pointer to a new node
Node * getNode(Label label) {
	
	Node *newNode = nullptr;

	//Create a new node
	newNode = new Node;
	newNode->label = label;
	newNode->token = "";
	newNode->left = newNode->middle1 = newNode->middle2 = newNode->right = nullptr;

	return newNode;
}

void destroySubTree(Node *nodePtr) {
	//delete all of the nodes below and including the given node	
	if (nodePtr) {
		if (nodePtr -> left)
			destroySubTree(nodePtr->left);
		if(nodePtr -> middle1)
			destroySubTree(nodePtr->middle1);
		if(nodePtr -> middle2)
			destroySubTree(nodePtr->middle2);
		if (nodePtr->right)
			destroySubTree(nodePtr->right);
		delete nodePtr;
	}
}

void processNode(Node * nodePtr, int level) {

	//indent 2*level
	int numSpaces = level * 2;
	while (numSpaces--)
		cout << " ";
	
	// write word to file
	cout << printLabel(nodePtr->label);
	if (nodePtr->token != ""){
		cout << ": " << nodePtr->token;
	}
	cout << endl;

}

void printPreorder(Node *nodePtr, int level) {
	//display the tree's nodes in pre order
	if (nodePtr) {
		processNode(nodePtr, level);
		printPreorder(nodePtr->left, level + 1); 
		printPreorder(nodePtr->middle1, level + 1);
		printPreorder(nodePtr->middle2, level + 1);
		printPreorder(nodePtr->right, level + 1);
	}
}

// string printLabel(Label label){
// 	switch (label)
// 	{
// 	case PROGRAM_LBL:
// 		return "program";
// 	case BLOCK_LBL:
// 		return "block";
// 	case VARS_LBL:
// 		return "vars";
// 	case EXPR_LBL:
// 		return "expr";
// 	case N_LBL:
// 		return "N";
// 	case X_LBL:
// 		return "X";
// 	case A_LBL:
// 		return "A";
// 	case M_LBL:
// 		return "M";
// 	case R_LBL:
// 		return "R";
// 	case STATS_LBL:
// 		return "stats";
// 	case MSTATS_LBL:
// 		return "mstats";
// 	case STAT_LBL:
// 		return "stat";
// 	case IN_LBL:
// 		return "in";
// 	case OUT_LBL:
// 		return "out";	
// 	case IF_LBL:
// 		return "if";
// 	case LOOP1_LBL:
// 		return "loop1";
// 	case LOOP2_LBL:
// 		return "loop2";
// 	case ASSIGN_LBL:
// 		return "assign";
// 	case RO_LBL:
// 		return "RO";
// 	case LABEL_LBL:
// 		return "label";
// 	case GOTO_LBL:
// 		return "goto";	
// 	default:
// 		return "nothing";
// 	}
// }
