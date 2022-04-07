#ifndef NODE_H
#define NODE_H

#include <string>
#include "tokenID.h"

using namespace std;

struct Node
	{
		string label;		//function that createed this non terminal
		string token;       //potential tokens that need to be stored

		Node *left;		  //pointer to left child node
        Node *middle1;   //pointer to first middle child node
        Node *middle2;   //pointer to second middle child node
		Node *right;	//pointer to right child node
	};

#endif