#ifndef TREE_H
#define TREE_H

#include <string.h>
#include "node.h"
#include "labels.h"

	Node * getNode(Label label);						//allocate memeory for a node
	void destroySubTree(Node *nodePtr);				 	//delete all of the nodes below and including the given node
	void processNode(Node *nodePtr, int level); 		//print the node
    void printPreorder(Node *nodePtr, int level);       //display the tree's node in pre order
	string printLabel(Label label);						//get a string for the node's label



#endif