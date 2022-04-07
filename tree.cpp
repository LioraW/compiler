#include <iostream>
#include <string>
#include <ctype.h>
#include <fstream>
 
#include "node.h"
#include "tree.h"

using namespace std;

//allocates and returns a pointer to a new node
Node * getNode(string label) {
	
	Node *newNode = nullptr;

	//Create a new node
	newNode = new Node;
	newNode->label = label;
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

void processNode(string word, int level) {

	//indent 2*level
	int numSpaces = level * 2;
	while (numSpaces--)
		cout << " ";
	
	// write word to file
	cout << word << endl;

}

void printPreorder(Node *nodePtr, int level) {
	//display the tree's nodes in pre order
	if (nodePtr) {
		processNode(nodePtr->label, level);
		printPreorder(nodePtr->left, level + 1); 
		printPreorder(nodePtr->middle1, level + 1);
		printPreorder(nodePtr->middle2, level + 1);
		printPreorder(nodePtr->right, level + 1);
	}
}

	


	

